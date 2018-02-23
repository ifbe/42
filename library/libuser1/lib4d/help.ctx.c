#include "actor.h"
//test
void clock_register(void*);
void codeimg_register(void*);
void doodle_register(void*);
void example_register(void*);
void fractal_register(void*);
void palette_register(void*);
//game
void the2048_register(void*);
void chess_register(void*);
void klotski_register(void*);
void maze_register(void*);
void ooxx_register(void*);
void pegged_register(void*);
void rubikscube_register(void*);
void snake_register(void*);
void sudoku_register(void*);
void tetris_register(void*);
void weiqi_register(void*);
void xiangqi_register(void*);
//tool
void browser_register(void*);
void calculator_register(void*);
void camera_register(void*);
void circuit_register(void*);
void font_register(void*);
void qrcode_register(void*);
void sketchpad_register(void*);
void spectrum_register(void*);
void stl_register(void*);
void terminal_register(void*);
//hack
void algorithm_register(void*);
void bintree_register(void*);
void bplus_register(void*);
void fs_register(void*);
void graph_register(void*);
void hex_register(void*);
void input_register(void*);




int content_create(u8* addr)
{
	u8* temp = (void*)(addr+0x100000);

	//test.codeimg
	codeimg_register(temp);
	temp += 0x100;

	//test.clock
	clock_register(temp);
	temp += 0x100;

	//test.doodle
	doodle_register(temp);
	temp += 0x100;

	//test.example
	example_register(temp);
	temp += 0x100;

	//test.fractal
	fractal_register(temp);
	temp += 0x100;

	//test.palette
	palette_register(temp);
	temp += 0x100;

	//game.2048
	the2048_register(temp);
	temp += 0x100;

	//game.chess
	chess_register(temp);
	temp += 0x100;

	//game.klotski
	klotski_register(temp);
	temp += 0x100;

	//game.maze
	maze_register(temp);
	temp += 0x100;

	//game.ooxx
	ooxx_register(temp);
	temp += 0x100;

	//game.pegged
	pegged_register(temp);
	temp += 0x100;

	//game.rubik
	rubikscube_register(temp);
	temp += 0x100;

	//game.snake
	snake_register(temp);
	temp += 0x100;

	//game.sudoku
	sudoku_register(temp);
	temp += 0x100;

	//game.tetris
	tetris_register(temp);
	temp += 0x100;

	//game.weiqi
	weiqi_register(temp);
	temp += 0x100;

	//game.xiangqi
	xiangqi_register(temp);
	temp += 0x100;

	//tool.browser
	browser_register(temp);
	temp += 0x100;

	//tool.calculator
	calculator_register(temp);
	temp += 0x100;

	//tool.camera
	camera_register(temp);
	temp += 0x100;

	//tool.circuit
	circuit_register(temp);
	temp += 0x100;

	//tool.font
	font_register(temp);
	temp += 0x100;

	//tool.qrcode
	qrcode_register(temp);
	temp += 0x100;

	//tool.sketchpad
	sketchpad_register(temp);
	temp += 0x100;

	//tool.stl
	stl_register(temp);
	temp += 0x100;

	//tool.spectrum
	spectrum_register(temp);
	temp += 0x100;

	//tool.terminal
	terminal_register(temp);
	temp += 0x100;

	//hack.algorithm
	algorithm_register(temp);
	temp += 0x100;

	//hack.bintree
	bintree_register(temp);
	temp += 0x100;

	//hack.bplus
	bplus_register(temp);
	temp += 0x100;

	//hack.fs
	fs_register(temp);
	temp += 0x100;

	//hack.graph
	graph_register(temp);
	temp += 0x100;

	//hack.hex
	hex_register(temp);
	temp += 0x100;

	//hack.input
	input_register(temp);
	temp += 0x100;

	//
	return temp-addr-0x100000;
}
void content_delete()
{
}
