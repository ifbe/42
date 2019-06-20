#include "libuser.h"
int arenaactor(struct arena* root, struct arena* twig, struct actor* leaf, struct actor* temp);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
//
int role_fromfile(int, u8**);
int role_default(int, u8**);




void prep()
{
	int ret;
	u8* argv[2];

	//cmdline
	arenacreate(_std_, 0);
	arenacreate(_tray_, 0);
/*
	//try load from file
	ret = role_fromfile(0, argv);
	if(ret > 0)return;

	//at least one window
	ret = role_default(0, argv);
*/
}
void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[256];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	argc = str2arg(buf, len, tmp, 256, argv, 8);
	if(argc <= 1){
		role_fromfile(1, argv);
		return;
	}

	//builtin
	if(0 == ncmp(argv[1], "builtin", 7)){

	}

	//myml
	else{
		role_fromfile(argc, argv);
	}

	//json

	//cfg

	//default
}
