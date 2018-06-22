#include "libuser.h"
//algo
void algorithm_register(void*);
void bintree_register(void*);
void bplus_register(void*);
void fs_register(void*);
void graph_register(void*);
void hex_register(void*);
//hack
void joystick_register(void*);
void keyboard_register(void*);
void rawdump_register(void*);
void switch_register(void*);
//item
void clock_register(void*);
void house_register(void*);
void human_register(void*);
void model_register(void*);
void piano_register(void*);
void picture_register(void*);
void skydome_register(void*);
void terrian_register(void*);
void tree_register(void*);
void water_register(void*);
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
//test
void codeimg_register(void*);
void doodle_register(void*);
void example_register(void*);
void fractal_register(void*);
void palette_register(void*);
void planet_register(void*);
//tool
void browser_register(void*);
void calculator_register(void*);
void camera_register(void*);
void circuit_register(void*);
void font_register(void*);
void qrcode_register(void*);
void sketchpad_register(void*);
void spectrum_register(void*);
void terminal_register(void*);



//
void content_delete()
{
}
void content_create(void* addr)
{
	void* temp = addr + 0x100000;




//---------------------algo----------------------
	algorithm_register(temp);
	temp += sizeof(struct actor);

	bintree_register(temp);
	temp += sizeof(struct actor);

	bplus_register(temp);
	temp += sizeof(struct actor);

	graph_register(temp);
	temp += sizeof(struct actor);




//----------------------game----------------------
	the2048_register(temp);
	temp += sizeof(struct actor);

	chess_register(temp);
	temp += sizeof(struct actor);

	klotski_register(temp);
	temp += sizeof(struct actor);

	maze_register(temp);
	temp += sizeof(struct actor);

	ooxx_register(temp);
	temp += sizeof(struct actor);

	pegged_register(temp);
	temp += sizeof(struct actor);

	poker_register(temp);
	temp += sizeof(struct actor);

	rubikscube_register(temp);
	temp += sizeof(struct actor);

	snake_register(temp);
	temp += sizeof(struct actor);

	sudoku_register(temp);
	temp += sizeof(struct actor);

	tetris_register(temp);
	temp += sizeof(struct actor);

	weiqi_register(temp);
	temp += sizeof(struct actor);

	xiangqi_register(temp);
	temp += sizeof(struct actor);




//--------------------hack-------------------------
	fs_register(temp);
	temp += sizeof(struct actor);

	joystick_register(temp);
	temp += sizeof(struct actor);

	keyboard_register(temp);
	temp += sizeof(struct actor);

	rawdump_register(temp);
	temp += sizeof(struct actor);

	switch_register(temp);
	temp += sizeof(struct actor);




//----------------------item-----------------------
	clock_register(temp);
	temp += sizeof(struct actor);

	house_register(temp);
	temp += sizeof(struct actor);

	human_register(temp);
	temp += sizeof(struct actor);

	model_register(temp);
	temp += sizeof(struct actor);

	piano_register(temp);
	temp += sizeof(struct actor);

	picture_register(temp);
	temp += sizeof(struct actor);

	skydome_register(temp);
	temp += sizeof(struct actor);

	terrian_register(temp);
	temp += sizeof(struct actor);

	tree_register(temp);
	temp += sizeof(struct actor);

	water_register(temp);
	temp += sizeof(struct actor);




//----------------------test----------------------
	codeimg_register(temp);
	temp += sizeof(struct actor);

	doodle_register(temp);
	temp += sizeof(struct actor);

	example_register(temp);
	temp += sizeof(struct actor);

	fractal_register(temp);
	temp += sizeof(struct actor);

	palette_register(temp);
	temp += sizeof(struct actor);

	planet_register(temp);
	temp += sizeof(struct actor);




//------------------------tool--------------------
	browser_register(temp);
	temp += sizeof(struct actor);

	calculator_register(temp);
	temp += sizeof(struct actor);

	camera_register(temp);
	temp += sizeof(struct actor);

	circuit_register(temp);
	temp += sizeof(struct actor);

	font_register(temp);
	temp += sizeof(struct actor);

	hex_register(temp);
	temp += sizeof(struct actor);

	qrcode_register(temp);
	temp += sizeof(struct actor);

	sketchpad_register(temp);
	temp += sizeof(struct actor);

	spectrum_register(temp);
	temp += sizeof(struct actor);

	terminal_register(temp);
	temp += sizeof(struct actor);
}