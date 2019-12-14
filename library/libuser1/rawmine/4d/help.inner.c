#include "libuser.h"
//chip.baseunit
void finfet_register(void*);
void resistor_register(void*);
void vsrc_register(void*);
//chip.compound
void nand_register(void*);
void nor_register(void*);
void hbridge_register(void*);
void rectify_register(void*);

//slal
void monomic_register(void*);
void stereo_register(void*);

//dxgl
//.cam
void orthcam_register(void*);
void firstperson_register(void*);
void thirdperson_register(void*);
void rtscam_register(void*);
void freecam_register(void*);
void vrglass_register(void*);
//.fbo
void glass_register(void*);
void mirror_register(void*);
void portal_register(void*);
void water_register(void*);
//.light
void dirlight_register(void*);
void pointlight_register(void*);
void projector_register(void*);
void spotlight_register(void*);
//.else
void curtain_register(void*);
void field_register(void*);
void ground_register(void*);
void particle_register(void*);
void skydome_register(void*);
void terrain_register(void*);
void texball_register(void*);
void texbox_register(void*);
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
void geometry_register(void*);
void house_register(void*);
void mario_register(void*);
void mobius_register(void*);
void model_register(void*);
void motor_register(void*);
void piano_register(void*);
void picture_register(void*);
void stair_register(void*);
void tardis_register(void*);
void texmix_register(void*);
void tree_register(void*);

//real
void theear_register(void*);
void theeye_register(void*);
void screen_register(void*);
void speaker_register(void*);

//robo
void dancemat_register(void*);
void drone_register(void*);
void human_register(void*);
void otto_register(void*);
void rccar_register(void*);
void robodog_register(void*);

//test
void calib3d_register(void*);
void codeimg_register(void*);
void doodle_register(void*);
void example_register(void*);
void fractal_register(void*);
void oscillo_register(void*);
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
void voxel_register(void*);
void button_register(void*);

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
void skillbar_register(void*);



//
void content_delete()
{
}
void content_create(void* addr)
{
	void* tmp = addr;




//-------------------chip.baseunit-----------------
	vsrc_register(tmp);
	tmp += sizeof(struct entity);

	resistor_register(tmp);
	tmp += sizeof(struct entity);

	finfet_register(tmp);
	tmp += sizeof(struct entity);




//-------------------chip.baseunit-----------------
	nand_register(tmp);
	tmp += sizeof(struct entity);

	nor_register(tmp);
	tmp += sizeof(struct entity);

	hbridge_register(tmp);
	tmp += sizeof(struct entity);

	rectify_register(tmp);
	tmp += sizeof(struct entity);




//----------------------slal-----------------------
	monomic_register(tmp);
	tmp += sizeof(struct entity);

	stereo_register(tmp);
	tmp += sizeof(struct entity);




//----------------------dxgl-----------------------
//.cam
	orthcam_register(tmp);
	tmp += sizeof(struct entity);

	firstperson_register(tmp);
	tmp += sizeof(struct entity);

	thirdperson_register(tmp);
	tmp += sizeof(struct entity);

	rtscam_register(tmp);
	tmp += sizeof(struct entity);

	freecam_register(tmp);
	tmp += sizeof(struct entity);

	vrglass_register(tmp);
	tmp += sizeof(struct entity);

//.fbo
	glass_register(tmp);
	tmp += sizeof(struct entity);

	mirror_register(tmp);
	tmp += sizeof(struct entity);

	portal_register(tmp);
	tmp += sizeof(struct entity);

	water_register(tmp);
	tmp += sizeof(struct entity);

//.light
	dirlight_register(tmp);
	tmp += sizeof(struct entity);

	spotlight_register(tmp);
	tmp += sizeof(struct entity);

	projector_register(tmp);
	tmp += sizeof(struct entity);

	pointlight_register(tmp);
	tmp += sizeof(struct entity);

//.else
	curtain_register(tmp);
	tmp += sizeof(struct entity);

	field_register(tmp);
	tmp += sizeof(struct entity);

	ground_register(tmp);
	tmp += sizeof(struct entity);

	particle_register(tmp);
	tmp += sizeof(struct entity);

	skydome_register(tmp);
	tmp += sizeof(struct entity);

	terrain_register(tmp);
	tmp += sizeof(struct entity);

	texball_register(tmp);
	tmp += sizeof(struct entity);

	texbox_register(tmp);
	tmp += sizeof(struct entity);

	weather_register(tmp);
	tmp += sizeof(struct entity);




//---------------------algo----------------------
	algorithm_register(tmp);
	tmp += sizeof(struct entity);

	bintree_register(tmp);
	tmp += sizeof(struct entity);

	bplus_register(tmp);
	tmp += sizeof(struct entity);

	graph_register(tmp);
	tmp += sizeof(struct entity);




//----------------------game----------------------
	the2048_register(tmp);
	tmp += sizeof(struct entity);

	chess_register(tmp);
	tmp += sizeof(struct entity);

	klotski_register(tmp);
	tmp += sizeof(struct entity);

	maze_register(tmp);
	tmp += sizeof(struct entity);

	ooxx_register(tmp);
	tmp += sizeof(struct entity);

	pegged_register(tmp);
	tmp += sizeof(struct entity);

	poker_register(tmp);
	tmp += sizeof(struct entity);

	rubikscube_register(tmp);
	tmp += sizeof(struct entity);

	snake_register(tmp);
	tmp += sizeof(struct entity);

	sudoku_register(tmp);
	tmp += sizeof(struct entity);

	tetris_register(tmp);
	tmp += sizeof(struct entity);

	weiqi_register(tmp);
	tmp += sizeof(struct entity);

	xiangqi_register(tmp);
	tmp += sizeof(struct entity);




//--------------------hack-------------------------
	browser_register(tmp);
	tmp += sizeof(struct entity);

	circuit_register(tmp);
	tmp += sizeof(struct entity);

	fs_register(tmp);
	tmp += sizeof(struct entity);

	pwmtool_register(tmp);
	tmp += sizeof(struct entity);

	rawdump_register(tmp);
	tmp += sizeof(struct entity);

	switch_register(tmp);
	tmp += sizeof(struct entity);




//----------------------item-----------------------
	clock_register(tmp);
	tmp += sizeof(struct entity);

	control_register(tmp);
	tmp += sizeof(struct entity);

	dna_register(tmp);
	tmp += sizeof(struct entity);

	geometry_register(tmp);
	tmp += sizeof(struct entity);

	house_register(tmp);
	tmp += sizeof(struct entity);

	mario_register(tmp);
	tmp += sizeof(struct entity);

	mobius_register(tmp);
	tmp += sizeof(struct entity);

	model_register(tmp);
	tmp += sizeof(struct entity);

	motor_register(tmp);
	tmp += sizeof(struct entity);

	piano_register(tmp);
	tmp += sizeof(struct entity);

	picture_register(tmp);
	tmp += sizeof(struct entity);

	stair_register(tmp);
	tmp += sizeof(struct entity);

	tardis_register(tmp);
	tmp += sizeof(struct entity);

	texmix_register(tmp);
	tmp += sizeof(struct entity);

	tree_register(tmp);
	tmp += sizeof(struct entity);




//----------------------robo----------------------
	theear_register(tmp);
	tmp += sizeof(struct entity);

	theeye_register(tmp);
	tmp += sizeof(struct entity);

	screen_register(tmp);
	tmp += sizeof(struct entity);

	speaker_register(tmp);
	tmp += sizeof(struct entity);




//----------------------robo----------------------
	dancemat_register(tmp);
	tmp += sizeof(struct entity);

	drone_register(tmp);
	tmp += sizeof(struct entity);

	human_register(tmp);
	tmp += sizeof(struct entity);

	otto_register(tmp);
	tmp += sizeof(struct entity);

	rccar_register(tmp);
	tmp += sizeof(struct entity);

	robodog_register(tmp);
	tmp += sizeof(struct entity);




//----------------------test----------------------
	calib3d_register(tmp);
	tmp += sizeof(struct entity);

	codeimg_register(tmp);
	tmp += sizeof(struct entity);

	doodle_register(tmp);
	tmp += sizeof(struct entity);

	example_register(tmp);
	tmp += sizeof(struct entity);

	fractal_register(tmp);
	tmp += sizeof(struct entity);

	oscillo_register(tmp);
	tmp += sizeof(struct entity);

	palette_register(tmp);
	tmp += sizeof(struct entity);

	planet_register(tmp);
	tmp += sizeof(struct entity);




//------------------------tool--------------------
	calculator_register(tmp);
	tmp += sizeof(struct entity);

	font_register(tmp);
	tmp += sizeof(struct entity);

	hex_register(tmp);
	tmp += sizeof(struct entity);

	qrcode_register(tmp);
	tmp += sizeof(struct entity);

	sketchpad_register(tmp);
	tmp += sizeof(struct entity);

	spectrum_register(tmp);
	tmp += sizeof(struct entity);

	terminal_register(tmp);
	tmp += sizeof(struct entity);

	video_register(tmp);
	tmp += sizeof(struct entity);

	voxel_register(tmp);
	tmp += sizeof(struct entity);

	button_register(tmp);
	tmp += sizeof(struct entity);




//-------------------menu------------------
//.2d
	corner_register(tmp);
	tmp += sizeof(struct entity);

	pointer_register(tmp);
	tmp += sizeof(struct entity);

//.3d
	camman_register(tmp);
	tmp += sizeof(struct entity);

	picker_register(tmp);
	tmp += sizeof(struct entity);

//.body
	tabbar_register(tmp);
	tmp += sizeof(struct entity);

//.vkbd
	vjoy_register(tmp);
	tmp += sizeof(struct entity);

	vkbd_register(tmp);
	tmp += sizeof(struct entity);

//.else
	login_register(tmp);
	tmp += sizeof(struct entity);

	overview_register(tmp);
	tmp += sizeof(struct entity);

	detail_register(tmp);
	tmp += sizeof(struct entity);

	skillbar_register(tmp);
	tmp += sizeof(struct entity);
}
