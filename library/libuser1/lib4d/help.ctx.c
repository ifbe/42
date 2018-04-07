#include "actor.h"
//test
void clock_register(void*);
void codeimg_register(void*);
void doodle_register(void*);
void example_register(void*);
void fractal_register(void*);
void palette_register(void*);
void planet_register(void*);
//game
void the2048_register(void*);
void chess_register(void*);
void klotski_register(void*);
void maze_register(void*);
void ooxx_register(void*);
void pegged_register(void*);
void poker_register(void*);
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
void piano_register(void*);
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
void rawdump_register(void*);
void switch_register(void*);



//
void content_delete()
{
}
void content_create(void* addr)
{
	void* temp = addr + 0x100000;

	//test.clock
	clock_register(temp);
	temp += sizeof(struct actor);

	//test.codeimg
	codeimg_register(temp);
	temp += sizeof(struct actor);

	//test.doodle
	doodle_register(temp);
	temp += sizeof(struct actor);

	//test.example
	example_register(temp);
	temp += sizeof(struct actor);

	//test.fractal
	fractal_register(temp);
	temp += sizeof(struct actor);

	//test.palette
	palette_register(temp);
	temp += sizeof(struct actor);

	//test.planet
	planet_register(temp);
	temp += sizeof(struct actor);

	//game.2048
	the2048_register(temp);
	temp += sizeof(struct actor);

	//game.chess
	chess_register(temp);
	temp += sizeof(struct actor);

	//game.klotski
	klotski_register(temp);
	temp += sizeof(struct actor);

	//game.maze
	maze_register(temp);
	temp += sizeof(struct actor);

	//game.ooxx
	ooxx_register(temp);
	temp += sizeof(struct actor);

	//game.pegged
	pegged_register(temp);
	temp += sizeof(struct actor);

	//game.poker
	poker_register(temp);
	temp += sizeof(struct actor);

	//game.rubik
	rubikscube_register(temp);
	temp += sizeof(struct actor);

	//game.snake
	snake_register(temp);
	temp += sizeof(struct actor);

	//game.sudoku
	sudoku_register(temp);
	temp += sizeof(struct actor);

	//game.tetris
	tetris_register(temp);
	temp += sizeof(struct actor);

	//game.weiqi
	weiqi_register(temp);
	temp += sizeof(struct actor);

	//game.xiangqi
	xiangqi_register(temp);
	temp += sizeof(struct actor);

	//tool.browser
	browser_register(temp);
	temp += sizeof(struct actor);

	//tool.calculator
	calculator_register(temp);
	temp += sizeof(struct actor);

	//tool.camera
	camera_register(temp);
	temp += sizeof(struct actor);

	//tool.circuit
	circuit_register(temp);
	temp += sizeof(struct actor);

	//tool.font
	font_register(temp);
	temp += sizeof(struct actor);

	//tool.piano
	piano_register(temp);
	temp += sizeof(struct actor);

	//tool.qrcode
	qrcode_register(temp);
	temp += sizeof(struct actor);

	//tool.sketchpad
	sketchpad_register(temp);
	temp += sizeof(struct actor);

	//tool.stl
	stl_register(temp);
	temp += sizeof(struct actor);

	//tool.spectrum
	spectrum_register(temp);
	temp += sizeof(struct actor);

	//tool.terminal
	terminal_register(temp);
	temp += sizeof(struct actor);

	//hack.algorithm
	algorithm_register(temp);
	temp += sizeof(struct actor);

	//hack.bintree
	bintree_register(temp);
	temp += sizeof(struct actor);

	//hack.bplus
	bplus_register(temp);
	temp += sizeof(struct actor);

	//hack.fs
	fs_register(temp);
	temp += sizeof(struct actor);

	//hack.graph
	graph_register(temp);
	temp += sizeof(struct actor);

	//hack.hex
	hex_register(temp);
	temp += sizeof(struct actor);

	//hack.input
	input_register(temp);
	temp += sizeof(struct actor);

	//hack.rawdump
	rawdump_register(temp);
	temp += sizeof(struct actor);

	//hack.switch
	switch_register(temp);
	temp += sizeof(struct actor);
}