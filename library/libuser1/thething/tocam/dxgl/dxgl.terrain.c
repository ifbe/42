#include "libuser.h"
#define DEPBUF buf0
#define RGBBUF buf1
#define CTXBUF buf2
#define RGBTEX 0
#define DEPTEX 1
void loadtexfromfile(struct glsrc* src, int idx, u8* name);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




char* terrain_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 n;\n"
"layout(location = 2)in mediump vec3 t;\n"
"out mediump vec3 objxyz;\n"
"out mediump vec3 normal;\n"
"out mediump vec3 texuvw;\n"
//"out mediump vec3 uvw;\n"
//"out mediump vec3 xyz;\n"
"uniform mat4 objmat;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"mediump vec4 xyzw = objmat * vec4(v, 1.0);\n"
	"objxyz = vec3(xyzw);\n"
	"normal = n;\n"
	"texuvw = t;\n"
	"gl_Position = cammvp * xyzw;\n"
"}\n";
/*
char* terrain_glsl_g =
GLSL_VERSION
"layout(triangles) in;\n"
//"layout(line_strip, max_vertices = 6) out;\n"
"layout(triangle_strip, max_vertices = 6) out;\n"
"in mediump vec3 uvw[];\n"
"in mediump vec3 xyz[];\n"
"out mediump vec3 objxyz;\n"
"out mediump vec3 normal;\n"
"out mediump vec3 texuvw;\n"
"void main(){\n"
	"vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);\n"
	"vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);\n"
	"vec3 n = normalize(cross(a, b));\n"
	"gl_Position = gl_in[0].gl_Position;\n"
	"objxyz = xyz[0];\n"
	"texuvw = uvw[0];\n"
	"normal = n;\n"
	"EmitVertex();\n"
	"gl_Position = gl_in[1].gl_Position;\n"
	"objxyz = xyz[1];\n"
	"texuvw = uvw[1];\n"
	"normal = n;\n"
	"EmitVertex();\n"
	"gl_Position = gl_in[2].gl_Position;\n"
	"objxyz = xyz[2];\n"
	"texuvw = uvw[2];\n"
	"normal = n;\n"
	"EmitVertex();\n"
	"EndPrimitive();\n"
"}\n";
*/
char* terrain_glsl_f =
GLSL_VERSION
"in mediump vec3 objxyz;\n"
"in mediump vec3 normal;\n"
"in mediump vec3 texuvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D rgbtex;\n"
"uniform mediump vec3 camxyz;\n"

"mediump vec3 litdir = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 litrgb = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 mtrfao = vec3(0.2, 0.8, 1.0);\n"
"mediump float metal = mtrfao.x;\n"
"mediump float rough = mtrfao.y;\n"
"mediump float amocc = mtrfao.z;\n"
"mediump float PI = 3.1415926535897932384626433832795028841971693993151;\n"
"mediump float getD(mediump float v, mediump float r){\n"
    "float a2 = r*r*r*r;\n"
    "float de = (v*v * (a2 - 1.0) + 1.0);\n"
    "return a2 / (PI * de * de);\n"
"}\n"
"mediump float getG(mediump float v, mediump float r){\n"
    "float k = (r+1.0) * (r+1.0) / 8.0;\n"
    "return v / (v * (1.0 - k) + k);\n"
"}\n"
"void main(){\n"
	"mediump vec3 albedo = texture(rgbtex, texuvw.xy).bgr;\n"

	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 E = normalize(camxyz - objxyz);\n"
	"mediump vec3 F0 = mix(vec3(0.04), albedo, metal);\n"

	"mediump vec3 ocolor = vec3(0.0);\n"
	//"for(){\n"
		"mediump vec3 L = litdir;\n"
		//"mediump float distance = length(L);\n"
		//"mediump float attenuation = 1.0 / (distance * distance);\n"
		//"mediump vec3 radiance = litrgb * attenuation;\n"
		"mediump vec3 radiance = litrgb;\n"

		"L = normalize(L);\n"
		"mediump vec3 H = normalize(E + L);\n"
		"mediump float NdotL = max(dot(N, L), 0.0);\n"
		"mediump float NdotE = max(dot(N, E), 0.0);\n"
		"mediump float NdotH = max(dot(N, H), 0.0);\n"
		"mediump float HdotE = max(dot(H, E), 0.0);\n"

		"mediump float G = getG(NdotL, rough)*getG(NdotE, rough);\n"
		"mediump float D = getD(NdotH, rough);\n"
		"mediump vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);\n"

		"mediump vec3 kS = F;\n"
		"mediump vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);\n"
		"mediump vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);\n"
		"ocolor += (kD * albedo / PI + specular) * radiance * NdotL;\n"
	//"}\n"

	"ocolor += vec3(0.03) * albedo * amocc;\n"
	"ocolor = ocolor / (ocolor + vec3(1.0));\n"
	"ocolor = pow(ocolor, vec3(1.0/2.2));\n"
	"FragColor = vec4(ocolor, 1.0);\n"
"}\n";

void copyname(u8* dst, u8* src)
{
	int j;
	while(0x20 == *src)src++;
	for(j=0;j<256;j++){
		if(src[j] < 0x20)break;
		dst[j] = src[j];
	}
	dst[j] = 0;
}




void terrain_generate(float (*vbuf)[9], u16* ibuf, struct entity* act, struct glsrc* src)
{
	int x,y,j;
	int cx,cy,px,py;
	int w = src->tex[DEPTEX].w;
	int h = src->tex[DEPTEX].h;
	u8* rgba = src->tex[DEPTEX].data;

	//cx,cy is integer
	cx = w * act->fx0;
	cy = h * act->fy0;
	for(y=0;y<255;y++){
		for(x=0;x<255;x++){
			//xyz[-1,1]
			px = cx+x-127;
			py = cy+y-127;
			vbuf[y*256+x][0] = (float)px/w*2 - 1.0;
			vbuf[y*256+x][1] = (float)py/h*2 - 1.0;

			//jpg inverts y?
			py = h-1-py;
			if((px < 0) | (px >= w) | (py < 0) | (py >= h))vbuf[y*256+x][2] = 0.0;
			vbuf[y*256+x][2] = rgba[(w*py+px)*4]/255.0;

			//uv[0,1]
			vbuf[y*256+x][6] = (float)px/w;
			vbuf[y*256+x][7] = (float)py/h;
			vbuf[y*256+x][8] = 0.0;
		}
	}

	for(y=1;y<254;y++){
		for(x=1;x<254;x++){
			//normal
			vbuf[y*256+x][3] = vbuf[(y+0)*256+x+1][2] - vbuf[(y+0)*256+x-1][2];
			vbuf[y*256+x][4] = vbuf[(y+1)*256+x+0][2] - vbuf[(y-1)*256+x+0][2];
			vbuf[y*256+x][5] = 2.0;
		}
	}

	j = 0;
	for(y=0;y<254;y++)
	{
		for(x=0;x<254;x++)
		{
			ibuf[j+0] = y*256+x;
			ibuf[j+1] = y*256+x+1;
			ibuf[j+2] = y*256+x+256;

			ibuf[j+3] = y*256+x+256;
			ibuf[j+4] = y*256+x+1;
			ibuf[j+5] = y*256+x+257;

			j += 6;
		}
	}
}/*
void terrain_locate(vec4 v, struct entity* act)
{
	//geometry
	int w = act->width;
	int h = act->height;

	//center
	float sx = (w-1) / 2.0;
	float sy = (h-1) / 2.0;

	//position
	float fx = sx + v[0] / 1000.0;
	float fy = sy - v[1] / 1000.0;

	//
	int x0;
	int x1;
	if(fx > 0){x0 = (int)fx;x1 = x0 + 1;}
	else{x1 = -(int)(-fx);x0 = x1 - 1;}

	//
	int y0;
	int y1;
	if(fx > 0){y0 = (int)fy;y1 = y0 + 1;}
	else{y1 = -(int)(-fy);y0 = y1 - 1;}

	//
	//say("%d,%d,%d,%d\n",x0,x1,y0,y1);
	if(x0 < 0)goto edge;
	if(x1 >= w)goto edge;
	if(y0 < 0)goto edge;
	if(y1 >= h)goto edge;

	//
	float val;
	float max;
	u8* rgba = act->DEPBUF;
	max = rgba[(w*y0 + x0)*4];
	val = rgba[(w*y0 + x1)*4];
	if(val > max)max = val;
	val = rgba[(w*y1 + x0)*4];
	if(val > max)max = val;
	val = rgba[(w*y1 + x1)*4];
	if(val > max)max = val;
	v[2] = max * 10000.0 / 255;
	return;

edge:
	v[2] = 0.0;
}
static void terrain_ask(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	float x,y;
	struct entity* act = (void*)(self->chip);
	int w = act->width;
	int h = act->height;
	float* v = (void*)buf;

	terrain_locate(v, act);
	say("%f,%f,%f\n", v[0], v[1], v[2]);

	x = v[0] / 1000.0;
	act->fxn = (int)x;
	while(act->fxn <= act->fx0-64){
		act->fx0 -= 64;
		act->iw0 = 0;
	}
	while(act->fxn >= act->fx0+64){
		act->fx0 += 64;
		act->iw0 = 0;
	}

	y = v[1] / 1000.0;
	act->fyn = (int)y;
	while(act->fyn <= act->fy0-64){
		act->fy0 -= 64;
		act->iw0 = 0;
	}
	while(act->fyn >= act->fy0+64){
		act->fy0 += 64;
		act->iw0 = 0;
	}

	say("%f,%f,%f,%f\n", act->fx0, act->fy0, act->fxn, act->fyn);
}*/




void terrain_ctxforwnd(struct glsrc* src, u8* rgbfile, u8* depfile)
{
	src->method = 'i';
	src->geometry = 3;

	//shader
	src->vs = terrain_glsl_v;
	//src->gs = terrain_glsl_g;
	src->fs = terrain_glsl_f;
	src->shader_enq = 42;

	//argument
	src->arg[0].name = "objmat";
	src->arg[0].data = memorycreate(4*4*4, 0);
	src->arg[0].fmt = 'm';

	//texture
	src->tex[RGBTEX].fmt = hex32('r','g','b','a');
	src->tex[RGBTEX].name = "rgbtex";
	src->tex[RGBTEX].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, RGBTEX, rgbfile);
	src->tex[RGBTEX].enq = 42;

	src->tex[DEPTEX].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, DEPTEX, depfile);

	//vertex
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*9;
	src->vbuf_h = 256*255;
	src->vbuf_len = (src->vbuf_w) * 256*256;
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 42;

	//index
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 254*254*2;
	src->ibuf_len = (src->ibuf_w) * 256*256*2;
	src->ibuf = memorycreate(src->ibuf_len, 0);
	src->ibuf_enq = 42;
}
static void terrain_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* area)
{
	struct glsrc* src;
	float* mat;
	void* vbuf;
	void* ibuf;

	float w = vec3_getlen(geom->fs.vr);
	float h = vec3_getlen(geom->fs.vf);
	float x = camg->frus.vc[0]/w/2 + 0.5;
	float y = camg->frus.vc[1]/h/2 + 0.5;
	float dx = x - act->fx0;
	float dy = y - act->fy0;
	if(dx<0)dx = -dx;
	if(dy<0)dy = -dy;
	//say("x=%f,y=%f,dx=%f,dy=%f\n",x,y,dx,dy);

	if((dx > 1.0/16)|(dy > 1.0/16)){
		act->fx0 = x;
		act->fy0 = y;

		src = act->CTXBUF;
		if(0 == src)return;

		//x0,y0,z0,dx,dy,dz -> ndc
		vbuf = src->vbuf;
		if(0 == vbuf)return;
		ibuf = src->ibuf;
		if(0 == ibuf)return;
		terrain_generate(vbuf, ibuf, act, src);
		src->vbuf_enq += 1;

		//ndc -> geom
		mat = src->arg[0].data;
		if(0 == mat)return;
		mat[ 0] = geom->fs.vr[0];
		mat[ 1] = geom->fs.vr[1];
		mat[ 2] = geom->fs.vr[2];
		mat[ 3] = 0.0;
		mat[ 4] = geom->fs.vf[0];
		mat[ 5] = geom->fs.vf[1];
		mat[ 6] = geom->fs.vf[2];
		mat[ 7] = 0.0;
		mat[ 8] = geom->fs.vt[0];
		mat[ 9] = geom->fs.vt[1];
		mat[10] = geom->fs.vt[2];
		mat[11] = 0.0;
		mat[12] = geom->fs.vc[0];
		mat[13] = geom->fs.vc[1];
		mat[14] = geom->fs.vc[2];
		mat[15] = 1.0;
	}
	gl41data_insert(ctx, 's', act->CTXBUF, 1);
}




static void terrain_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void terrain_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void terrain_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		terrain_draw_gl41(ent,slot, wor,geom, dup,camg, wnd, area);
	}
}
static void terrain_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void terrain_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void terrain_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void terrain_search(struct entity* act)
{
}
static void terrain_modify(struct entity* act)
{
}
static void terrain_delete(struct entity* act)
{
	if(0 == act)return;
}
static void terrain_create(struct entity* act, void* arg, int argc, u8** argv)
{
	int j,k;
	struct glsrc* src;
	u8 rgbpath[256] = {0};
	u8 deppath[256] = {0};
	if(0 == act)return;

	act->fx0 = -2.0;
	act->fy0 = -2.0;
	act->fz0 = -2.0;
	for(j=1;j<argc;j++){
		//say("%.6s\n",argv[j]);
		if(0 == ncmp(argv[j], "rgb:", 4)){copyname(rgbpath, argv[j]+4);continue;}
		if(0 == ncmp(argv[j], "dep:", 4)){copyname(deppath, argv[j]+4);continue;}
	}

	if(0 == arg)arg = "datafile/jpg/cartoon.jpg";
	if(0 == rgbpath[0])copyname(rgbpath, arg);
	if(0 == deppath[0])copyname(deppath, arg);

	src = act->CTXBUF = memorycreate(0x200, 0);
	if(0 == src)return;
	terrain_ctxforwnd(src, rgbpath, deppath);


	int x,y;
	u8* rgba;
	if((0 == src->tex[DEPTEX].w) | (0 == src->tex[DEPTEX].h))
	{
		src->tex[DEPTEX].w = 2048;
		src->tex[DEPTEX].h = 2048;
		if(0 == src->tex[DEPTEX].data)src->tex[DEPTEX].data = memorycreate(2048*2048*4, 0);

		rgba = src->tex[DEPTEX].data;
		for(y=0;y<2048;y++)
		{
			for(x=0;x<2048;x++)
			{
				rgba[4*(y*2048 + x)+0] = getrandom()%256;
				rgba[4*(y*2048 + x)+1] = getrandom()%256;
				rgba[4*(y*2048 + x)+2] = getrandom()%256;
			}
		}
	}
}




void terrain_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'r', 'r', 'a', 'i', 'n', 0);

	p->oncreate = (void*)terrain_create;
	p->ondelete = (void*)terrain_delete;
	p->onsearch = (void*)terrain_search;
	p->onmodify = (void*)terrain_modify;

	p->onlinkup = (void*)terrain_linkup;
	p->ondiscon = (void*)terrain_discon;
	p->onread  = (void*)terrain_read;
	p->onwrite = (void*)terrain_write;
}
