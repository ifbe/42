#include "actor.h"




void carvearrorkey2d(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf,
	u8* buf, int t)
{
	u32 c;
	vec3 tc;
	vec3 tr;
	vec3 tf;

	carvesolid2d_circle(win, 0x404040, vc, vr, vf);
	tr[0] = vr[0]/3;
	tr[1] = vr[1]/3;
	tr[2] = vr[2]/3;
	tf[0] = vf[0]/3;
	tf[1] = vf[1]/3;
	tf[2] = vf[2]/3;

	//x-: left
	if(buf[0]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0]-vr[0]*2/3;
	tc[1] = vc[1]-vr[1]*2/3;
	tc[2] = vc[2]-vr[2]*2/3-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[0]&0x7f);
		

	//x+: right
	if(buf[1]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0]+vr[0]*2/3;
	tc[1] = vc[1]+vr[1]*2/3;
	tc[2] = vc[2]+vr[2]*2/3-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[1]&0x7f);

	//y-: near
	if(buf[2]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0]-vf[0]*2/3;
	tc[1] = vc[1]-vf[1]*2/3;
	tc[2] = vc[2]-vf[2]*2/3-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[2]&0x7f);

	//y+: far
	if(buf[3]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0]+vf[0]*2/3;
	tc[1] = vc[1]+vf[1]*2/3;
	tc[2] = vc[2]+vf[2]*2/3-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[3]&0x7f);

	//z-: trigger
	if(buf[4]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0]-vr[0]*2/3+vf[0]*4/3;
	tc[1] = vc[1]-vr[1]*2/3+vf[1]*4/3;
	tc[2] = vc[2]-vr[2]*2/3+vf[2]*4/3-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[4]&0x7f);

	//z+: bumper
	if(buf[5]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0]+vr[0]*2/3+vf[0]*4/3;
	tc[1] = vc[1]+vr[1]*2/3+vf[1]*4/3;
	tc[2] = vc[2]+vr[2]*2/3+vf[2]*4/3-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[5]&0x7f);

	//press
	if(buf[6]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0];
	tc[1] = vc[1];
	tc[2] = vc[2]-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[6]&0x7f);

	//select or start
	if(buf[7]&0x80)c = 0xff00ff;
	else c = 0x808080;
	tc[0] = vc[0]+vr[0]*t*4/3+vf[0]*2/3;
	tc[1] = vc[1]+vr[1]*t*4/3+vf[1]*2/3;
	tc[2] = vc[2]+vr[2]*t*4/3+vf[2]*2/3-0.1;
	carvesolid2d_circle(win, c, tc, tr, tf);
	tc[0] += vr[0]/6;
	tc[1] += vr[1]/6;
	tc[2] += vr[2]/6-0.1;
	carve2d_ascii(win, 0xffffff, tc, tr, tf, buf[7]&0x7f);
}