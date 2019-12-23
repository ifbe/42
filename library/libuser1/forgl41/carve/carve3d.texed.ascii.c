#include "libuser.h"
int utf2unicode(u8* src,u32* dst);
void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);




#ifdef __ANDROID__
	static char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 colour;\n"
		"in mediump vec2 texuvw;\n"
		"out mediump vec4 FragColor;\n"
		"uniform sampler2D tex0;\n"
		"void main(){\n"
			"FragColor = vec4(colour, 1.0)*texture(tex0, texuvw).aaaa;\n"
		"}\n"
	};
#else
	static char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 colour;\n"
		"in mediump vec2 texuvw;\n"
		"out mediump vec4 FragColor;\n"
		"uniform sampler2D tex0;\n"
		"void main(){\n"
			"FragColor = vec4(colour, 1.0)*texture(tex0, texuvw).rrrr;\n"
		"}\n"
	};
#endif

static char font3dvert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"layout(location = 2)in mediump vec2 t;\n"
"out mediump vec3 colour;\n"
"out mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"texuvw = t;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

static u8* buf = 0;




static int aidfont_load()
{
	int j;
	if(buf)return 0;
	say("@loadbuf\n");

	buf = memorycreate(2048*2048*4, 0);
	if(0 == buf)return -1;

	//clean
	for(j=0;j<2048*2048*4;j++)buf[j] = 0;

	//utf8
	for(j=0x80;j<0x4000;j++){
		drawunicode_alpha(buf+0, 2048, 2048, 
			(j&0x7f)<<4, (j&0xff80)>>3, j
		);
	}
	for(j=0;j<0x4000;j++){
		drawunicode_alpha(buf+0x400000, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x4000
		);
	}
	for(j=0;j<0x4000;j++){
		drawunicode_alpha(buf+0x800000, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x8000
		);
	}
	for(j=0;j<0x4000;j++){
		drawunicode_alpha(buf+0xc00000, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0xc000
		);
	}

	//failsafe
	for(j=0x20;j<0x80;j++){
		drawascii_alpha(buf+0, 2048, 2048,
			j<<4, 0, j
		);
	}
	return 0;
}
static int aidfont_fill(struct glsrc* src, int id)
{
	src->method = 'i';
	src->geometry = 3;

	if(0 == src->vs){
		src->vs = font3dvert;
		src->fs = fontfrag;
		src->shader_enq = 1;
	}

	if(0 == src->tex[0].data){
		if(0 == buf)return -3;

		src->tex[0].name = "tex0";
		src->tex[0].data = buf + 0x400000*id;

		src->tex[0].w = 2048;
		src->tex[0].h = 2048;
		src->tex[0].fmt = hex32('o','n','e', 0);
		src->tex[0].enq = 1;
	}

	if(0 == src->ibuf){
		src->ibuf_len = 0x100000;
		src->ibuf = memorycreate(src->ibuf_len, 0);
		if(0 == src->ibuf)return -2;

		src->ibuf_w = 2*3;
		src->ibuf_h = 0;	//(src->ibuf_len) / (src->ibuf_w);
		src->ibuf_fmt = 0x222;
		src->ibuf_enq = 1;
	}

	if(0 == src->vbuf){
		src->vbuf_len = 65536*4*9;
		src->vbuf = memorycreate(src->vbuf_len, 0);
		if(0 == src->vbuf)return -1;

		src->vbuf_w = 4*3*3;
		src->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		src->vbuf_fmt = vbuffmt_333;
		src->vbuf_enq = 1;
	}

	return 0;
}
int ascii3d_vars(struct entity* win, int id, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct glsrc* src;
	int vlen,ilen,ret;
	if(0 == win)return -1;
	if(0 == win->gl_opaque)return -2;

	src = win->gl_opaque[font3d0 + id];
	if(0 == src){
		src = win->gl_opaque[font3d0 + id] = memorycreate(0x200, 0);
		if(0 == src)return -3;
	}
	if(0 == src->vbuf){
		ret = aidfont_load();
		if(ret < 0)return -4;

		ret = aidfont_fill(src, id);
		if(ret < 0)return -5;
	}

	vlen = src->vbuf_h;
	ilen = src->ibuf_h;
	*vbuf = (src->vbuf) + (36*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carveascii_test(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = ascii3d_vars(win, 0, &vbuf, &ibuf, 4, 2);
	if(vlen < 0)return;

	vbuf[ 0] = vc[0];
	vbuf[ 1] = vc[1];
	vbuf[ 2] = vc[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = 0.0;
	vbuf[ 7] = 1.0;

	vbuf[ 9] = vc[0]+vr[0];
	vbuf[10] = vc[1]+vr[1];
	vbuf[11] = vc[2]+vr[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = 1.0;
	vbuf[16] = 1.0;

	vbuf[18] = vc[0]+vf[0];
	vbuf[19] = vc[1]+vf[1];
	vbuf[20] = vc[2]+vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = 0.0;
	vbuf[25] = 0.0;

	vbuf[27] = vc[0]+vr[0]+vf[0];
	vbuf[28] = vc[1]+vr[1]+vf[1];
	vbuf[29] = vc[2]+vr[2]+vf[2];
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = 1.0;
	vbuf[34] = 0.0;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+3;
	ibuf[3] = vlen+0;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}
void carveascii(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8 dat)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = ascii3d_vars(win, 0, &vbuf, &ibuf, 4, 2);
	if(vlen < 0)return;

	vbuf[ 0] = vc[0];
	vbuf[ 1] = vc[1];
	vbuf[ 2] = vc[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (dat+0.0)/128.0;
	vbuf[ 7] = 1.0/128.0;

	vbuf[ 9] = vc[0]+vr[0];
	vbuf[10] = vc[1]+vr[1];
	vbuf[11] = vc[2]+vr[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = (dat+1.0)/128.0;
	vbuf[16] = 1.0/128.0;

	vbuf[18] = vc[0]+vf[0];
	vbuf[19] = vc[1]+vf[1];
	vbuf[20] = vc[2]+vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (dat+0.0)/128.0;
	vbuf[25] = 0.0;

	vbuf[27] = vc[0]+vr[0]+vf[0];
	vbuf[28] = vc[1]+vr[1]+vf[1];
	vbuf[29] = vc[2]+vr[2]+vf[2];
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = (dat+1.0)/128.0;
	vbuf[34] = 0.0;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+3;
	ibuf[3] = vlen+0;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}
void carveascii_center(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8 dat)
{
	int j;
	vec3 tc,tr,tf;
	for(j=0;j<3;j++){
		tr[j] = vr[j]*2;
		tf[j] = vf[j]*2;
		tc[j] = vc[j] - vr[j]/2 - vf[j];
	}
	carveascii(win, rgb, tc, tr, tf, dat);
}




void carveunicode(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 unicode)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vvv = (unicode&0xffff)/0x4000;
	int vlen = ascii3d_vars(win, vvv, &vbuf, &ibuf, 4, 2);
	if(vlen < 0)return;

	unicode = unicode&0x3fff;
	vbuf[ 0] = vc[0];
	vbuf[ 1] = vc[1];
	vbuf[ 2] = vc[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (unicode&0x7f)/128.0;
	vbuf[ 7] = ((unicode>>7)+1)/128.0;

	vbuf[ 9] = vc[0]+vr[0];
	vbuf[10] = vc[1]+vr[1];
	vbuf[11] = vc[2]+vr[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = ((unicode&0x7f)+1)/128.0;
	vbuf[16] = ((unicode>>7)+1)/128.0;

	vbuf[18] = vc[0]+vf[0];
	vbuf[19] = vc[1]+vf[1];
	vbuf[20] = vc[2]+vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (unicode&0x7f)/128.0;
	vbuf[25] = (unicode>>7)/128.0;

	vbuf[27] = vc[0]+vr[0]+vf[0];
	vbuf[28] = vc[1]+vr[1]+vf[1];
	vbuf[29] = vc[2]+vr[2]+vf[2];
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = ((unicode&0x7f)+1)/128.0;
	vbuf[34] = (unicode>>7)/128.0;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+3;
	ibuf[3] = vlen+0;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}
void carveunicode_center(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 unicode)
{
	vec3 tc;
	tc[0] = vc[0] - vr[0]/2 - vf[0]/2;
	tc[1] = vc[1] - vr[1]/2 - vf[1]/2;
	tc[2] = vc[2] - vr[2]/2 - vf[2]/2;
	carveunicode(win, rgb, tc, vr, vf, unicode);
}




void carveutf8(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	u32 unicode;
	utf2unicode(buf, &unicode);
	carveunicode(win, rgb, vc, vr, vf, unicode);
}
void carveutf8_center(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	vec3 tc;
	tc[0] = vc[0] - vr[0]/2 - vf[0]/2;
	tc[1] = vc[1] - vr[1]/2 - vf[1]/2;
	tc[2] = vc[2] - vr[2]/2 - vf[2]/2;
	carveutf8(win, rgb, tc, vr, vf, buf, len);
}




void carvestring(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	int j,k;
	u32 unicode;
	vec3 tc;

	if(0 == buf)return;
	if(0 == len)
	{
		while(buf[len] > 0x20)len++;
	}
	else
	{
		for(j=0;j<len;j++)
		{
			if(buf[j] < 0x20){len = j;break;}
		}
	}
	if(len == 0)return;

	j = k = 0;
	tc[0] = vc[0];
	tc[1] = vc[1];
	tc[2] = vc[2];
	while(j < len)
	{
		k = utf2unicode(buf+j, &unicode);
		if(k > 1){
			carveunicode(win, rgb, tc, vr, vf, unicode);
			tc[0] += vr[0];
			tc[1] += vr[1];
			tc[2] += vr[2];
			j += k;
		}
		else{
			carveascii(win, rgb, tc, vr, vf, buf[j]);
			tc[0] += vr[0]/2;
			tc[1] += vr[1]/2;
			tc[2] += vr[2]/2;
			j++;
		}
	}
}
void carvestring_center(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	float dx;
	int j,k,cnt;
	vec3 tc,tr,tf;

	if(0 == buf)return;
	if(0 == len){
		while(buf[len] >= 0x20)len++;
		if(0 == len)return;

		cnt = len;
	}
	else{
		for(cnt=0;cnt<len;cnt++){
			if(buf[cnt] <= 0x20)break;
		}
	}
	dx = (len-cnt)/2.0;

	//eachsize = 2*vr/len
	tr[0] = 4*vr[0]/len;
	tr[1] = 4*vr[1]/len;
	tr[2] = 4*vr[2]/len;
	tf[0] = 2*vf[0];
	tf[1] = 2*vf[1];
	tf[2] = 2*vf[2];
	for(j=0;j<len;j++)
	{
		for(k=0;k<3;k++)tc[k] = vc[k] -vr[k]+tr[k]*(j+dx)/2 -vf[k];
		carveascii(win, rgb, tc, tr, tf, buf[j]);
	}
}




void carvedecimal(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 val)
{
	int j,len;
	u8 str[8];

	for(len=0;len<8;len++)
	{
		if(0 == val)break;
		str[len] = 0x30 + (val%10);
		val = val/10;
	}
	if(len == 0)
	{
		len = 1;
		str[0] = '0';
	}
	carvestring(win, rgb, vc, vr, vf, str, len);
}
void carvehexadecimal(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 val)
{
	int j,len;
	u8 str[8];

	for(len=0;len<8;len++)
	{
		if(0 == val)break;
		str[7-len] = 0x30 + (val&0xf);
		if(str[7-len] > 0x39)str[7-len] += 7;
		val = val>>4;
	}
	if(len == 0)
	{
		len = 1;
		str[7] = '0';
	}
	carvestring(win, rgb, vc, vr, vf, str+8-len, len);
}
void carvehex8_center(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 val)
{
	int j;
	vec3 tc,tr,tf;
	for(j=0;j<3;j++){
		tc[j] = vc[j]-vr[j]-vf[j];
		tr[j] = vr[j]*2;
		tf[j] = vf[j]*2;
	}
	carvehexadecimal(win,rgb,tc,tr,tf,val);
}




void carvefloat(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, float data)
{
	u8 mystr[100];
	float2decstr(data, mystr);
	carvestring(win, rgb, vc, vr, vf, mystr, 0);
}
void carvedouble(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, double data)
{
	u8 mystr[100];
	double2decstr(data, mystr);
	carvestring(win, rgb, vc, vr, vf, mystr, 0);
}




void carvetext2d(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf,
	u8* buf, int len)
{
}
void carvetext_reverse(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf,
	u8* buf, int len)
{
	int j,k,cnt;
	float n;
	vec3 tc;
	vec3 tr;
	vec3 tf;
	if(0 == buf)return;
	if(0 == len)return;

	n = 32.0 / vec3_getlen(vr);
	tr[0] = vr[0] * n;
	tr[1] = vr[1] * n;
	tr[2] = vr[2] * n;

	n = 32.0 / vec3_getlen(vf);
	tf[0] = vf[0] * n;
	tf[1] = vf[1] * n;
	tf[2] = vf[2] * n;

	tc[0] = vc[0] - vr[0] - vf[0];
	tc[1] = vc[1] - vr[1] - vf[1];
	tc[2] = vc[2] - vr[2] - vf[2];

	k = len;
	cnt = 0;
	for(j=len-1;j>=0;j--)
	{
		if(0 == j)
		{
			carvestring(win, rgb, tc, tr, tf, buf, k);
			break;
		}

		if('\n' == buf[j])
		{
			carvestring(win, rgb, tc, tr, tf, buf+j+1, k-j-1);
			tc[0] += tf[0];
			tc[1] += tf[1];
			tc[2] += tf[2];

			k = j;

			cnt += 1;
			if(cnt > 100)break;
		}

	}
}
