#include "libuser.h"
#define _std_  hex32('s','t','d', 0)
#define _bdc_  hex32('b','d','c', 0)
#define _step_ hex32('s','t','e','p')
void* arenalist(void*, int);
void* actorlist(void*, int);
int arenaactor(struct arena* root, struct arena* twig, struct actor* leaf, struct actor* temp);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);




void* allocarena();
void* allocstyle();
void* allocpinid();
void* vbonode_create(u64, u64);
int vbonode_start(struct arena* twig, void* tf, struct arena* root, void* rf);


#define COUNT 5
static u64 want[COUNT] = {
	hex64('o','v','e','r','v','i','e','w'),
	hex32('v','j','o','y'),
	hex32('v','k','b','d'),
	hex64('c','o','r','n','e','r', 0, 0),
	hex64('p','o','i','n','t','e','r', 0)
};
static u8 flag[COUNT] = {
	0,
	'#',
	'#',
	0,
	0
};
void other_create(struct arena* win, void* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<COUNT;j++)
	{
		act = actorcreate(want[j], 0);
		if(0 == act)continue;

		sty = allocstyle();
		if(0 == sty)continue;

		pin = allocpinid();
		if(0 == pin)continue;

		sty->vc[0] = 0;
		sty->vc[1] = 0;
		sty->vc[2] = 0;
		sty->uc[3] = flag[j];

		sty->vr[0] = 1.0;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 1.0;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 1.0;

		relationcreate(act, pin, _act_, 0, win, sty, _win_, _ui_);
	}
}
void aider_create(struct arena* win, void* str)
{
#define _aid_ hex32('a','i','d',0)
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	act = actorcreate(hex64('a','i','d','f','o','n','t', 0), 0);
	sty = allocstyle();
	pin = allocpinid();

	sty->uc[3] = '#';
	relationcreate(act, pin, _act_, 0, win, sty, _win_, _aid_);


	act = actorcreate(hex64('a','i','d','g','e','o','m', 0), 0);
	sty = allocstyle();
	pin = allocpinid();

	sty->uc[3] = '#';
	relationcreate(act, pin, _act_, 0, win, sty, _win_, _aid_);
}


void viewportvertex1(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

#define sin34 0.55919290
#define cos34 0.8290
	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] = -cos34;
	win->camera.vb[2] = -sin34;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = cos34;
	win->camera.vu[2] = sin34;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = sin34;
	win->camera.vn[2] =-cos34;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] =-2000.0 * sin34;
	win->camera.vc[2] = 2000.0 * cos34;
}
void viewportvertex2(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] = -1.0;
	win->camera.vb[2] = 0.0;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 1.0;
	win->camera.vu[2] = 0.0;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = 0.0;
	win->camera.vn[2] = -1.0;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] = 0.0;
	win->camera.vc[2] = 10000.0;
}




static int roletype = 0;
void role_delete()
{
}
void role_create()
{
	if(roletype)return;

	//+libhard0
	//devicecreate(_ahci_, 0);
	//devicecreate(_xhci_, 0);

	//+libhard1
	//drivercreate(_sata_, 0);
	//drivercreate(_usb_, 0);

	//+libsoft0
	//systemcreate(_uart_, "/dev/ptmx");
	//systemcreate(_uart_, "/dev/ttyACM0");

	//+libsoft1
	arterycreate(0,   "HACK://0.0.0.0:2222");
	arterycreate(0,   "QUIC://0.0.0.0:4444");
	arterycreate(0,    "SSH://0.0.0.0:2022");
	arterycreate(0, "TELNET://0.0.0.0:2023");
	arterycreate(0,   "HTTP://0.0.0.0:2080");
	arterycreate(0,    "TLS://0.0.0.0:2443");
	arterycreate(0,  "SERVE://0.0.0.0:2099");

	//+libuser0
	arenacreate(_std_, 0);
	arenacreate(_tray_, 0);

	struct arena* win = arenacreate(_win_,  0);
	struct arena* ctx = vbonode_create(_vbo_, _ui_);
	struct style* win_vp1 = allocstyle();
	struct pinid* ctx_pl1 = allocpinid();

	ctx->win = win->win;
	win_vp1->vc[0] = 0.0;
	win_vp1->vc[1] = 0.0;
	win_vp1->vq[0] = 1.0;
	win_vp1->vq[1] = 1.0;
	relationcreate(
		ctx, ctx_pl1, _win_, 0,
		win, win_vp1, _win_, _vp_);

	//+libuser1
	aider_create(ctx, 0);
	other_create(ctx, 0);
	vbonode_start(ctx, 0, win, 0);
}




int parsefmt(u8* buf, u8* str)
{
	int j,k;
	while(*str == 0x20)str += 1;

	for(j=0;j<8;j++){
		k = str[j];
		if((k >= '0') && (k <= '9')){
			buf[j] = k;
			continue;
		}
		if((k >= 'a') && (k <= 'z')){
			buf[j] = k;
			continue;
		}
		break;
	}
	for(;j<8;j++)buf[j] = 0;
	return 0;
}
int parsefloat(float* f, u8* str)
{
	double d;
	int j = decstr2double(str, &d);

	f[0] = d;
	return j;
}
int parsevec4(vec4 vec, u8* str)
{
	int j = 0, k;

first:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[0], str+j);

	for(k=j;k<j+16;j++){
		if(',' == str[k]){
			j = k+1;
			goto second;
		}
	}
	return 1;

second:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[1], str+j);

	for(k=j;k<j+16;j++){
		if(',' == str[k]){
			j = k+1;
			goto third;
		}
	}
	return 2;

third:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[2], str+j);

	for(k=j;k<j+16;j++){
		if(',' == str[k]){
			j = k+1;
			goto fourth;
		}
	}
	return 3;

fourth:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[3], str+j);
	return 4;
}




void role_test_arena(u8* buf, int len)
{
	//say("arena:\n%.*s\n", len, buf);

	u64 fmt;
	u64 file;
	struct arena* win;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {

			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				propdata = j+1;
				propname = str;
				str = -1;

				//say("propname = %.*s\n", j-propname, buf+propname);
				if(0 == ncmp(buf+propname, "fmt", 3)){
					parsefmt((void*)&fmt, buf+propdata);
					say("%llx\n", fmt);
				}
				if(0 == ncmp(buf+propname, "file", 4)){
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			say("actnode = %.*s\n", j-nodename, buf+nodename);
		}
		if('}' == k) {
			if(nodename >= 0){
				nodename = -1;

				win = arenacreate(fmt, 0);
				say("@%llx\n", win);
			}//if innode

			fmt = 0;
			file = 0;
		}//if }
	}//for
}
void role_test_actor(u8* buf, int len)
{
	//say("actor:\n%.*s\n", len, buf);
	u64 fmt;
	u64 file;
	struct actor* act;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {

			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				propdata = j+1;
				propname = str;
				str = -1;

				//say("propname = %.*s\n", j-propname, buf+propname);
				if(0 == ncmp(buf+propname, "fmt", 3)){
					parsefmt((void*)&fmt, buf+propdata);
					say("%llx\n", fmt);
				}
				if(0 == ncmp(buf+propname, "file", 4)){
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			say("actnode = %.*s\n", j-nodename, buf+nodename);
		}
		if('}' == k) {
			if(nodename >= 0){
				nodename = -1;

				act = actorcreate(fmt, 0);
				say("@%llx\n", act);
			}//if innode

			fmt = 0;
			file = 0;
		}//if }
	}//for
}
void role_test_style(u8* buf, int len)
{
	//say("style:\n%.*s\n", len, buf);
	struct style* sty;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
/*
			if(propdata >= 0) {
				say("propctx = %.*s\n", j-propdata, buf+propdata);
				propname = propdata = -1;
			}
*/
			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				//say("propname = %.*s\n", j-str, buf+str);
				propdata = j+1;
				propname = str;
				str = -1;

				switch(buf[propname]){
					case 'l':parsevec4(sty->vl, buf+propdata);break;
					case 'r':parsevec4(sty->vr, buf+propdata);break;
					case 'b':parsevec4(sty->vb, buf+propdata);break;
					case 'u':parsevec4(sty->vu, buf+propdata);break;
					case 'n':parsevec4(sty->vn, buf+propdata);break;
					case 'f':parsevec4(sty->vf, buf+propdata);break;
					case 'q':parsevec4(sty->vq, buf+propdata);break;
					case 'c':parsevec4(sty->vc, buf+propdata);break;
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			sty = allocstyle();
			say("stynode = %.*s, @%llx\n", j-nodename, buf+nodename, sty);
		}
		if('}' == k) {
			if(nodename >= 0){
				nodename = -1;
/*
				say("l:%f, %f, %f\n", sty->vl[0], sty->vl[1], sty->vl[2]);
				say("r:%f, %f, %f\n", sty->vr[0], sty->vr[1], sty->vr[2]);
				say("b:%f, %f, %f\n", sty->vb[0], sty->vb[1], sty->vb[2]);
				say("u:%f, %f, %f\n", sty->vu[0], sty->vu[1], sty->vu[2]);
				say("n:%f, %f, %f\n", sty->vn[0], sty->vn[1], sty->vn[2]);
				say("f:%f, %f, %f\n", sty->vf[0], sty->vf[1], sty->vf[2]);
				say("q:%f, %f, %f\n", sty->vq[0], sty->vq[1], sty->vq[2]);
				say("c:%f, %f, %f\n", sty->vc[0], sty->vc[1], sty->vc[2]);
*/
			}//if innode
		}//if }
	}//for
}

void role_test_pinid(u8* buf, int len)
{
	//say("pinid:\n%.*s\n", len, buf);
	struct pinid* pin;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				say("propname = %.*s\n", j-str, buf+str);
				propdata = j+1;
				propname = str;
				str = -1;
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			pin = allocpinid();
			say("pinnode = %.*s, @%llx\n", j-nodename, buf+nodename, pin);
		}
		if('}' == k) {
			if(nodename >= 0) {
				nodename = -1;
			}
		}
	}//for
}

void role_test_relation(u8* buf, int len)
{
	//say("relation:\n%.*s\n", len, buf);
	
	int j,k;
	int wirellll = -1;
	int wirerrrr = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			wirellll = wirerrrr = -1;
			continue;
		}

		//(src) -> (dst)
		if('(' == k) {
			if(wirellll < 0) {
				wirellll = j+1;
			}
			else {
				wirerrrr = j+1;
			}
		}
		if(')' == k) {
			if(wirerrrr >= 0) {
				say("r(%.*s)\n", j-wirerrrr, buf+wirerrrr);
				wirellll = wirerrrr = -1;
			}
			else if(wirellll >= 0) {
				say("l(%.*s) to ", j-wirellll, buf+wirellll);
			}
		}
	}//for
}
void role_test1(u8* buf, int len)
{
	int j,k;

	int head = 0;
	int line = 1;

	int typename = -1;
	int typedata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			//say("%04d: %.*s\n", l, j-head, buf+head);

			head = j+1;
			line += 1;
			continue;
		}

		//<typename>...</typename>
		if('-' == k) {
			if('>' == buf[j+1]) {
				j++;
				continue;
			}
		}
		if('<' == k) {
			if(typename < 0) {
				typename = j+1;
			}
			else {
				if(0 == ncmp(buf+typename, "arena", 5)) {
					role_test_arena(buf + typedata, j-typedata);
				}
				if(0 == ncmp(buf+typename, "actor", 5)) {
					role_test_actor(buf + typedata, j-typedata);
				}
				if(0 == ncmp(buf+typename, "style", 5)) {
					role_test_style(buf + typedata, j-typedata);
				}
				if(0 == ncmp(buf+typename, "pinid", 5)) {
					role_test_pinid(buf + typedata, j-typedata);
				}
				if(0 == ncmp(buf+typename, "relation", 8)) {
					role_test_relation(buf + typedata, j-typedata);
				}
				typename = -1;
			}
		}
		if('>' == k) {
			if(typename < 0)continue;

			////say("typename = %.*s\n", j-typename, buf+typename);
			typedata = j+1;
		}

	}//for
}
void role_test(int argc, u8** argv)
{
	int j,len;
	char* str;
	u8 buf[0x1000];

	if(argc >= 2)str = (char*)argv[1];
	else str = "datafile/mainwindow.ml";

	len = openreadclose(str, 0, buf, 0x1000);
	if(len <= 0)return;
	//say("%s", buf);

	role_test1(buf, len);
}
void role_toycar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_bdc_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);
}
void role_stepcar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_step_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);
}
void role_control(int argc, u8** argv)
{
}
void role_uarthelp(int argc, u8** argv)
{
	void* sys;
	void* act;
	void* win;
	if(argc < 2)return;

	sys = systemcreate(_uart_, argv[1]);
	if(0 == sys)return;

	act = actorcreate(_std_, 0);
	if(act)relationcreate(act, 0, _act_, 0, sys, 0, _fd_, 0);

	win = arenalist("std", 0);
	if(win)relationcreate(act, 0, _act_, 0, win, 0, _win_, 0);

	win = arenacreate(_win_, 0);
	if(win)arenaactor(win, 0, act, 0);
}
void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	argc = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(argc < 2)return;

	if(0 == ncmp(argv[1], "test", 4))
	{
		role_test(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "toycar", 6))
	{
		roletype = 1;
		role_toycar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "stepcar", 6))
	{
		roletype = 1;
		role_stepcar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "control", 7))
	{
		roletype = 2;
		role_control(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "uart", 4))
	{
		roletype = 3;
		role_uarthelp(argc-1, &argv[1]);
	}
}
