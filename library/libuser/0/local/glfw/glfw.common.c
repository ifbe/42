#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "libuser.h"
void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);
void* allocifoot();




void initobject(struct arena* w)  
{
	struct texandobj* mod = w->mod;

//---------------------0--------------------------
	//vao
	glGenVertexArrays(1, &mod[0].vao);
	glBindVertexArray(mod[0].vao);

	//ibo
	glGenBuffers(1, &mod[0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[0].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------1--------------------------
	//vao
	glGenVertexArrays(1, &mod[1].vao);
	glBindVertexArray(mod[1].vao);

	//ibo
	glGenBuffers(1, &mod[1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[1].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[1].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------2--------------------------
	//vao
	glGenVertexArrays(1, &mod[2].vao);
	glBindVertexArray(mod[2].vao);

	//ibo
	glGenBuffers(1, &mod[2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[2].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[2].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------3--------------------------
	//vao
	glGenVertexArrays(1, &mod[3].vao);
	glBindVertexArray(mod[3].vao);

	//ibo
	glGenBuffers(1, &mod[3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[3].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[3].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//---------------------4--------------------------
	//vao
	glGenVertexArrays(1, &mod[4].vao);
	glBindVertexArray(mod[4].vao);

	//ibo
	glGenBuffers(1, &mod[4].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[4].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[4].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[4].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------5--------------------------
	//vao
	glGenVertexArrays(1, &mod[5].vao);
	glBindVertexArray(mod[5].vao);

	//ibo
	glGenBuffers(1, &mod[5].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[5].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[5].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[5].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------6--------------------------
	//vao
	glGenVertexArrays(1, &mod[6].vao);
	glBindVertexArray(mod[6].vao);

	//ibo
	glGenBuffers(1, &mod[6].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[6].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[6].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[6].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------7--------------------------
	//vao
	glGenVertexArrays(1, &mod[7].vao);
	glBindVertexArray(mod[7].vao);

	//ibo
	glGenBuffers(1, &mod[7].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[7].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[7].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[7].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);

/*
//-------------------------2d screen--------------------------
	//vao
	glGenVertexArrays(1, &mod[4].vao);
	glBindVertexArray(mod[4].vao);

	//vbo
	glGenBuffers(1, &mod[4].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[4].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);
*/

//-------------------------0x80--------------------------
//drawarray.point(vertex,colour)
	glGenVertexArrays(1, &mod[0x80].vao);
	glBindVertexArray(mod[0x80].vao);

	glGenBuffers(1, &mod[0x80].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x80].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x81--------------------------
	//drawelement.line(vertex,colour)
	glGenVertexArrays(1, &mod[0x81].vao);
	glBindVertexArray(mod[0x81].vao);

	glGenBuffers(1, &mod[0x81].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x81].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x81].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x81].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x82---------------------------
	//drawarray.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x82].vao);
	glBindVertexArray(mod[0x82].vao);

	glGenBuffers(1, &mod[0x82].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000,
		mod[0x82].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------0x83---------------------------
	//drawelement.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x83].vao);
	glBindVertexArray(mod[0x83].vao);

	glGenBuffers(1, &mod[0x83].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x83].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x83].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000,
		mod[0x83].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//------------------------0x84--------------------------
	glGenVertexArrays(1, &mod[0x84].vao);
	glBindVertexArray(mod[0x84].vao);

	glGenBuffers(1, &mod[0x84].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x84].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x85-------------------------
	glGenVertexArrays(1, &mod[0x85].vao);
	glBindVertexArray(mod[0x85].vao);

	glGenBuffers(1, &mod[0x85].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x85].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x85].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x85].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x86-------------------------
	glGenVertexArrays(1, &mod[0x86].vao);
	glBindVertexArray(mod[0x86].vao);

	glGenBuffers(1, &mod[0x86].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x86].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x86].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x86].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);
}
void inittexture(struct arena* w)  
{
	int j;
	u8* buf = w->buf;
	struct texandobj* mod = w->mod;


//---------------------[0000,3fff]------------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0x80;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048, 
			(j&0x7f)<<4, (j&0xff80)>>3, j
		);
	}
	for(j=0x20;j<0x80;j++)
	{
		drawascii_alpha(buf, 2048, 2048,
			j<<4, 0, j
		);
	}

	glGenTextures(1, &(mod[0].tex));
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);


//---------------------[4000,7fff]----------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x4000
		);
	}

	glGenTextures(1, &(mod[1].tex));
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);


//------------------[8000,bfff]---------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x8000
		);
	}

	glGenTextures(1, &(mod[2].tex));
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);


//----------------[c000,ffff]--------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0xc000
		);
	}

	glGenTextures(1, &(mod[3].tex));
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);

/*
//------------------2d screen--------------------
	glGenTextures(1, &(mod[4].tex));
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, buf
	);
*/
	/*
	glGenFramebuffers(1, &shadowfb);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowfb);

	glGenTextures(1, &shadowtexture);
	glBindTexture(GL_TEXTURE_2D, shadowtexture);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_DEPTH_COMPONENT, 1024, 1024, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		shadowtexture, 0
	);
	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("error@fbo\n");
	}
*/
}
void initmodbuf(struct arena* w)
{
	struct texandobj* mod;
	w->mod = malloc(0x10000);
	w->buf = malloc(0x400000);
	mod = w->mod;

//--------------------font3d-------------------
	//[0000,3fff]
	mod[0].vbuf = malloc(0x200000);
	mod[0].vlen = 0;
	mod[0].ibuf = malloc(0x100000);
	mod[0].ilen = 0;

	//[4000,7fff]
	mod[1].vbuf = malloc(0x200000);
	mod[1].vlen = 0;
	mod[1].ibuf = malloc(0x100000);
	mod[1].ilen = 0;

	//[8000,bfff]
	mod[2].vbuf = malloc(0x200000);
	mod[2].vlen = 0;
	mod[2].ibuf = malloc(0x100000);
	mod[2].ilen = 0;

	//[c000,ffff]
	mod[3].vbuf = malloc(0x200000);
	mod[3].vlen = 0;
	mod[3].ibuf = malloc(0x100000);
	mod[3].ilen = 0;

//--------------------font2d-------------------
	//[0000,3fff]
	mod[4].vbuf = malloc(0x200000);
	mod[4].vlen = 0;
	mod[4].ibuf = malloc(0x100000);
	mod[4].ilen = 0;

	//[4000,7fff]
	mod[5].vbuf = malloc(0x200000);
	mod[5].vlen = 0;
	mod[5].ibuf = malloc(0x100000);
	mod[5].ilen = 0;

	//[8000,bfff]
	mod[6].vbuf = malloc(0x200000);
	mod[6].vlen = 0;
	mod[6].ibuf = malloc(0x100000);
	mod[6].ilen = 0;

	//[c000,ffff]
	mod[7].vbuf = malloc(0x200000);
	mod[7].vlen = 0;
	mod[7].ibuf = malloc(0x100000);
	mod[7].ilen = 0;
/*
	//2d screen
	mod[4].vbuf = malloc(0x100000);
	mod[4].vlen = 0;
*/
//--------------------3d-------------------
	//drawarray.point
	mod[0x80].vbuf = malloc(0x100000);
	mod[0x80].vlen = 0;

	//drawelement.line
	mod[0x81].ibuf = malloc(0x100000);
	mod[0x81].ilen = 0;
	mod[0x81].vbuf = malloc(0x100000);
	mod[0x81].vlen = 0;

	//drawarray.trigon
	mod[0x82].vbuf = malloc(0x1000000);
	mod[0x82].vlen = 0;

	//drawelement.trigon
	mod[0x83].ibuf = malloc(0x100000);
	mod[0x83].ilen = 0;
	mod[0x83].vbuf = malloc(0x1000000);
	mod[0x83].vlen = 0;

//----------------------2d--------------------
	//drawarray.point
	mod[0x84].vbuf = malloc(0x100000);
	mod[0x84].vlen = 0;

	//drawelement.line
	mod[0x85].ibuf = malloc(0x100000);
	mod[0x85].ilen = 0;
	mod[0x85].vbuf = malloc(0x100000);
	mod[0x85].vlen = 0;

	//drawelement.trigon
	mod[0x86].ibuf = malloc(0x100000);
	mod[0x86].ilen = 0;
	mod[0x86].vbuf = malloc(0x100000);
	mod[0x86].vlen = 0;
}




void callback_update_eachpass(struct ifoot* fi, struct ofoot* fo)
{
	//say("%llx,%llx\n", fi, fo);

	//shader
	if(fi->shader_deq != fo->shader_enq)
	{
		say("vs=%llx,fs=%llx\n", fo->vs, fo->fs);
		fi->shader_deq = fo->shader_enq;
	}

	//argument
	if(fi->arg_deq[0] != fo->arg_enq[0])
	{
		say("arg=%x\n", fo->arg[0]);
		fi->arg_deq[0] = fo->arg_enq[0];
	}

	//texture
	if(fi->tex_deq[0] != fo->tex_enq[0])
	{
		say("tex=%llx\n", fo->tex[0]);
		fi->tex_deq[0] = fo->tex_enq[0];
	}

	//vertex
	if(fi->vbo_deq != fo->vbuf_enq)
	{
		say("ibuf=%llx,vbuf=%llx\n", fo->ibuf, fo->vbuf);
		fi->vbo_deq = fo->vbuf_enq;
	}

	//index
	if(fi->ibo_deq != fo->ibuf_enq)
	{
		say("ibuf=%llx,vbuf=%llx\n", fo->ibuf, fo->vbuf);
		fi->ibo_deq = fo->ibuf_enq;
	}
}
void callback_update_eachactor(struct arena* w)
{
	int j;
	u64* pi;
	u64* po;
	struct relation* rel;

	w->fmt = hex32('v','b','o',0);
	actorread_all(w);

	rel = w->irel0;
	while(1)
	{
		if(0 == rel)break;

		pi = (void*)(rel->dstfoot) + 0x80;
		po = (void*)(rel->srcfoot) + 0x80;
		for(j=0;j<16;j++)
		{
			if(0 == po[j])break;
			if(0 == pi[j])pi[j] = (u64)allocifoot();
			callback_update_eachpass((void*)pi[j], (void*)po[j]);
		}

		rel = samedstnextsrc(rel);
	}
}
void callback_update(struct arena* w)
{
/*
//--------------------------------
	w->fmt = hex64('r','g','b','a','8','8','8','8');
	w->height = 1024;
	w->width = w->stride = 1024;
	actorread_all(w);

	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, w->buf
	);
*/
//--------------------------------
	struct texandobj* mod = w->mod;
	callback_update_eachactor(w);

//----------------------font3d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0].ilen, mod[0].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0].vlen,mod[0].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[1].ilen, mod[1].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[1].vlen, mod[1].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[2].ilen,mod[2].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[2].vlen,mod[2].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[3].ilen, mod[3].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[3].vlen, mod[3].vbuf);


//----------------------font2d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[4].ilen, mod[4].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[4].vlen,mod[4].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[5].ilen, mod[5].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[5].vlen, mod[5].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[6].ilen,mod[6].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[6].vlen,mod[6].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[7].ilen, mod[7].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[7].vlen, mod[7].vbuf);
/*
	//2d screen
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[4].vlen, mod[4].vbuf);
*/

//----------------------3d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x80].vlen, mod[0x80].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x81].ilen, mod[0x81].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x81].vlen, mod[0x81].vbuf);

	//drawarray: trigon
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0x82].vlen, mod[0x82].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x83].ilen, mod[0x83].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 36*mod[0x83].vlen, mod[0x83].vbuf);


//----------------------2d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x84].vlen, mod[0x84].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x85].ilen, mod[0x85].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x85].vlen, mod[0x85].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x86].ilen, mod[0x86].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 24*mod[0x86].vlen, mod[0x86].vbuf);
}