#include "libuser.h"
//chem
void ch4_register(void*);
void h2o_register(void*);
void diamond_register(void*);
void graphene_register(void*);

//chip.baseunit
void finfet_register(void*);
void resistor_register(void*);
void vsrc_register(void*);
//
void halfadd_register(void*);
void fulladd_register(void*);
//chip.compound
void nand_register(void*);
void nor_register(void*);
void xor_register(void*);
void xnor_register(void*);
void autocmos_register(void*);
//
void not_register(void*);
void mem_register(void*);
void dff_register(void*);
void hbridge_register(void*);
void rectify_register(void*);

//table
void karnaugh_register(void*);
void truth_register(void*);

//mic
void monomic_register(void*);
void stereo_register(void*);

//cam
void orthcam_register(void*);
void firstperson_register(void*);
void thirdperson_register(void*);
void rtscam_register(void*);
void freecam_register(void*);
void vrglass_register(void*);

//dxgl
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
void mnist_register(void*);

//file
//.biology
void dna_register(void*);
//.factory
void gcode_register(void*);
void gerber_register(void*);
//.model
void geometry_register(void*);
void obj3d_register(void*);
void stl3d_register(void*);
//.program
void cpure_register(void*);
//.other
void hexedit_register(void*);

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

//item
void clock_register(void*);
void house_register(void*);
void mario_register(void*);
void mobius_register(void*);
void motor_register(void*);
void piano_register(void*);
void stair_register(void*);
void tardis_register(void*);
void tree_register(void*);

//real
void theear_register(void*);
void theeye_register(void*);
void screen_register(void*);
void speaker_register(void*);

//robo
void balance_register(void*);
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
void palette_register(void*);
void planet_register(void*);

//bar
void skillbar_register(void*);
void tabbar_register(void*);

//uiux
void gbuffer_register(void*);
void oscillo_register(void*);
void picture_register(void*);
void qrcode_register(void*);
void spectrum_register(void*);
void texmix_register(void*);
void video_register(void*);
void voxel_register(void*);

//vkbd
void button_register(void*);
void control_register(void*);
void slider_register(void*);
void vjoy_register(void*);
void vkbd_register(void*);

//tool
void calculator_register(void*);
void font_register(void*);
void sketchpad_register(void*);
void terminal_register(void*);

//menu
//.2d
void corner_register(void*);
void pointer_register(void*);
//.3d
void camman_register(void*);
void picker_register(void*);
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
	void* tmp = addr +0x100000 -sizeof(struct entity);




//----------------------chem--------------------
	ch4_register(tmp);
	tmp -= sizeof(struct entity);

	h2o_register(tmp);
	tmp -= sizeof(struct entity);

	diamond_register(tmp);
	tmp -= sizeof(struct entity);

	graphene_register(tmp);
	tmp -= sizeof(struct entity);




//-------------------chip.baseunit-----------------
	vsrc_register(tmp);
	tmp -= sizeof(struct entity);

	resistor_register(tmp);
	tmp -= sizeof(struct entity);

	finfet_register(tmp);
	tmp -= sizeof(struct entity);




//-------------------chip.baseunit-----------------
	nand_register(tmp);
	tmp -= sizeof(struct entity);

	nor_register(tmp);
	tmp -= sizeof(struct entity);

	xor_register(tmp);
	tmp -= sizeof(struct entity);

	xnor_register(tmp);
	tmp -= sizeof(struct entity);

	fulladd_register(tmp);
	tmp -= sizeof(struct entity);

	halfadd_register(tmp);
	tmp -= sizeof(struct entity);

	autocmos_register(tmp);
	tmp -= sizeof(struct entity);

	not_register(tmp);
	tmp -= sizeof(struct entity);

	mem_register(tmp);
	tmp -= sizeof(struct entity);

	dff_register(tmp);
	tmp -= sizeof(struct entity);

	hbridge_register(tmp);
	tmp -= sizeof(struct entity);

	rectify_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------tab-----------------------
	karnaugh_register(tmp);
	tmp -= sizeof(struct entity);

	truth_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------mic-----------------------
	monomic_register(tmp);
	tmp -= sizeof(struct entity);

	stereo_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------cam------------------------
	orthcam_register(tmp);
	tmp -= sizeof(struct entity);

	firstperson_register(tmp);
	tmp -= sizeof(struct entity);

	thirdperson_register(tmp);
	tmp -= sizeof(struct entity);

	rtscam_register(tmp);
	tmp -= sizeof(struct entity);

	freecam_register(tmp);
	tmp -= sizeof(struct entity);

	vrglass_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------dxgl-----------------------
//.fbo
	glass_register(tmp);
	tmp -= sizeof(struct entity);

	mirror_register(tmp);
	tmp -= sizeof(struct entity);

	portal_register(tmp);
	tmp -= sizeof(struct entity);

	water_register(tmp);
	tmp -= sizeof(struct entity);

//.light
	dirlight_register(tmp);
	tmp -= sizeof(struct entity);

	spotlight_register(tmp);
	tmp -= sizeof(struct entity);

	projector_register(tmp);
	tmp -= sizeof(struct entity);

	pointlight_register(tmp);
	tmp -= sizeof(struct entity);

//.else
	curtain_register(tmp);
	tmp -= sizeof(struct entity);

	field_register(tmp);
	tmp -= sizeof(struct entity);

	ground_register(tmp);
	tmp -= sizeof(struct entity);

	particle_register(tmp);
	tmp -= sizeof(struct entity);

	skydome_register(tmp);
	tmp -= sizeof(struct entity);

	terrain_register(tmp);
	tmp -= sizeof(struct entity);

	texball_register(tmp);
	tmp -= sizeof(struct entity);

	texbox_register(tmp);
	tmp -= sizeof(struct entity);

	weather_register(tmp);
	tmp -= sizeof(struct entity);




//---------------------algo----------------------
	algorithm_register(tmp);
	tmp -= sizeof(struct entity);

	bintree_register(tmp);
	tmp -= sizeof(struct entity);

	bplus_register(tmp);
	tmp -= sizeof(struct entity);

	graph_register(tmp);
	tmp -= sizeof(struct entity);

	mnist_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------game----------------------
	the2048_register(tmp);
	tmp -= sizeof(struct entity);

	chess_register(tmp);
	tmp -= sizeof(struct entity);

	klotski_register(tmp);
	tmp -= sizeof(struct entity);

	maze_register(tmp);
	tmp -= sizeof(struct entity);

	ooxx_register(tmp);
	tmp -= sizeof(struct entity);

	pegged_register(tmp);
	tmp -= sizeof(struct entity);

	poker_register(tmp);
	tmp -= sizeof(struct entity);

	rubikscube_register(tmp);
	tmp -= sizeof(struct entity);

	snake_register(tmp);
	tmp -= sizeof(struct entity);

	sudoku_register(tmp);
	tmp -= sizeof(struct entity);

	tetris_register(tmp);
	tmp -= sizeof(struct entity);

	weiqi_register(tmp);
	tmp -= sizeof(struct entity);

	xiangqi_register(tmp);
	tmp -= sizeof(struct entity);




//--------------------hack-------------------------
	browser_register(tmp);
	tmp -= sizeof(struct entity);

	circuit_register(tmp);
	tmp -= sizeof(struct entity);

	fs_register(tmp);
	tmp -= sizeof(struct entity);

	pwmtool_register(tmp);
	tmp -= sizeof(struct entity);

	rawdump_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------item-----------------------
	clock_register(tmp);
	tmp -= sizeof(struct entity);

	house_register(tmp);
	tmp -= sizeof(struct entity);

	mario_register(tmp);
	tmp -= sizeof(struct entity);

	mobius_register(tmp);
	tmp -= sizeof(struct entity);

	motor_register(tmp);
	tmp -= sizeof(struct entity);

	piano_register(tmp);
	tmp -= sizeof(struct entity);

	stair_register(tmp);
	tmp -= sizeof(struct entity);

	tardis_register(tmp);
	tmp -= sizeof(struct entity);

	tree_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------file----------------------
//.biology
	dna_register(tmp);
	tmp -= sizeof(struct entity);

//.factory
	gcode_register(tmp);
	tmp -= sizeof(struct entity);

	gerber_register(tmp);
	tmp -= sizeof(struct entity);

//.model
	geometry_register(tmp);
	tmp -= sizeof(struct entity);

	obj3d_register(tmp);
	tmp -= sizeof(struct entity);

	stl3d_register(tmp);
	tmp -= sizeof(struct entity);

//.program
	cpure_register(tmp);
	tmp -= sizeof(struct entity);

//.else
	hexedit_register(tmp);
	tmp -= sizeof(struct entity);





//----------------------real----------------------
	theear_register(tmp);
	tmp -= sizeof(struct entity);

	theeye_register(tmp);
	tmp -= sizeof(struct entity);

	screen_register(tmp);
	tmp -= sizeof(struct entity);

	speaker_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------robo----------------------
	balance_register(tmp);
	tmp -= sizeof(struct entity);

	dancemat_register(tmp);
	tmp -= sizeof(struct entity);

	drone_register(tmp);
	tmp -= sizeof(struct entity);

	human_register(tmp);
	tmp -= sizeof(struct entity);

	otto_register(tmp);
	tmp -= sizeof(struct entity);

	rccar_register(tmp);
	tmp -= sizeof(struct entity);

	robodog_register(tmp);
	tmp -= sizeof(struct entity);




//----------------------test----------------------
	calib3d_register(tmp);
	tmp -= sizeof(struct entity);

	codeimg_register(tmp);
	tmp -= sizeof(struct entity);

	doodle_register(tmp);
	tmp -= sizeof(struct entity);

	example_register(tmp);
	tmp -= sizeof(struct entity);

	fractal_register(tmp);
	tmp -= sizeof(struct entity);

	palette_register(tmp);
	tmp -= sizeof(struct entity);

	planet_register(tmp);
	tmp -= sizeof(struct entity);




//------------------------tool--------------------
	calculator_register(tmp);
	tmp -= sizeof(struct entity);

	font_register(tmp);
	tmp -= sizeof(struct entity);

	sketchpad_register(tmp);
	tmp -= sizeof(struct entity);

	terminal_register(tmp);
	tmp -= sizeof(struct entity);




//-------------------uiux------------------
	gbuffer_register(tmp);
	tmp -= sizeof(struct entity);

	oscillo_register(tmp);
	tmp -= sizeof(struct entity);

	picture_register(tmp);
	tmp -= sizeof(struct entity);

	qrcode_register(tmp);
	tmp -= sizeof(struct entity);

	spectrum_register(tmp);
	tmp -= sizeof(struct entity);

	texmix_register(tmp);
	tmp -= sizeof(struct entity);

	video_register(tmp);
	tmp -= sizeof(struct entity);

	voxel_register(tmp);
	tmp -= sizeof(struct entity);




//-------------------bar-------------------
	skillbar_register(tmp);
	tmp -= sizeof(struct entity);

	tabbar_register(tmp);
	tmp -= sizeof(struct entity);




//-------------------vkbd------------------
	button_register(tmp);
	tmp -= sizeof(struct entity);

	control_register(tmp);
	tmp -= sizeof(struct entity);

	slider_register(tmp);
	tmp -= sizeof(struct entity);

	vjoy_register(tmp);
	tmp -= sizeof(struct entity);

	vkbd_register(tmp);
	tmp -= sizeof(struct entity);




//-------------------menu------------------
//.2d
	corner_register(tmp);
	tmp -= sizeof(struct entity);

	pointer_register(tmp);
	tmp -= sizeof(struct entity);

//.3d
	camman_register(tmp);
	tmp -= sizeof(struct entity);

	picker_register(tmp);
	tmp -= sizeof(struct entity);

//.else
	login_register(tmp);
	tmp -= sizeof(struct entity);

	overview_register(tmp);
	tmp -= sizeof(struct entity);

	detail_register(tmp);
	tmp -= sizeof(struct entity);
}
