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
void* vbonode_create(u64, u64);
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
void* hostviewport_create(struct arena* window)
{
	struct arena* viewport;

	viewport = allocarena();
	if(0 == viewport)return 0;

	viewport->type = viewport->fmt = hex32('v','p',0,0);

	return viewport;
}
void hostwindow_create(struct arena* window)
{
	struct arena* uiviewport;
	struct style* uistyle;
	struct arena* world2;

	uiviewport = hostviewport_create(window);
	if(uiviewport)
	{
		uistyle = allocstyle();
		uistyle->vc[0] = 0.0;
		uistyle->vc[1] = 0.0;
		uistyle->vq[0] = 1.0;
		uistyle->vq[1] = 1.0;

		viewportvertex2(uiviewport);
		relationcreate(uiviewport, 0, _win_, window, uistyle, _win_);
	}

	world2 = vbonode_create(_vbo_, _ui_);
	if(world2){
		world2->win = window->win;
		relationcreate(world2, 0, _win_, uiviewport, 0, _win_);
	}
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

	hostwindow_create(win);

	//+libuser1
	//actorcreate(_2048_, 0);
}




void role_test1(u8* buf, int len)
{
	int j,k;
	int str = -1;

	int head = 0;
	int line = 1;

	int typename = -1;
	int typedata = -1;
	int nodename = -1;
	int nodedata = -1;
	int propname = -1;
	int propdata = -1;
	int wirellll = -1;
	int wirerrrr = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			//say("%04d: %.*s\n", l, j-head, buf+head);

			if(propdata >= 0) {
				say("propctx = %.*s\n", j-propdata, buf+propdata);
				propname = propdata = -1;
			}

			head = j+1;
			line += 1;

			wirellll = wirerrrr = -1;

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
			if( (typename >=0) && (nodename >= 0) && (str >= 0) ) {
				//fillstyle(.addr, buf+j+1)
				say("propname = %.*s\n", j-str, buf+str);
				propdata = j+1;
				propname = str;
				str = -1;
			}
			continue;
		}

		//(src) -> (dst)
		if('-' == k) {
			if('>' == buf[j+1]) {
				j++;
				continue;
			}
		}
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

		//nodename{...}
		if('{' == k) {
			if(typename >= 0) {
				nodename = str;
				nodedata = j+1;
				say("nodename = %.*s\n", j-nodename, buf+nodename);
				str = -1;
				//.name = str;
				//.addr = allocstyle();
			}
		}
		if('}' == k) {
			if(typename >= 0) {
				if(nodename >= 0) {
					//say("[%.*s]\n", j-in22, buf+in22);

					if(propdata >= 0) {
						say("propctx = %.*s\n", j-propdata, buf+propdata);
						propname = propdata = -1;
					}

					nodename = -1;
				}
			}
		}

		//<typename>...</typename>
		if('<' == k) {
			if(typename < 0) {
				typename = j+1;
			}
			else {
				typename = -1;
			}
		}
		if('>' == k) {
			if(typename >= 0) {
				say("typename = %.*s\n", j-typename, buf+typename);
				typedata = j+1;
			}
		}

	}//for
}
void role_test(int argc, u8** argv)
{
	int j,len;
	char* str;
	u8 buf[0x1000];

	if(argc >= 2)str = argv[1];
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
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);
}
void role_stepcar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_step_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);
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
	if(act)relationcreate(act, 0, _act_, sys, 0, _fd_);

	win = arenalist("std", 0);
	if(win)relationcreate(act, 0, _act_, win, 0, _win_);

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
