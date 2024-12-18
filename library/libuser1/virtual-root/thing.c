#include "libuser.h"




//chem
void ch4_register(void*);
void h2o_register(void*);
void diamond_register(void*);
void graphene_register(void*);

//gear
void spurgear_register(void*);
void planetary_register(void*);

//chip.baseunit
void mosfet_register(void*);
void finfet_register(void*);
void resistor_register(void*);
void vsrc_register(void*);
//
void pmos_register(void*);
void nmos_register(void*);
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
void freecam_register(void*);
void orthcam_register(void*);
void vrbox_register(void*);
void vrglass_register(void*);

//dxgl
//.fbo
void glass_register(void*);
void mirror_register(void*);
void portal_register(void*);
void water_register(void*);
//.light
void dirlight_register(void*);
void imagelight_register(void*);
void pointlight_register(void*);
void projector_register(void*);
void spotlight_register(void*);
//.sky
void skydome_register(void*);
void equirect_register(void*);
void texbox_register(void*);
//.terrain
void dbggnd_register(void*);
void ground_register(void*);
void terrain_register(void*);
//.else
void curtain_register(void*);
void field_register(void*);
void particle_register(void*);
void weather_register(void*);
void texball_register(void*);

//algo
void algorithm_register(void*);
void bintree_register(void*);
void bplus_register(void*);
void graph_register(void*);

//deeplearn
void mnist_register(void*);
void transformer_register(void*);

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
void wrl3d_register(void*);

//game simple
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

//game complex
void mario_register(void*);

//hack
void browser_register(void*);
void fslist_register(void*);
void pwmtool_register(void*);
void rawdump_register(void*);

//item
void clock_register(void*);
void house_register(void*);
void mobius_register(void*);
void motor_register(void*);
void piano_register(void*);
void satellite_register(void*);
void tardis_register(void*);
void spring_register(void*);
void stick_register(void*);

//plant
void flower_register(void*);
void grass_register(void*);
void tree_register(void*);

//move
void elevator_register(void*);
void ladder_register(void*);
void stair_register(void*);
void telpher_register(void*);

//real
void theear_register(void*);
void theeye_register(void*);
void screen_register(void*);
void speaker_register(void*);

//robo
//.air
void aircraft_register(void*);
void drone_register(void*);
//.space
void rocket_register(void*);
void station_register(void*);
//.land
void rccar_register(void*);
void balance_register(void*);
//.bionic
void human_register(void*);
void otto_register(void*);
void robodog_register(void*);
//.else
void dancemat_register(void*);

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

void cbuffer_register(void*);
void gbuffer_register(void*);

//uiux
void minimap_register(void*);
void picture_register(void*);
void qrcode_register(void*);
void skill_register(void*);
void status_register(void*);
void texmix_register(void*);
void video_register(void*);
void voxel_register(void*);

//
void radar_register(void*);
void oscillo_register(void*);
void spectrum_register(void*);

//debug
void dbgpid_register(void*);

//edit
void hexedit_register(void*);
void mmioedit_register(void*);
void textedit_register(void*);
void codeedit_register(void*);

void button_register(void*);
void slider_register(void*);

//vkbd
void vjoy_register(void*);
void vkbd_register(void*);
void vmouse_register(void*);
void vtouch_register(void*);

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

//vmhw
void arm64_register(void*);
void x8664_register(void*);
void vmddr_register(void*);



//
void thing_exit()
{
}
void thing_init(void* addr)
{
	void* tmp = addr +0x100000 -sizeof(_obj);




//----------------------gear--------------------
	spurgear_register(tmp);
	tmp -= sizeof(_obj);

	planetary_register(tmp);
	tmp -= sizeof(_obj);




//----------------------chem--------------------
	ch4_register(tmp);
	tmp -= sizeof(_obj);

	h2o_register(tmp);
	tmp -= sizeof(_obj);

	diamond_register(tmp);
	tmp -= sizeof(_obj);

	graphene_register(tmp);
	tmp -= sizeof(_obj);




//-------------------chip.baseunit-----------------
	vsrc_register(tmp);
	tmp -= sizeof(_obj);

	resistor_register(tmp);
	tmp -= sizeof(_obj);

	mosfet_register(tmp);
	tmp -= sizeof(_obj);

	finfet_register(tmp);
	tmp -= sizeof(_obj);




//-------------------chip.baseunit-----------------
	pmos_register(tmp);
	tmp -= sizeof(_obj);

	nmos_register(tmp);
	tmp -= sizeof(_obj);

	nand_register(tmp);
	tmp -= sizeof(_obj);

	nor_register(tmp);
	tmp -= sizeof(_obj);

	xor_register(tmp);
	tmp -= sizeof(_obj);

	xnor_register(tmp);
	tmp -= sizeof(_obj);

	fulladd_register(tmp);
	tmp -= sizeof(_obj);

	halfadd_register(tmp);
	tmp -= sizeof(_obj);

	autocmos_register(tmp);
	tmp -= sizeof(_obj);

	not_register(tmp);
	tmp -= sizeof(_obj);

	mem_register(tmp);
	tmp -= sizeof(_obj);

	dff_register(tmp);
	tmp -= sizeof(_obj);

	hbridge_register(tmp);
	tmp -= sizeof(_obj);

	rectify_register(tmp);
	tmp -= sizeof(_obj);




//----------------------tab-----------------------
	karnaugh_register(tmp);
	tmp -= sizeof(_obj);

	truth_register(tmp);
	tmp -= sizeof(_obj);




//----------------------mic-----------------------
	monomic_register(tmp);
	tmp -= sizeof(_obj);

	stereo_register(tmp);
	tmp -= sizeof(_obj);




//----------------------cam------------------------
	freecam_register(tmp);
	tmp -= sizeof(_obj);

	orthcam_register(tmp);
	tmp -= sizeof(_obj);

	vrbox_register(tmp);
	tmp -= sizeof(_obj);

	vrglass_register(tmp);
	tmp -= sizeof(_obj);




//----------------------dxgl-----------------------
//.fbo
	glass_register(tmp);
	tmp -= sizeof(_obj);

	mirror_register(tmp);
	tmp -= sizeof(_obj);

	portal_register(tmp);
	tmp -= sizeof(_obj);

	water_register(tmp);
	tmp -= sizeof(_obj);

//.light
	dirlight_register(tmp);
	tmp -= sizeof(_obj);

	imagelight_register(tmp);
	tmp -= sizeof(_obj);

	projector_register(tmp);
	tmp -= sizeof(_obj);

	pointlight_register(tmp);
	tmp -= sizeof(_obj);

	spotlight_register(tmp);
	tmp -= sizeof(_obj);

//.sky
	skydome_register(tmp);
	tmp -= sizeof(_obj);

	equirect_register(tmp);
	tmp -= sizeof(_obj);

	texbox_register(tmp);
	tmp -= sizeof(_obj);

//.terrain
	dbggnd_register(tmp);
	tmp -= sizeof(_obj);

	ground_register(tmp);
	tmp -= sizeof(_obj);

	terrain_register(tmp);
	tmp -= sizeof(_obj);

//.else
	curtain_register(tmp);
	tmp -= sizeof(_obj);

	field_register(tmp);
	tmp -= sizeof(_obj);

	particle_register(tmp);
	tmp -= sizeof(_obj);

	weather_register(tmp);
	tmp -= sizeof(_obj);

	texball_register(tmp);
	tmp -= sizeof(_obj);




//---------------------algo----------------------
	algorithm_register(tmp);
	tmp -= sizeof(_obj);

	bintree_register(tmp);
	tmp -= sizeof(_obj);

	bplus_register(tmp);
	tmp -= sizeof(_obj);

	graph_register(tmp);
	tmp -= sizeof(_obj);




//----------------------deeplearn----------------------
	mnist_register(tmp);
	tmp -= sizeof(_obj);

	transformer_register(tmp);
	tmp -= sizeof(_obj);




//----------------------game----------------------
	the2048_register(tmp);
	tmp -= sizeof(_obj);

	chess_register(tmp);
	tmp -= sizeof(_obj);

	klotski_register(tmp);
	tmp -= sizeof(_obj);

	maze_register(tmp);
	tmp -= sizeof(_obj);

	ooxx_register(tmp);
	tmp -= sizeof(_obj);

	pegged_register(tmp);
	tmp -= sizeof(_obj);

	poker_register(tmp);
	tmp -= sizeof(_obj);

	rubikscube_register(tmp);
	tmp -= sizeof(_obj);

	snake_register(tmp);
	tmp -= sizeof(_obj);

	sudoku_register(tmp);
	tmp -= sizeof(_obj);

	tetris_register(tmp);
	tmp -= sizeof(_obj);

	weiqi_register(tmp);
	tmp -= sizeof(_obj);

	xiangqi_register(tmp);
	tmp -= sizeof(_obj);

	mario_register(tmp);
	tmp -= sizeof(_obj);




//--------------------hack-------------------------
	browser_register(tmp);
	tmp -= sizeof(_obj);

	fslist_register(tmp);
	tmp -= sizeof(_obj);

	pwmtool_register(tmp);
	tmp -= sizeof(_obj);

	rawdump_register(tmp);
	tmp -= sizeof(_obj);




//----------------------item-----------------------
	clock_register(tmp);
	tmp -= sizeof(_obj);

	house_register(tmp);
	tmp -= sizeof(_obj);

	mobius_register(tmp);
	tmp -= sizeof(_obj);

	motor_register(tmp);
	tmp -= sizeof(_obj);

	piano_register(tmp);
	tmp -= sizeof(_obj);

	satellite_register(tmp);
	tmp -= sizeof(_obj);

	stair_register(tmp);
	tmp -= sizeof(_obj);

	tardis_register(tmp);
	tmp -= sizeof(_obj);

	spring_register(tmp);
	tmp -= sizeof(_obj);

	stick_register(tmp);
	tmp -= sizeof(_obj);




//----------------------file----------------------
	flower_register(tmp);
	tmp -= sizeof(_obj);

	grass_register(tmp);
	tmp -= sizeof(_obj);

	tree_register(tmp);
	tmp -= sizeof(_obj);




//----------------------file----------------------
//.biology
	dna_register(tmp);
	tmp -= sizeof(_obj);

//.factory
	gcode_register(tmp);
	tmp -= sizeof(_obj);

	gerber_register(tmp);
	tmp -= sizeof(_obj);

//.model
	geometry_register(tmp);
	tmp -= sizeof(_obj);

	obj3d_register(tmp);
	tmp -= sizeof(_obj);

	stl3d_register(tmp);
	tmp -= sizeof(_obj);

	wrl3d_register(tmp);
	tmp -= sizeof(_obj);





//----------------------real----------------------
	theear_register(tmp);
	tmp -= sizeof(_obj);

	theeye_register(tmp);
	tmp -= sizeof(_obj);

	screen_register(tmp);
	tmp -= sizeof(_obj);

	speaker_register(tmp);
	tmp -= sizeof(_obj);




//----------------------robo----------------------
//.air
	aircraft_register(tmp);
	tmp -= sizeof(_obj);

	drone_register(tmp);
	tmp -= sizeof(_obj);
//.space
	rocket_register(tmp);
	tmp -= sizeof(_obj);

	station_register(tmp);
	tmp -= sizeof(_obj);
//.land
	rccar_register(tmp);
	tmp -= sizeof(_obj);

	balance_register(tmp);
	tmp -= sizeof(_obj);
//.bionic
	human_register(tmp);
	tmp -= sizeof(_obj);

	otto_register(tmp);
	tmp -= sizeof(_obj);

	robodog_register(tmp);
	tmp -= sizeof(_obj);
//.else
	dancemat_register(tmp);
	tmp -= sizeof(_obj);




//----------------------test----------------------
	calib3d_register(tmp);
	tmp -= sizeof(_obj);

	codeimg_register(tmp);
	tmp -= sizeof(_obj);

	doodle_register(tmp);
	tmp -= sizeof(_obj);

	example_register(tmp);
	tmp -= sizeof(_obj);

	fractal_register(tmp);
	tmp -= sizeof(_obj);

	palette_register(tmp);
	tmp -= sizeof(_obj);

	planet_register(tmp);
	tmp -= sizeof(_obj);




//------------------------tool--------------------
	calculator_register(tmp);
	tmp -= sizeof(_obj);

	font_register(tmp);
	tmp -= sizeof(_obj);

	sketchpad_register(tmp);
	tmp -= sizeof(_obj);

	terminal_register(tmp);
	tmp -= sizeof(_obj);

	dbgpid_register(tmp);
	tmp -= sizeof(_obj);




//-------------postprocess-----------------
	cbuffer_register(tmp);
	tmp -= sizeof(_obj);

	gbuffer_register(tmp);
	tmp -= sizeof(_obj);




//-------------------uiux------------------
	minimap_register(tmp);
	tmp -= sizeof(_obj);

	picture_register(tmp);
	tmp -= sizeof(_obj);

	qrcode_register(tmp);
	tmp -= sizeof(_obj);

	skill_register(tmp);
	tmp -= sizeof(_obj);

	status_register(tmp);
	tmp -= sizeof(_obj);

	texmix_register(tmp);
	tmp -= sizeof(_obj);

	video_register(tmp);
	tmp -= sizeof(_obj);

	voxel_register(tmp);
	tmp -= sizeof(_obj);




//-------------------instrument-------------------
	radar_register(tmp);
	tmp -= sizeof(_obj);

	oscillo_register(tmp);
	tmp -= sizeof(_obj);

	spectrum_register(tmp);
	tmp -= sizeof(_obj);




//-------------------bar-------------------
	skillbar_register(tmp);
	tmp -= sizeof(_obj);

	tabbar_register(tmp);
	tmp -= sizeof(_obj);




//-------------------edit------------------
	hexedit_register(tmp);
	tmp -= sizeof(_obj);

	mmioedit_register(tmp);
	tmp -= sizeof(_obj);




//-------------------vkbd------------------
	button_register(tmp);
	tmp -= sizeof(_obj);

	slider_register(tmp);
	tmp -= sizeof(_obj);




//-------------------vkbd------------------
	vjoy_register(tmp);
	tmp -= sizeof(_obj);

	vkbd_register(tmp);
	tmp -= sizeof(_obj);

	vmouse_register(tmp);
	tmp -= sizeof(_obj);

	vtouch_register(tmp);
	tmp -= sizeof(_obj);




//-------------------menu------------------
//.2d
	corner_register(tmp);
	tmp -= sizeof(_obj);

	pointer_register(tmp);
	tmp -= sizeof(_obj);

//.3d
	camman_register(tmp);
	tmp -= sizeof(_obj);

	picker_register(tmp);
	tmp -= sizeof(_obj);

//.else
	login_register(tmp);
	tmp -= sizeof(_obj);

	overview_register(tmp);
	tmp -= sizeof(_obj);

	detail_register(tmp);
	tmp -= sizeof(_obj);



//----------------vmhw-------------------
	arm64_register(tmp);
	tmp -= sizeof(_obj);

	x8664_register(tmp);
	tmp -= sizeof(_obj);

	vmddr_register(tmp);
	tmp -= sizeof(_obj);
}
