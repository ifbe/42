#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libuser.h"
#define _quat_ hex32('q','u','a','t')

#ifdef __ANDROID__
	#include <jni.h>
	#include <errno.h>
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <android/log.h>
	#include <android_native_app_glue.h>
#else
	#include <GL/glew.h>
#endif
GLuint uploadvertex(void* i, void* o);
GLuint uploadtexture(void* i, u32 t, void* buf, int fmt, int w, int h);
GLuint shaderprogram(void* v, void* f, void* g, void* tc, void* te, void* c);
int gl41fbo_create(struct gl41data* tar);


//#define DEBUG_PPLL
//#define DEBUG_READPIXEL

#ifdef __APPLE__
#define PPLL_DISABLE
#endif

#ifdef __ANDROID__
#define PPLL_DISABLE
void gpudata_cleanup(u8* ss, u8* ee)
{
	while(ss < ee){
		*ss = 0;
		ss++;
	}
}
void gpudata_validate(_obj* wnd)
{
	struct gl41world* world = &wnd->gl41list.world[0];
	struct gl41data** cam = world->camera;
	struct gl41data** lit = world->light;
	struct gl41data** solid = world->solid;
	struct gl41data** opaque = world->opaque;

	//camera
	int j;
	for(j=0;j<16;j++){
		if(0 == cam[j])break;
		if(cam[j]->dst.ctxage != wnd->gl41list.ctxage){
			say("cam %d\n",j);
			gpudata_cleanup(cam[j]->dst.gpudata_head, cam[j]->dst.gpudata_tail);
			cam[j]->dst.ctxage = wnd->gl41list.ctxage;
		}
	}

	//light
	for(j=0;j<1;j++){
		if(0 == lit[j])break;
		if(lit[j]->dst.ctxage != wnd->gl41list.ctxage){
			say("lit %d\n",j);
			gpudata_cleanup(lit[j]->dst.gpudata_head, lit[j]->dst.gpudata_tail);
			lit[j]->dst.ctxage = wnd->gl41list.ctxage;
		}
	}

	//solid
	for(j=0;j<64;j++)
	{
		if(0 == solid[j])continue;
		if(solid[j]->dst.ctxage != wnd->gl41list.ctxage){
			say("solid %d\n",j);
			gpudata_cleanup(solid[j]->dst.gpudata_head, solid[j]->dst.gpudata_tail);
			solid[j]->dst.ctxage = wnd->gl41list.ctxage;
		}
	}

	//opaque
	for(j=0;j<64;j++)
	{
		if(0 == opaque[j])continue;
		if(opaque[j]->dst.ctxage != wnd->gl41list.ctxage){
			say("opaque %d\n",j);
			gpudata_cleanup(opaque[j]->dst.gpudata_head, opaque[j]->dst.gpudata_tail);
			opaque[j]->dst.ctxage = wnd->gl41list.ctxage;
		}
	}
}
#endif




#define UPLOADSTEP_PPLLWORLD0 0x10
#define UPLOADSTEP_PPLLWORLD1 0x11
#define UPLOADSTEP_GBUFWORLD0 0x20
#define UPLOADSTEP_GBUFWORLD1 0x21
void update_onedraw(struct gldst* dst, struct mysrc* src)
{
	int j;
	int w,h,fmt;
	void* buf0;
	void* buf1;
	void* buf2;
	//say("%llx,%llx\n", dst, src);

//say("@update shader\n");
	//0: shader
	if(dst->shader_deq != src->shader_enq)
	{
		//say("@1\n");
		buf0 = (void*)(src->vs);
		buf1 = (void*)(src->fs);
		buf2 = (void*)(src->gs);
		//say("shader:%llx,%llx,%llx\n", buf0, buf1, buf2);
		if((0 != buf0)&&(0 != buf1))
		{
			dst->shader = shaderprogram(buf0, buf1, buf2, 0, 0, 0);
			//say("shader:(%llx,%llx,%llx)->%x\n", buf0, buf1, buf2, fd);
		}

		dst->shader_deq = src->shader_enq;
	}

//say("@update vertex\n");
	//2: vertex
	if(	(dst->vbo_deq != src->vbuf_enq) |
		(dst->ibo_deq != src->ibuf_enq) )
	{
		//say("@4\n");
		uploadvertex(dst, src);
		//say("vertex:(%x,%x,%x)\n", dst->vao, dst->vbo, dst->ibo);
		dst->vbo_deq = src->vbuf_enq;
		dst->ibo_deq = src->ibuf_enq;
	}

//say("@update texture\n");
	//3: texture
	for(j=0;j<5;j++){
		if(dst->tex_deq[j] == src->tex_enq[j])continue;

		fmt = src->tex[j].fmt;
		if('!' == fmt){
			dst->tex[j] = src->tex[j].glfd;
		}
		else{
			buf0 = (void*)(src->tex[j].data);
			if(0 != buf0){
				w = src->tex[j].w;
				h = src->tex[j].h;
				dst->tex[j] = uploadtexture(dst, dst->tex[j], buf0, fmt, w, h);
				//say("texture:(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, dst->tex[j]);
			}
		}

		dst->tex_deq[j] = src->tex_enq[j];
	}
//say("@update done\n");
}
#ifndef PPLL_DISABLE
void update_ppll(struct gldst* dst, struct mysrc* src)
{
	GLuint data[4] = {1,2,3,4};
	if(0 == dst->ppll_atomic){
		glGenBuffers(1, &dst->ppll_atomic);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, dst->ppll_atomic);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint)*4, data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	}
	if(0 == dst->ppll_head){
		glGenBuffers(1, &dst->ppll_head);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, dst->ppll_head);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (4*2)*(1024*768)*6, 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	if(0 == dst->ppll_data){
		glGenBuffers(1, &dst->ppll_data);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, dst->ppll_data);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (4*4)*(1024*768)*6, 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

#ifdef DEBUG_PPLL
	//ppll_atomic: read old value
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, dst->ppll_atomic);
	GLuint* rbuf = glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_ONLY);
	if(rbuf){
		say("fd=%d,opaquecount=%d\n", dst->ppll_atomic, rbuf[0]);
	}
	else{
		say("err@glMapBuffer:fd=%d,glGetError=%d\n", dst->ppll_atomic, glGetError());
	}
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
#endif

	//ppll_atomic: write new value
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, dst->ppll_atomic);
	GLuint* wbuf = glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
	if(wbuf)wbuf[0] = 0;
	else say("err@glMapBuffer:fd=%d,glGetError=%d\n", dst->ppll_atomic, glGetError());
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

#ifdef DEBUG_PPLL
	//ppll_head: read old value
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dst->ppll_head);
	int xxxx = 384*1024+512;		//screen center
	int temp = 0;
	GLuint* head_rbuf = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	if(head_rbuf){
		temp = head_rbuf[xxxx*2+0];
		say("head: prev=%d,count=%d\n", head_rbuf[xxxx*2+0], head_rbuf[xxxx*2+1]);
	}
	else say("err@glMapBuffer:fd=%d,glGetError=%d\n", dst->ppll_head, glGetError());
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
#endif

	//ppll_head: write new value
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dst->ppll_head);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
	//say("glGetError=%d\n",glGetError());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

#ifdef DEBUG_PPLL
	//ppll_data: read old value
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dst->ppll_data);
	GLuint* data_rbuf = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	if(data_rbuf){
		while((temp > 0) && (temp < 1024*768*4)){
			say("data[%x]: prev=%d,temp=%d,color=%x,depth=%f\n", temp,
				data_rbuf[temp*4+0], data_rbuf[temp*4+1], data_rbuf[temp*4+2], *(float*)&data_rbuf[temp*4+3]);
			temp = data_rbuf[temp*4+0];
		}
	}
	else say("err@glMapBuffer:fd=%d,glGetError=%d\n", dst->ppll_data, glGetError());
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
#endif

	//ppll_data: write new value
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dst->ppll_data);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
	//say("glGetError=%d\n",glGetError());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
#endif
void fullwindow_upload(struct gl41world* world, int step)
{
	struct gl41data** cam = world->camera;
	struct gl41data** lit = world->light;
	struct gl41data** solid = world->solid;
	struct gl41data** opaque = world->opaque;

#ifndef PPLL_DISABLE
	if(UPLOADSTEP_PPLLWORLD0 == step){
		update_ppll(&cam[0]->dst, &cam[0]->src);
	}
#endif

	//camera
	int j;
	for(j=0;j<16;j++){
		if(0 == cam[j])break;
		if(cam[j]->dst.target_deq != cam[j]->src.target_enq){
			gl41fbo_create(cam[j]);
			cam[j]->dst.target_deq = cam[j]->src.target_enq;
		}
	}

	//light
	for(j=0;j<1;j++){
		if(0 == lit[j])break;
		update_onedraw(&lit[j]->dst, &lit[j]->src);
	}

	//solid
	for(j=0;j<64;j++)
	{
		if(0 == solid[j])continue;
		if(0 == solid[j]->src.vtx[0].vbuf)continue;
		//say("%d\n",j);
		update_onedraw(&solid[j]->dst, &solid[j]->src);
	}

	//opaque
	for(j=0;j<64;j++)
	{
		if(0 == opaque[j])continue;
		if(0 == opaque[j]->src.vtx[0].vbuf)continue;
		//say("%d\n",j);
		update_onedraw(&opaque[j]->dst, &opaque[j]->src);
	}
}




#define RENDERSTEP_PPLLWORLD0 0x10
#define RENDERSTEP_PPLLWORLD1 0x11
#define RENDERSTEP_GBUFWORLD0 0x20
#define RENDERSTEP_GBUFWORLD1 0x21
void updatearg(u32 shader, struct gl41data* data)
{
	int j;
	int iii;
	u32 uuu;
	struct mysrc* src = &data->src;
	struct gldst* dst = &data->dst;

/*
#ifndef __ANDROID__
	if((dst->routine_name) && (dst->routine_detail)){
		//iii = glGetSubroutineUniformLocation(shader, GL_FRAGMENT_SHADER, dst->routine_name);
		uuu = glGetSubroutineIndex(shader, GL_FRAGMENT_SHADER, dst->routine_detail);
		if(uuu < 256)glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &uuu);
	}
#endif
*/
	if(dst->lighttype){
		iii = glGetUniformLocation(shader, "lighttype");
		if(iii >= 0){
			//say("iii=%d,lighttype=%d\n",iii,dst->lighttype);
			glUniform1i(iii, dst->lighttype);
		}
	}

	for(j=0;j<4;j++){
		if(0 == dst->arg[j].name)break;
		if(0 == dst->arg[j].data)break;
//say("%d,%d,%llx,%s\n", j, shader, src, dst->arg[j].name);

		iii = glGetUniformLocation(shader, dst->arg[j].name);
		if(iii < 0)continue;

		switch(dst->arg[j].fmt){
			case 'm':{
				glUniformMatrix4fv(iii, 1, GL_FALSE, dst->arg[j].data);
				break;
			}//mat4
			case 'v':{
				glUniform3fv(iii, 1, dst->arg[j].data);
				break;
			}//vertex
			case 'f':{
				glUniform1fv(iii, 1, dst->arg[j].data);
				break;
			}//float
		}//switch
	}//for
}
void render_material(struct gl41data* cam, struct gl41data* lit, struct gl41data* data)
{
	int j,k;
	u32 fmt;
	u32 vbo;
	u32 vao;
	struct mysrc* src = &data->src;
	struct gldst* dst = &data->dst;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	//0.shader
	glUseProgram(dst->shader);

#ifndef PPLL_DISABLE
	if(cam && cam[0].dst.ppll_data){
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, cam[0].dst.ppll_atomic);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cam[0].dst.ppll_head);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, cam[0].dst.ppll_data);
	}
#endif

	//1.argument
	updatearg(dst->shader, data);
	if(lit)updatearg(dst->shader, lit);
	if(cam)updatearg(dst->shader, cam);

	//2.texture
	k = 0;
	if(lit){
		for(j=0;j<5;j++){
			if(0 == lit->dst.texname[j])continue;
			if(0 == lit->src.tex[j].data)continue;

			glActiveTexture(GL_TEXTURE0 + k);
			glBindTexture(GL_TEXTURE_2D, lit->dst.tex[j]);
			glUniform1i(glGetUniformLocation(dst->shader, lit->dst.texname[j]), k);
			k++;
		}
	}
	for(j=0;j<5;j++)
	{
		if(0 == dst->tex[j])continue;
		if(0 == dst->texname[j])continue;
		glActiveTexture(GL_TEXTURE0 + k);
		glBindTexture(GL_TEXTURE_2D, dst->tex[j]);
		glUniform1i(glGetUniformLocation(dst->shader, dst->texname[j]), k);
		k++;
	}

	//3.vertex
	fmt = src->vtx[0].vbuf_fmt;
	vbo = dst->vbo;
	vao = dst->vao;
	glBindVertexArray(vao);
//say("%d,%d,%d,%d,%d,%d\n", dst->shader, dst->tex[0], vao, vbo, src->ibuf_h, src->vbuf_h);

//say("8888@opaque=%x, geom=%x, vbuf_h=%x, ibuf_h=%x\n", src->opaque, src->geometry, src->vbuf_h, src->ibuf_h);
	struct vertex* vtx = src->vtx;
#ifndef __ANDROID__
	if((3==vtx->geometry)&&(2==vtx->fill)){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
#endif
	if(vtx->ibuf)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
		if(1 == vtx->geometry)glDrawElements(GL_POINTS, vtx->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else if(2 == vtx->geometry)glDrawElements(GL_LINES, 2*vtx->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else glDrawElements(GL_TRIANGLES, 3*vtx->ibuf_h, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		if(1 == vtx->geometry)glDrawArrays(GL_POINTS, 0, vtx->vbuf_h);
		else if(2 == vtx->geometry)glDrawArrays(GL_LINES, 0, vtx->vbuf_h);
		else glDrawArrays(GL_TRIANGLES, 0, vtx->vbuf_h);
	}
}
void render_world(struct gl41data* cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque, int step)
{
	int j;
	//say("fullwindow_render:%llx,%llx,%llx,%llx,%llx,%llx\n",cam,lit,solid,opaque,wnd,area);

	if(RENDERSTEP_PPLLWORLD1 == step){
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_SRC_ALPHA);
	}

	//solid
	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		if(0 == solid[j]->src.vtx[0].vbuf)continue;
		render_material(cam, lit[0], solid[j]);
	}

	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(j=0;j<64;j++){
		if(0 == opaque[j])continue;
		if(0 == opaque[j]->src.vtx[0].vbuf)continue;
		render_material(cam, lit[0], opaque[j]);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void render_target(_obj* wnd, struct fstyle* area, struct gl41data* cam, int step)
{
	int clear;
	int x0,y0,ww,hh;
	if(cam && cam->dst.fbo){		//draw fbo, default fullscreen
		x0 = 0;
		y0 = 0;
		ww = cam->src.tex[0].w;
		hh = cam->src.tex[0].h;
		clear = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	}
	else{	//draw backbuffer
		if(area){	//setarea
			x0 = area->vc[0] * wnd->whdf.fbwidth;
			y0 = area->vc[1] * wnd->whdf.fbheight;
			ww = area->vq[0] * wnd->whdf.fbwidth;
			hh = area->vq[1] * wnd->whdf.fbheight;
			clear = GL_DEPTH_BUFFER_BIT;
		}
		else{	//fullscreen
			x0 = 0;
			y0 = 0;
			ww = wnd->whdf.fbwidth;
			hh = wnd->whdf.fbheight;
			clear = GL_DEPTH_BUFFER_BIT;
		}
	}

	glViewport(x0, y0, ww, hh);
	glScissor(x0, y0, ww, hh);

	//ppll second overlay, dont clear depth
	if(RENDERSTEP_PPLLWORLD1 == step){
		glDisable(GL_DEPTH_TEST);
	}
	else{
		//glDepthMask(GL_TRUE);
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(clear);

		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
	}

#ifndef __ANDROID__
	glPointSize(4.0*wnd->whdf.fbwidth/wnd->whdf.width);
#endif
	glLineWidth(4.0*wnd->whdf.fbwidth/wnd->whdf.width);
}
void fullwindow_render(struct gl41world* world, int step, _obj* wnd, struct fstyle* area)
{
	struct gl41data** cam = world->camera;
	struct gl41data** lit = world->light;
	struct gl41data** solid = world->solid;
	struct gl41data** opaque = world->opaque;

	int j;
	for(j=8;j>0;j--){
		if(0 == cam[j])continue;
		//say("%d\n",j);
		glBindFramebuffer(GL_FRAMEBUFFER, cam[j]->dst.fbo);
		render_target(wnd, 0, cam[j], step);
		render_world(cam[j],lit, solid,opaque, step);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if(cam[0] && cam[0]->dst.fbo){
		glBindFramebuffer(GL_FRAMEBUFFER, cam[0]->dst.fbo);
		render_target(wnd,area, cam[0], step);
		render_world(cam[0],lit, solid,opaque, step);
		//say("gbuf:fbo=%d,d=%d,c0=%d,c1=%d,c2=%d,c3=%d\n",cam[0]->dst.fbo,cam[0]->dst.rbo, cam[0]->dst.tex[0], cam[0]->dst.tex[1], cam[0]->dst.tex[2], cam[0]->dst.tex[3]);
	}
	else{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		render_target(wnd,area, cam[0], step);
		render_world(cam[0],lit, solid,opaque, step);
	}
}
void fullwindow_uploadandrender(_obj* wnd, struct fstyle* area)
{
	switch(wnd->vfmt){
	case _gbuf_:
		//render to gbuf
		fullwindow_upload(&wnd->gl41list.world[0], UPLOADSTEP_GBUFWORLD0);
		fullwindow_render(&wnd->gl41list.world[0], RENDERSTEP_GBUFWORLD0, wnd, area);

		//gbuf to screen
		fullwindow_upload(&wnd->gl41list.world[1], UPLOADSTEP_GBUFWORLD1);
		fullwindow_render(&wnd->gl41list.world[1], RENDERSTEP_GBUFWORLD1, wnd, area);
		break;
	case _ppll_:
		//render to screen and ssbo
		fullwindow_upload(&wnd->gl41list.world[0], UPLOADSTEP_PPLLWORLD0);
		fullwindow_render(&wnd->gl41list.world[0], RENDERSTEP_PPLLWORLD0, wnd, area);

		//ssbo to screen
		fullwindow_upload(&wnd->gl41list.world[1], UPLOADSTEP_PPLLWORLD1);
		fullwindow_render(&wnd->gl41list.world[1], RENDERSTEP_PPLLWORLD1, wnd, area);
		break;
	default:
		fullwindow_upload(&wnd->gl41list.world[0], 0);
		fullwindow_render(&wnd->gl41list.world[0], 0, wnd, area);
		break;
	}

#ifdef DEBUG_READPIXEL
	u32 color[4];
	float depth[4];
	glReadPixels(512, 384, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glReadPixels(512, 384, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
	say("c=%x,d=%f\n", color[0], depth[0]);
#endif
}




int fullwindow_trytosend(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int cmd, struct relation* rel, struct event* ev)
{
	stack[sp+0].pchip = rel->psrcchip;
	stack[sp+0].pfoot = rel->psrcfoot;
	stack[sp+0].foottype = rel->srcfoottype;

	stack[sp+1].pchip = rel->pdstchip;
	stack[sp+1].pfoot = rel->pdstfoot;
	stack[sp+1].foottype = rel->dstfoottype;
	return entity_giveby(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, ev,0);
}
int fullwindow_chooseandsend(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int cmd, struct event* ev)
{
	int ret;
	float x,y,x0,y0,xn,yn;
	short* v;
	struct fstyle* sty;
	struct relation* rel = 0;
	//say("@gl41wnd0_write:(%p,%x)(%llx,%llx,%llx,%llx)\n", arg,cmd, ev->why, ev->what, ev->where, ev->when);
/*
	if(0x2b70 == ev->what){
		float color[4];
		float depth[4];
		v = (short*)ev;
		x = v[0];
		y = (wnd->whdf.height-1) - v[1];
		glReadPixels(x, y, 1, 1, GL_BLUE, GL_FLOAT, color);
		glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
		say("x=%f,y=%f,c=%f,d=%f\n", x, y, color[0], depth[0]);
	}
*/
	if(0x4070 == ev->what){
		rel = wnd->oreln;
		while(1){
			if(0 == rel)return 0;
			sty = rel->psrcfoot;
			if(sty){
				x0 = sty->vc[0] * wnd->whdf.width;
				y0 = sty->vc[1] * wnd->whdf.height;
				xn = sty->vq[0] * wnd->whdf.width + x0;
				yn = sty->vq[1] * wnd->whdf.height + y0;

				v = (short*)ev;
				x = v[0];
				y = (wnd->whdf.height-1) - v[1];
				if( (x>x0) && (x<xn) && (y>y0) && (y<yn) ){
					wnd->gl41list.glevto = rel;
					ret = fullwindow_trytosend(wnd,foot, stack,sp, arg,cmd, rel,ev);
					if(_thru_ != ret)goto senddone;
				}
			}
			rel = samesrcprevdst(rel);
		}
		return 0;
	}
	else if(_quat_ == cmd){
		rel = wnd->oreln;
		while(1){
			if(0 == rel)return 0;

			ret = fullwindow_trytosend(wnd,foot, stack,sp, arg,cmd, rel,ev);
			if(_thru_ != ret)goto senddone;

			rel = samesrcprevdst(rel);
		}
		return 0;
	}
	else{
		rel = wnd->gl41list.glevto;
		if(0 == rel)rel = wnd->oreln;
		if(0 == rel)return 0;
	}

sendevent:
	if(rel){
		fullwindow_trytosend(wnd,foot, stack,sp, arg,cmd, rel,ev);
	}

senddone:
	return 0;
}




int fullwindow_take(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	//say("@gl41wnd0_read\n");
	//say("%d,%llx@fullwindow_renderwnd\n", rsp, stack);
	//say("gl41wnd0_read:%llx,%llx,%llx,%x,%llx,%d\n",self,peer,stack,rsp,buf,len);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, wnd->whdf.fbwidth, wnd->whdf.fbheight);
	glScissor(0, 0, wnd->whdf.fbwidth, wnd->whdf.fbheight);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//foreach camera
	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		//get vertex
		stack[sp+0].pchip = rel->psrcchip;
		stack[sp+0].pfoot = rel->psrcfoot;
		//stack[sp+0].type = rel->srctype;
		stack[sp+0].foottype = rel->srcfoottype;
		stack[sp+1].pchip = rel->pdstchip;
		stack[sp+1].pfoot = rel->pdstfoot;
		//stack[sp+1].type = rel->dsttype;
		stack[sp+1].foottype = rel->dstfoottype;
		entity_takeby(rel->pdstchip,rel->pdstfoot, stack,sp+2, arg,cmd, 0,0);

#ifdef __ANDROID__
		gpudata_validate(wnd);
#endif

		//wnd = rel->psrcchip;		//double check
		struct fstyle* area = rel->psrcfoot;
		if(area)fullwindow_uploadandrender(wnd, area);

next:
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int fullwindow_give(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
/*
	if(mouse event from window)send to user
	if(draw command from user)draw to window
*/
	_obj* caller = 0;
	u64 callertype = 0;
	if(sp >= 2){
		caller = stack[sp-2].pchip;
		if(caller)callertype = caller->hfmt;
	}
/*	if(_camrts_ == callertype)
	{
		say("@%s\n",__FUNCTION__);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, wnd->whdf.fbwidth, wnd->whdf.fbheight);
		glScissor(0, 0, wnd->whdf.fbwidth, wnd->whdf.fbheight);
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT);

		fullwindow_uploadandrender(wnd, 0);
		return 0;
	}
*/
	fullwindow_chooseandsend(wnd,foot, stack,sp, arg,cmd, buf);
	return 0;
}
void fullwindow_delete(_obj* ogl)
{
}
void fullwindow_create(_obj* ogl, void* arg, int argc, char** argv)
{
	int j;
	say("argc=%d,argv=%p\n",argc,argv);
	for(j=0;j<argc;j++){
		say("arg%d:%.4s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "vfmt:gbuf", 9))ogl->vfmt = _gbuf_;
		if(0 == ncmp(argv[j], "vfmt:ppll", 9))ogl->vfmt = _ppll_;
	}

	ogl->hfmt = _gl41list_;
	//ogl->vfmt= _gbuf_;

	struct gl41world* world = ogl->gl41list.world;
	world[0].camera = memorycreate(0x10000, 0);
	world[0].light  = memorycreate(0x10000, 0);
	world[0].solid  = memorycreate(0x10000, 0);
	world[0].opaque = memorycreate(0x10000, 0);

	if((_gbuf_ == ogl->vfmt)|(_ppll_ == ogl->vfmt)){
		world[1].camera = memorycreate(0x10000, 0);
		world[1].light  = memorycreate(0x10000, 0);
		world[1].solid  = memorycreate(0x10000, 0);
		world[1].opaque = memorycreate(0x10000, 0);
	}
}
