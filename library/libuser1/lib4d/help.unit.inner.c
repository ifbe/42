#include "libuser.h"
//chip
void vsrc_register(void*);
void resistor_register(void*);
void finfet_register(void*);
void hbridge_register(void*);
void rectify_register(void*);

//dxgl
//.cam
void orthcam_register(void*);
void firstperson_register(void*);
void thirdperson_register(void*);
void rtscam_register(void*);
void surround_register(void*);
void vrglass_register(void*);
//.fbo
void glass_register(void*);
void mirror_register(void*);
void portal_register(void*);
void water_register(void*);
//.light
void dirlight_register(void*);
void spotlight_register(void*);
//.aid
void aidfont_register(void*);
void aidgeom_register(void*);
//.else
void ground_register(void*);
void particle_register(void*);
void skydome_register(void*);
void terrain_register(void*);
void texball_register(void*);
void weather_register(void*);

//algo
void algorithm_register(void*);
void bintree_register(void*);
void bplus_register(void*);
void graph_register(void*);

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

//hack
void browser_register(void*);
void circuit_register(void*);
void fs_register(void*);
void pwmtool_register(void*);
void rawdump_register(void*);
void switch_register(void*);

//item
void clock_register(void*);
void control_register(void*);
void dna_register(void*);
void drone_register(void*);
void geometry_register(void*);
void house_register(void*);
void human_register(void*);
void mobius_register(void*);
void model_register(void*);
void motor_register(void*);
void piano_register(void*);
void picture_register(void*);
void rccar_register(void*);
void stair_register(void*);
void tardis_register(void*);
void tree_register(void*);

//test
void codeimg_register(void*);
void doodle_register(void*);
void example_register(void*);
void fractal_register(void*);
void palette_register(void*);
void planet_register(void*);

//tool
void calculator_register(void*);
void font_register(void*);
void hex_register(void*);
void qrcode_register(void*);
void sketchpad_register(void*);
void spectrum_register(void*);
void terminal_register(void*);
void video_register(void*);

//menu
//.2d
void corner_register(void*);
void pointer_register(void*);
//.3d
void camman_register(void*);
void picker_register(void*);
//.body
void tabbar_register(void*);
//.vkbd
void vjoy_register(void*);
void vkbd_register(void*);
//.else
void login_register(void*);
void overview_register(void*);
void detail_register(void*);



//
void content_delete()
{
}
void content_create(void* addr)
{
	void* tmp = addr + 0x100000;




//-------------------chip-----------------
	vsrc_register(tmp);
	tmp += sizeof(struct actor);

	resistor_register(tmp);
	tmp += sizeof(struct actor);

	finfet_register(tmp);
	tmp += sizeof(struct actor);

	hbridge_register(tmp);
	tmp += sizeof(struct actor);

	rectify_register(tmp);
	tmp += sizeof(struct actor);




//----------------------dxgl-----------------------
//.cam
	orthcam_register(tmp);
	tmp += sizeof(struct actor);

	firstperson_register(tmp);
	tmp += sizeof(struct actor);

	thirdperson_register(tmp);
	tmp += sizeof(struct actor);

	rtscam_register(tmp);
	tmp += sizeof(struct actor);

	surround_register(tmp);
	tmp += sizeof(struct actor);

	vrglass_register(tmp);
	tmp += sizeof(struct actor);

//.fbo
	glass_register(tmp);
	tmp += sizeof(struct actor);

	mirror_register(tmp);
	tmp += sizeof(struct actor);

	portal_register(tmp);
	tmp += sizeof(struct actor);

	water_register(tmp);
	tmp += sizeof(struct actor);

//.light
	dirlight_register(tmp);
	tmp += sizeof(struct actor);

	spotlight_register(tmp);
	tmp += sizeof(struct actor);

//.aid
	aidfont_register(tmp);
	tmp += sizeof(struct actor);

	aidgeom_register(tmp);
	tmp += sizeof(struct actor);

//.else
	ground_register(tmp);
	tmp += sizeof(struct actor);

	particle_register(tmp);
	tmp += sizeof(struct actor);

	skydome_register(tmp);
	tmp += sizeof(struct actor);

	terrain_register(tmp);
	tmp += sizeof(struct actor);

	texball_register(tmp);
	tmp += sizeof(struct actor);

	weather_register(tmp);
	tmp += sizeof(struct actor);




//---------------------algo----------------------
	algorithm_register(tmp);
	tmp += sizeof(struct actor);

	bintree_register(tmp);
	tmp += sizeof(struct actor);

	bplus_register(tmp);
	tmp += sizeof(struct actor);

	graph_register(tmp);
	tmp += sizeof(struct actor);




//----------------------game----------------------
	the2048_register(tmp);
	tmp += sizeof(struct actor);

	chess_register(tmp);
	tmp += sizeof(struct actor);

	klotski_register(tmp);
	tmp += sizeof(struct actor);

	maze_register(tmp);
	tmp += sizeof(struct actor);

	ooxx_register(tmp);
	tmp += sizeof(struct actor);

	pegged_register(tmp);
	tmp += sizeof(struct actor);

	poker_register(tmp);
	tmp += sizeof(struct actor);

	rubikscube_register(tmp);
	tmp += sizeof(struct actor);

	snake_register(tmp);
	tmp += sizeof(struct actor);

	sudoku_register(tmp);
	tmp += sizeof(struct actor);

	tetris_register(tmp);
	tmp += sizeof(struct actor);

	weiqi_register(tmp);
	tmp += sizeof(struct actor);

	xiangqi_register(tmp);
	tmp += sizeof(struct actor);




//--------------------hack-------------------------
	browser_register(tmp);
	tmp += sizeof(struct actor);

	circuit_register(tmp);
	tmp += sizeof(struct actor);

	fs_register(tmp);
	tmp += sizeof(struct actor);

	pwmtool_register(tmp);
	tmp += sizeof(struct actor);

	rawdump_register(tmp);
	tmp += sizeof(struct actor);

	switch_register(tmp);
	tmp += sizeof(struct actor);




//----------------------item-----------------------
	clock_register(tmp);
	tmp += sizeof(struct actor);

	control_register(tmp);
	tmp += sizeof(struct actor);

	dna_register(tmp);
	tmp += sizeof(struct actor);

	drone_register(tmp);
	tmp += sizeof(struct actor);

	geometry_register(tmp);
	tmp += sizeof(struct actor);

	house_register(tmp);
	tmp += sizeof(struct actor);

	human_register(tmp);
	tmp += sizeof(struct actor);

	mobius_register(tmp);
	tmp += sizeof(struct actor);

	model_register(tmp);
	tmp += sizeof(struct actor);

	motor_register(tmp);
	tmp += sizeof(struct actor);

	piano_register(tmp);
	tmp += sizeof(struct actor);

	picture_register(tmp);
	tmp += sizeof(struct actor);

	rccar_register(tmp);
	tmp += sizeof(struct actor);

	stair_register(tmp);
	tmp += sizeof(struct actor);

	tardis_register(tmp);
	tmp += sizeof(struct actor);

	tree_register(tmp);
	tmp += sizeof(struct actor);




//----------------------test----------------------
	codeimg_register(tmp);
	tmp += sizeof(struct actor);

	doodle_register(tmp);
	tmp += sizeof(struct actor);

	example_register(tmp);
	tmp += sizeof(struct actor);

	fractal_register(tmp);
	tmp += sizeof(struct actor);

	palette_register(tmp);
	tmp += sizeof(struct actor);

	planet_register(tmp);
	tmp += sizeof(struct actor);




//------------------------tool--------------------
	calculator_register(tmp);
	tmp += sizeof(struct actor);

	font_register(tmp);
	tmp += sizeof(struct actor);

	hex_register(tmp);
	tmp += sizeof(struct actor);

	qrcode_register(tmp);
	tmp += sizeof(struct actor);

	sketchpad_register(tmp);
	tmp += sizeof(struct actor);

	spectrum_register(tmp);
	tmp += sizeof(struct actor);

	terminal_register(tmp);
	tmp += sizeof(struct actor);

	video_register(tmp);
	tmp += sizeof(struct actor);




//-------------------menu------------------
//.2d
	corner_register(tmp);
	tmp += sizeof(struct actor);

	pointer_register(tmp);
	tmp += sizeof(struct actor);

//.3d
	camman_register(tmp);
	tmp += sizeof(struct actor);

	picker_register(tmp);
	tmp += sizeof(struct actor);

//.body
	tabbar_register(tmp);
	tmp += sizeof(struct actor);

//.vkbd
	vjoy_register(tmp);
	tmp += sizeof(struct actor);

	vkbd_register(tmp);
	tmp += sizeof(struct actor);

//.else
	login_register(tmp);
	tmp += sizeof(struct actor);

	overview_register(tmp);
	tmp += sizeof(struct actor);

	detail_register(tmp);
	tmp += sizeof(struct actor);
}
