#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libuser.h"

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
void fullwindow_upload(struct gl41world* world)
{
	struct gl41data** cam = world->camera;
	struct gl41data** lit = world->light;
	struct gl41data** solid = world->solid;
	struct gl41data** opaque = world->opaque;

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




void updatearg(u32 shader, struct gl41data* data)
{
	int j;
	int iii;
	u32 uuu;
	struct mysrc* src = &data->src;
	struct gldst* dst = &data->dst;

#ifndef __ANDROID__
	if((dst->routine_name) && (dst->routine_detail)){
		//iii = glGetSubroutineUniformLocation(shader, GL_FRAGMENT_SHADER, dst->routine_name);
		uuu = glGetSubroutineIndex(shader, GL_FRAGMENT_SHADER, dst->routine_detail);
		if(uuu < 256)glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &uuu);
	}
#endif

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
//say("tex=%x\n", dst->tex[j]);
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
void render_target(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque, _obj* wnd, struct fstyle* area)
{
	//say("fullwindow_render:%llx,%llx,%llx,%llx,%llx,%llx\n",cam,lit,solid,opaque,wnd,area);
	int j;
	int x0,y0,ww,hh;
	if(0 == area){
		glViewport(0, 0, 1024, 1024);
		glScissor(0, 0, 1024, 1024);
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else{
		x0 = area->vc[0] * wnd->whdf.fbwidth;
		y0 = area->vc[1] * wnd->whdf.fbheight;
		ww = area->vq[0] * wnd->whdf.fbwidth;
		hh = area->vq[1] * wnd->whdf.fbheight;
		glViewport(x0, y0, ww, hh);
		glScissor(x0, y0, ww, hh);
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);

#ifndef __ANDROID__
	glPointSize(4.0*wnd->whdf.fbwidth/wnd->whdf.width);
#endif
	glLineWidth(4.0*wnd->whdf.fbwidth/wnd->whdf.width);

	//solid
	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		if(0 == solid[j]->src.vtx[0].vbuf)continue;
		render_material(cam[0], lit[0], solid[j]);
	}

	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(j=0;j<64;j++){
		if(0 == opaque[j])continue;
		if(0 == opaque[j]->src.vtx[0].vbuf)continue;
		render_material(cam[0], lit[0], opaque[j]);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void fullwindow_render(struct gl41world* world, _obj* wnd, struct fstyle* area)
{
	struct gl41data** cam = world->camera;
	struct gl41data** lit = world->light;
	struct gl41data** solid = world->solid;
	struct gl41data** opaque = world->opaque;

	int j;
	for(j=8;j>0;j--){
		if(0 == cam[j])continue;
		say("%d\n",j);
		glBindFramebuffer(GL_FRAMEBUFFER, cam[j]->dst.fbo);
		render_target(&cam[j],lit, solid,opaque, wnd,0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if(cam[0] && cam[0]->dst.fbo){
		glBindFramebuffer(GL_FRAMEBUFFER, cam[0]->dst.fbo);
		render_target(cam,lit, solid,opaque, wnd,area);
		say("gbuf:d=%d,c0=%d,c1=%d,c2=%d,c3=%d\n",cam[0]->dst.rbo, cam[0]->dst.tex[0], cam[0]->dst.tex[1], cam[0]->dst.tex[2], cam[0]->dst.tex[3]);
	}
	else{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		render_target(cam,lit, solid,opaque, wnd,area);
	}
}




void fullwindow_dealwith(struct gl41list* perogl, _obj* wnd, struct fstyle* area)
{
	int j;
	for(j=0;j<1;j++){
		//upload
		fullwindow_upload(&wnd->gl41list.world[j]);

		//render
		fullwindow_render(&wnd->gl41list.world[j], wnd, area);
	}
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

		//wnd = rel->psrcchip;		//double check
		struct fstyle* area = rel->psrcfoot;
		if(area){
			//get vertex
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			//stack[sp+0].type = rel->srctype;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			//stack[sp+1].type = rel->dsttype;
			stack[sp+1].flag = rel->dstflag;
			entity_take(rel->pdstchip,rel->pdstfoot, stack,sp+2, arg,cmd, 0,0);

			fullwindow_dealwith(&wnd->gl41list, wnd, area);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int fullwindow_give(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	float x,y,x0,y0,xn,yn;
	short* v;
	struct relation* rel;
	struct fstyle* sty;
	//say("@gl41wnd0_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);

	struct event* ev = buf;
	if(0x4070 == ev->what){
		rel = wnd->oreln;
		while(1){
			if(0 == rel)return 0;
			sty = rel->psrcfoot;
			x0 = sty->vc[0] * wnd->whdf.width;
			y0 = sty->vc[1] * wnd->whdf.height;
			xn = sty->vq[0] * wnd->whdf.width + x0;
			yn = sty->vq[1] * wnd->whdf.height + y0;

			v = (short*)ev;
			x = v[0];
			y = (wnd->whdf.height-1) - v[1];
			if( (x>x0) && (x<xn) && (y>y0) && (y<yn) )goto found;
			rel = samesrcprevdst(rel);
		}
		return 0;
	}
	else{
		rel = wnd->gl41list.glevto;
		if(0 == rel)rel = wnd->oreln;
		if(0 == rel)return 0;
	}

found:
	wnd->gl41list.glevto = rel;
	stack[sp+0].pchip = rel->psrcchip;
	stack[sp+0].pfoot = rel->psrcfoot;
	stack[sp+0].flag = rel->srcflag;

	stack[sp+1].pchip = rel->pdstchip;
	stack[sp+1].pfoot = rel->pdstfoot;
	stack[sp+1].flag = rel->dstflag;
	entity_give(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, ev,0);
	return 0;
}
void fullwindow_delete(_obj* ogl)
{
}
void fullwindow_create(_obj* ogl)
{
	ogl->hfmt = _gl41list_;
	ogl->vfmt= _gl41list_;

	struct gl41world* world = ogl->gl41list.world;
	world[0].camera = memorycreate(0x10000, 0);
	world[0].light  = memorycreate(0x10000, 0);
	world[0].solid  = memorycreate(0x10000, 0);
	world[0].opaque = memorycreate(0x10000, 0);
}
