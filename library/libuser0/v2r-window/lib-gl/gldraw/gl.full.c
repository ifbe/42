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
int gl41wnd0_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len);
int gl41wnd0_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len);



void update_onedraw(struct gldst* dst, struct glsrc* src)
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
	for(j=0;j<4;j++){
		if(dst->tex_deq[j] == src->tex[j].enq)continue;

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
				//say("texture:(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, fd);
			}
		}

		dst->tex_deq[j] = src->tex[j].enq;
	}
//say("@update done\n");
}
void fullwindow_upload(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque)
{
	int j;
	//say("fullwindow_upload:%llx,%llx,%llx,%llx\n",cam,lit,solid,opaque);

	//camera
	//update_onedraw(&cam[0].dst, &cam[0].src);

	//light
	if(lit[0])update_onedraw(&lit[0]->dst, &lit[0]->src);

	//solid
	for(j=0;j<64;j++)
	{
		if(0 == solid[j])continue;
		if(0 == solid[j]->src.vbuf)continue;
		//say("%d\n",j);
		update_onedraw(&solid[j]->dst, &solid[j]->src);
	}

	//opaque
	for(j=0;j<64;j++)
	{
		if(0 == opaque[j])continue;
		if(0 == opaque[j]->src.vbuf)continue;
		//say("%d\n",j);
		update_onedraw(&opaque[j]->dst, &opaque[j]->src);
	}
}




void updatearg(u32 shader, struct glsrc* src)
{
	int j;
	int iii;
	u32 uuu;

	if((src->routine_name) && (src->routine_detail)){
		//iii = glGetSubroutineUniformLocation(shader, GL_FRAGMENT_SHADER, src->routine_name);
		uuu = glGetSubroutineIndex(shader, GL_FRAGMENT_SHADER, src->routine_detail);
		if(uuu < 256)glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &uuu);
	}

	for(j=0;j<4;j++){
		if(0 == src->arg[j].name)break;
		if(0 == src->arg[j].data)break;
//say("%d,%d,%llx,%s\n", j, shader, src, src->arg[j].name);

		iii = glGetUniformLocation(shader, src->arg[j].name);
		if(iii < 0)continue;

		switch(src->arg[j].fmt){
			case 'm':{
				glUniformMatrix4fv(iii, 1, GL_FALSE, src->arg[j].data);
				break;
			}//mat4
			case 'v':{
				glUniform3fv(iii, 1, src->arg[j].data);
				break;
			}//vertex
			case 'f':{
				glUniform1fv(iii, 1, src->arg[j].data);
				break;
			}//float
		}//switch
	}//for
}
void render_onedraw(struct gl41data* cam, struct gl41data* lit, struct gl41data* data)
{
	int j,k;
	u32 fmt;
	u32 vbo;
	u32 vao;
	struct gldst* dst = &data->dst;
	struct glsrc* src = &data->src;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	//0.shader
	glUseProgram(dst->shader);

	//1.argument
	updatearg(dst->shader, src);
	if(lit)updatearg(dst->shader, &lit->src);
	if(cam)updatearg(dst->shader, &cam->src);

	//2.texture
	k = 0;
	if(lit){
		for(j=0;j<4;j++){
			if(0 == lit->src.tex[j].name)continue;
			if(0 == lit->src.tex[j].data)continue;

			glActiveTexture(GL_TEXTURE0 + k);
			glBindTexture(GL_TEXTURE_2D, lit->dst.tex[j]);
			glUniform1i(glGetUniformLocation(dst->shader, lit->src.tex[j].name), k);
			k++;
		}
	}
	for(j=0;j<4;j++)
	{
		if(0 == dst->tex[j])continue;
		if(0 == src->tex[j].name)continue;
//say("tex=%x\n", dst->tex[j]);
		glActiveTexture(GL_TEXTURE0 + k);
		glBindTexture(GL_TEXTURE_2D, dst->tex[j]);
		glUniform1i(glGetUniformLocation(dst->shader, src->tex[j].name), k);
		k++;
	}

	//3.vertex
	fmt = src->vbuf_fmt;
	vbo = dst->vbo;
	vao = dst->vao;
	glBindVertexArray(vao);
//say("%d,%d,%d,%d,%d,%d\n", dst->shader, dst->tex[0], vao, vbo, src->ibuf_h, src->vbuf_h);

//say("8888@method=%x, geom=%x, vbuf_h=%x, ibuf_h=%x\n", src->method, src->geometry, src->vbuf_h, src->ibuf_h);
	if('i' == src->method)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
		if(1 == src->geometry)glDrawElements(GL_POINTS, src->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else if(2 == src->geometry)glDrawElements(GL_LINES, 2*src->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else glDrawElements(GL_TRIANGLES, 3*src->ibuf_h, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		if(1 == src->geometry)glDrawArrays(GL_POINTS, 0, src->vbuf_h);
		else if(2 == src->geometry)glDrawArrays(GL_LINES, 0, src->vbuf_h);
		else glDrawArrays(GL_TRIANGLES, 0, src->vbuf_h);
	}
}
void fullwindow_render(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque, struct supply* wnd, struct fstyle* area)
{
	//say("fullwindow_render:%llx,%llx,%llx,%llx,%llx,%llx\n",cam,lit,solid,opaque,wnd,area);
	int j;
	int x0,y0,ww,hh;
	x0 = area->vc[0] * wnd->fbwidth;
	y0 = area->vc[1] * wnd->fbheight;
	ww = area->vq[0] * wnd->fbwidth;
	hh = area->vq[1] * wnd->fbheight;
	//say("%d,%d,%d,%d\n", x0, y0, ww, hh);
	glViewport(x0, y0, ww, hh);
	glScissor(x0, y0, ww, hh);

	//
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
	glPointSize(4.0*wnd->fbwidth/wnd->width);
	glLineWidth(4.0*wnd->fbwidth/wnd->width);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	//solid
	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		if(0 == solid[j]->src.vbuf)continue;
		render_onedraw(cam[0], lit[0], solid[j]);
	}

	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(j=0;j<64;j++){
		if(0 == opaque[j])continue;
		if(0 == opaque[j]->src.vbuf)continue;
		render_onedraw(cam[0], lit[0], opaque[j]);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}




void fullwindow_write(struct supply* ogl, struct event* ev)
{
	struct halfrel rel[2];
	struct halfrel* stack[16];

	rel[0].pchip = ogl;
	gl41wnd0_write(&rel[0], &rel[1], stack, 0, ev, 0);

	//say("@fullwindow_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);
	//relationwrite(ogl, _wnd_, stack, 0, ev, 0);
}
void fullwindow_read(struct supply* ogl)
{
	struct halfrel rel[2];
	struct halfrel* stack[16];

	rel[0].pchip = ogl;
	gl41wnd0_read(&rel[0], &rel[1], stack, 0, 0, 0);

/*	struct halfrel* stack[16];
	struct relation* rel;
	struct supply* wnd;
	//say("@fullwindow_read\n");

	rel = ogl->orel0;
	while(1){
		if(0 == rel)break;

		wnd = rel->pdstchip;
		//say("%.8s\n", &wnd->type);
		switch(wnd->type){
			case _gl41wnd0_:
				wnd->width = ogl->width;
				wnd->height = ogl->height;
				wnd->fbwidth = ogl->fbwidth;
				wnd->fbheight = ogl->fbheight;
				stack[0+0] = (void*)(rel->src);
				stack[0+1] = (void*)(rel->dst);
				supplyread(stack[1], stack[0], stack, 0+2, 0, 0);
		}

		rel = samesrcnextdst(rel);
	}*/
}
void fullwindow_delete(struct supply* ogl)
{
}
void fullwindow_create(struct supply* ogl)
{
	ogl->fmt = _full_;

	ogl->gl_camera = memorycreate(0x10000, 0);
	ogl->gl_light  = memorycreate(0x10000, 0);
	ogl->gl_solid  = memorycreate(0x10000, 0);
	ogl->gl_opaque = memorycreate(0x10000, 0);
}
