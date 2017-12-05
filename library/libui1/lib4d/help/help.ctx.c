#include "actor.h"
//game
void the2048_create(u8*,u8*);
void the2048_delete();
void chess_create(u8*,u8*);
void chess_delete();
void ooxx_create(u8*,u8*);
void ooxx_delete();
void rubikscube_create(u8*,u8*);
void rubikscube_delete();
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
void codeimg_create(u8*,u8*);
void codeimg_delete();
void color_create(u8*,u8*);
void color_delete();
void doodle_create(u8*,u8*);
void doodle_delete();
void fractal_create(u8*,u8*);
void fractal_delete();
void pure_create(u8*,u8*);
void pure_delete();
//hack
void algorithm_create(u8*,u8*);
void algorithm_delete();
void browser_create(u8*,u8*);
void browser_delete();
void hex_create(u8*,u8*);
void hex_delete();
void joystick_create(u8*,u8*);
void joystick_delete();
void keyboard_create(u8*,u8*);
void keyboard_delete();
void tree_create(u8*,u8*);
void tree_delete();
//tool
void camera_create(u8*,u8*);
void camera_delete();
void calculator_create(u8*,u8*);
void calculator_delete();
void circuit_create(u8*,u8*);
void circuit_delete();
void font_create(u8*,u8*);
void font_delete();
void qrcode_create(u8*,u8*);
void qrcode_delete();
void sketchpad_create(u8*,u8*);
void sketchpad_delete();
void spectrum_create(u8*,u8*);
void spectrum_delete();
void stl_create(u8*,u8*);
void stl_delete();
void terminal_create(u8*,u8*);
void terminal_delete();
//
void backgroundcolor(void*, u32);
void vt100_read(void*, int, int, int, int);




int content_create(u8* addr)
{
	u8* temp = (void*)(addr+0x100000);

	//game.2048
	the2048_create(addr, temp);
	temp += 0x100;

	//game.chess
	chess_create(addr, temp);
	temp += 0x100;

	//game.ooxx
	ooxx_create(addr, temp);
	temp += 0x100;

	//game.rubikscube
	rubikscube_create(addr, temp);
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

	//hack.browser
	browser_create(addr, temp);
	temp += 0x100;

	//hack.hex
	hex_create(addr, temp);
	temp += 0x100;

	//hack.joystick
	joystick_create(addr, temp);
	temp += 0x100;

	//hack.keyboard
	keyboard_create(addr, temp);
	temp += 0x100;

	//hack.tree
	tree_create(addr, temp);
	temp += 0x100;

	//test.codeimg
	codeimg_create(addr, temp);
	temp += 0x100;

	//test.color
	color_create(addr, temp);
	temp += 0x100;

	//test.fractal
	fractal_create(addr, temp);
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

	//tool.font
	font_create(addr, temp);
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

	//tool.terminal
	terminal_create(addr, temp);
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
	keyboard_delete();
	joystick_delete();
	hex_delete();
	browser_delete();
	algorithm_delete();

	codeimg_delete();
	color_delete();
	doodle_delete();
	font_delete();
	fractal_delete();
	pure_delete();

	the2048_delete();
	chess_delete();
	ooxx_delete();
	rubikscube_delete();
	snake_delete();
	sudoku_delete();
	tetris_delete();
	weiqi_delete();
	xiangqi_delete();
}
