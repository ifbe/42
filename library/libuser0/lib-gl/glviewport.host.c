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

void fixmatrix(float*, void*);
void* allocarena();



void display_eachpass(
	struct gldst* dst, struct glsrc* src,
	struct arena* win, float* cammvp)
{
	int j;
	u32 tmp;
	u32 fmt;
	u32 vbo;
	u32 vao;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	//0.shader
	glUseProgram(dst->shader);

	//1.argument
	glUniformMatrix4fv(glGetUniformLocation(dst->shader, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform3fv(glGetUniformLocation(dst->shader, "camxyz"  ), 1, win->camera.vc);
	//glUniformMatrix4fv(glGetUniformLocation(dst->shader, "sunmvp"), 1, GL_FALSE, cammvp);
	//glUniform3fv(glGetUniformLocation(dst->shader, "sunxyz"  ), 1, win->camera.vc);
	for(j=0;j<4;j++){
		if(0 == src->arg_name[j])continue;
		if(0 == src->arg_data[j])continue;

		tmp = glGetUniformLocation(dst->shader, src->arg_name[j]);
		switch(src->arg_fmt[j]){
			case 'm':{
				glUniformMatrix4fv(tmp, 1, GL_FALSE, src->arg_data[j]);
				break;
			}//mat4

			case 'f':{
				glUniform1fv(tmp, 1, src->arg_data[j]);
				break;
			}
		}//switch
	}//for

	//2.texture
	for(j=0;j<4;j++)
	{
		if(0 == dst->tex[j])continue;
		if(0 == src->tex_name[j])continue;

		glUniform1i(glGetUniformLocation(dst->shader, src->tex_name[j]), j);
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, dst->tex[j]);
	}

	//3.vertex
	fmt = src->vbuf_fmt;
	vbo = dst->vbo;
	vao = dst->vao;
	glBindVertexArray(vao);
//say("%d,%d,%d,%d,%d,%d\n", dst->shader, dst->tex[0], vao, vbo, src->ibuf_h, src->vbuf_h);

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




void hostviewport_render(
	struct arena*  vp, struct style* aa,
	struct arena* win, struct style* st)
{
	int j;
	struct datapair* mod;

	float cammvp[4*4];
	float w,h,x0,y0,x1,y1;

	w = win->fbwidth;
	h = win->fbheight;
	x0 = w * st->vc[0];	//0
	y0 = h * st->vc[1];	//0
	x1 = w * st->vq[0];	//0.5
	y1 = h * st->vq[1];	//1

	//
	vp->width = vp->fbwidth = x1;
	vp->height = vp->fbheight = y1;
	glViewport(x0, y0, x1, y1);

	//
	struct relation* rel;
	struct arena* ctxnode;
	rel = vp->orel0;
	while(1){
		if(0 == rel)break;
		if(_win_ == rel->dsttype){
			ctxnode = (void*)(rel->dstchip);
			if(ctxnode)goto found;
			break;
		}
		rel = samesrcnextdst(rel);
	}
	return;

found:
	fixmatrix(cammvp, vp);
	mat4_transpose((void*)cammvp);


	//solid
	mod = ctxnode->gl_solid;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(&mod[j].dst, &mod[j].src, vp, cammvp);
	}


	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mod = ctxnode->gl_opaque;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(&mod[j].dst, &mod[j].src, vp, cammvp);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}




void* hostviewport_create(struct arena* window)
{
	struct arena* viewport;

	viewport = allocarena();
	if(0 == viewport)return 0;

	viewport->type = viewport->fmt = hex32('v','p',0,0);

	return viewport;
}