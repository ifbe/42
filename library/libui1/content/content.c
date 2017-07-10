#include "actor.h"
//bg
void bgcolor_create(u8*,u8*);
void bgcolor_delete();
void bgpicture_create(u8*,u8*);
void bgpicture_delete();
//game
void the2048_create(u8*,u8*);
void the2048_delete();
void chess_create(u8*,u8*);
void chess_delete();
void ooxx_create(u8*,u8*);
void ooxx_delete();
void snake_create(u8*,u8*);
void snake_delete();
void sudoku_create(u8*,u8*);
void sudoku_delete();
void tetris_create(u8*,u8*);
void tetris_delete();
void weiqi_create(u8*,u8*);
void weiqi_delete();
void xiangqi_create(u8*,u8*);
void xiangqi_delete();
//test
void color_create(u8*,u8*);
void color_delete();
void doodle_create(u8*,u8*);
void doodle_delete();
void font_create(u8*,u8*);
void font_delete();
void pure_create(u8*,u8*);
void pure_delete();
//hack
void algorithm_create(u8*,u8*);
void algorithm_delete();
void browse_create(u8*,u8*);
void browse_delete();
void control_create(u8*,u8*);
void control_delete();
void hex_create(u8*,u8*);
void hex_delete();
void tree_create(u8*,u8*);
void tree_delete();
//tool
void camera_create(u8*,u8*);
void camera_delete();
void calculator_create(u8*,u8*);
void calculator_delete();
void circuit_create(u8*,u8*);
void circuit_delete();
void sketchpad_create(u8*,u8*);
void sketchpad_delete();
void spectrum_create(u8*,u8*);
void spectrum_delete();
void stl_create(u8*,u8*);
void stl_delete();
void qrcode_create(u8*,u8*);
void qrcode_delete();
//
void vt100_read(void*, int, int, int, int);




int content_create(u8* addr)
{
	u8* temp = (void*)(addr+0x100000);

	//
	bgcolor_create(addr, temp);
	temp += 0x100;

	bgpicture_create(addr, temp);
	temp += 0x100;

	//game.2048
	the2048_create(addr, temp);
	temp += 0x100;

	//game.chess
	chess_create(addr, temp);
	temp += 0x100;

	//game.ooxx
	ooxx_create(addr, temp);
	temp += 0x100;

	//game.snake
	snake_create(addr, temp);
	temp += 0x100;

	//game.sudoku
	sudoku_create(addr, temp);
	temp += 0x100;

	//game.tetris
	tetris_create(addr, temp);
	temp += 0x100;

	//game.weiqi
	weiqi_create(addr, temp);
	temp += 0x100;

	//game.xiangqi
	xiangqi_create(addr, temp);
	temp += 0x100;

	//hack.algorithm
	algorithm_create(addr, temp);
	temp += 0x100;

	//hack.browse
	browse_create(addr, temp);
	temp += 0x100;

	//hack.hex
	hex_create(addr, temp);
	temp += 0x100;

	//hack.input
	control_create(addr, temp);
	temp += 0x100;

	//hack.tree
	tree_create(addr, temp);
	temp += 0x100;

	//test.color
	color_create(addr, temp);
	temp += 0x100;

	//test.font
	font_create(addr, temp);
	temp += 0x100;

	//test.doodle
	doodle_create(addr, temp);
	temp += 0x100;

	//test.pure
	pure_create(addr, temp);
	temp += 0x100;

	//tool.calculator
	calculator_create(addr, temp);
	temp += 0x100;

	//tool.camera
	camera_create(addr, temp);
	temp += 0x100;

	//tool.circuit
	circuit_create(addr, temp);
	temp += 0x100;

	//tool.qrcode
	qrcode_create(addr, temp);
	temp += 0x100;

	//tool.sketchpad
	sketchpad_create(addr, temp);
	temp += 0x100;

	//tool.stl
	stl_create(addr, temp);
	temp += 0x100;

	//tool.spectrum
	spectrum_create(addr, temp);
	temp += 0x100;

	//
	return temp-addr-0x100000;
}
void content_delete()
{
	spectrum_delete();
	stl_delete();
	sketchpad_delete();
	qrcode_delete();
	circuit_delete();
	calculator_delete();
	camera_delete();

	tree_delete();
	hex_delete();
	control_delete();
	browse_delete();
	algorithm_delete();

	color_delete();
	doodle_delete();
	font_delete();
	pure_delete();

	the2048_delete();
	chess_delete();
	ooxx_delete();
	snake_delete();
	sudoku_delete();
	tetris_delete();
	weiqi_delete();
	xiangqi_delete();

	bgpicture_delete();
	bgcolor_delete();
}




int content_read(struct arena* win)
{
	int j;
	struct actor* act;
	struct relation* rel;

	//cli
	if(win->fmt == 0x696c63)return 0;

	//voice
	if(win->fmt == 0x6563696f76)return 0;

	//1d:	cli
	if(win->dim == 1)
	{
		vt100_read(win, 0, 0, 0xffff, 0xffff);
		return 0;
	}

	//2d:	rgba
	else if(win->dim == 2)
	{
		rel = win->bot;
		for(j=0;j<16;j++)
		{
			if(rel == 0)break;

			act = rel->child_this;
			act->read(rel);

			rel = rel->above;
		}
	}

	//3d:	directx, opengl, vulkan
	else if(win->dim == 3)
	{
	}//dim=3

	//
	return 0;
}
int content_write(struct event* ev)
{
}