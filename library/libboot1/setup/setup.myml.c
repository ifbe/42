#include "libuser.h"
void* style_alloc();
void* style_recycle();
//
int copypath(u8* path, u8* data);
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);
//
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
//
void* poller_alloc();
void poller(void*);




struct chiplist{
	u64 tier;
	u64 type;

	u64 hash;
	void* addr;
};
struct footlist{
	u64 hash;
	void* addr;
};




int parsefmt(u8* buf, u8* str)
{
	int j,k;
	while(*str == 0x20)str += 1;

	for(j=0;j<8;j++){
		k = str[j];
		if( ((k >= '0') && (k <= '9')) |
			((k >= 'A') && (k <= 'Z')) |
			((k >= 'a') && (k <= 'z')) |
			('.' == k) |
			('_' == k) )
		{
			buf[j] = k;
			continue;
		}
		break;
	}
	for(;j<8;j++)buf[j] = 0;
	return 0;
}
int parsenameandtype(u8* buf, int len, u8* name, u8* type)
{
	//logtoall("parsenameandtype:%.16s\n",buf);
	parsefmt(name, buf);

	int j;
	for(j=1;j<16;j++){
		if(':' == buf[j]){
			parsefmt(type, buf+j+1);
			break;
		}
	}
	return 0;
}




void parsehalfrel_old(u8* buf, int len,
	struct chiplist chip[], int clen, struct footlist foot[], int flen,
	struct halfrel* rel)
{
	//window, leftpart, win, vp
	int j,k;
	int c[4];
	u64 hash;
	u8* addr = (u8*)&hash;

	k = 0;
	for(j=0;j<len;j++){
		if(',' == buf[j]){
			c[k] = j+1;
			k++;
			if(k >= 3)return;
		}
	}
	if(k != 2){
		logtoall("error@parsehalfrel,k=%d\n",k);
		return;
	}

	//thechip
	j = 0;
	while(0x20 == buf[j])j++;
	parsefmt(addr, buf+j);

	for(k=0;k<clen;k++){
		if(hash == chip[k].hash){
			rel->pchip = chip[k].addr;
			rel->nodetype = chip[k].tier;
		}
	}

	//thefoot
	j = c[0];
	while(0x20 == buf[j])j++;
	parsefmt(addr, buf+j);

	for(k=0;k<flen;k++){
		if(hash == foot[k].hash){
			rel->foot = (u64)foot[k].addr;
		}
	}
/*
	//thetype
	j = c[1];
	while(0x20 == buf[j])j++;
	parsefmt(addr, buf+j);
	rel->nodetype = hash;
*/
	//theflag
	j = c[1];
	while(0x20 == buf[j])j++;
	parsefmt(addr, buf+j);
	rel->foottype = hash;
}
void parsehalfrel(u8* buf, int len,
	struct chiplist chip[], int clen, struct footlist foot[], int flen,
	struct halfrel* rel)
{
	int j;
	int cntcol=0;
	int findcol[4];
	for(j=0;j<len;j++){
		if(',' == buf[j]){
			findcol[cntcol] = j;
			cntcol++;
		}
	}
	if(cntcol > 1){
		parsehalfrel_old(buf,len, chip,clen, foot,flen, rel);
		return;
	}

	u64 hash;
	int lat=-1;
	int rat=-1;
	for(j=0;j<findcol[0];j++){
		if('@' == buf[j]){
			lat = j;
			break;
		}
	}
	for(j=findcol[0];j<len;j++){
		if('@' == buf[j]){
			rat = j;
			break;
		}
	}

	//chip.type
	j = 0;
	while(0x20 == buf[j])j++;
	parsefmt((u8*)&hash, buf+j);
	rel->nodetype = hash;
	logtoall("nodetype=%.8s\n", &rel->nodetype);

	//chip.addr
	if(lat >= 0){
		j = lat+1;
		while(0x20 == buf[j])j++;
		parsefmt((u8*)&hash, buf+j);

		for(j=0;j<clen;j++){
			if(hash == chip[j].hash){
				rel->pchip = chip[j].addr;
				if(0==rel->nodetype)rel->nodetype = chip[j].tier;
				break;
			}
		}
	}
	else{
		logtoall("no @ at left?\n");
	}
	logtoall("nodeaddr=%p\n", rel->pchip);

	//foot.type
	j = findcol[0]+1;
	while(0x20 == buf[j])j++;
	parsefmt((u8*)&hash, buf+j);
	rel->foottype = hash;
	logtoall("foottype=%.8s\n", &rel->foottype);

	if(rat >= 0){
		j = rat+1;
		while(0x20 == buf[j])j++;
		parsefmt((u8*)&hash, buf+j);

		for(j=0;j<flen;j++){
			if(hash == foot[j].hash){
				rel->pfoot = foot[j].addr;
				break;
			}
		}
	}
	logtoall("footaddr=%p\n", rel->pfoot);
}
void role_test_relation(
	struct chiplist chip[], int clen,
	struct footlist foot[], int flen,
	u8* buf, int len)
{
	//logtoall("relation:\n%.*s\n", len, buf);
	struct halfrel src;
	struct halfrel dst;
	struct relation* rel;

	int run,val,tmp;
	int bracket_l = -1;
	int bracket_r = -1;

	dst.chip = src.chip = 0;
	for(run=0;run<=len;run++) {

		val = buf[run];
		if( (run == len) | ('\n' == val) ) {
			if((0 != dst.chip) && (0 != src.chip))
			{
				logtoall("%.4s@%llx,%.4s@%llx -> %.4s@%llx,%.4s@%llx\n",
					&src.nodetype,src.chip, &src.foottype,src.foot,
					&dst.nodetype,dst.chip, &dst.foottype,dst.foot
				);
				rel = relationcreate(
					(void*)dst.chip, (void*)dst.foot, dst.nodetype, dst.foottype,
					(void*)src.chip, (void*)src.foot, src.nodetype, src.foottype
				);
				relationattach((void*)&rel->srcchip, (void*)&rel->dstchip);
				logtoall("\n");
			}
//logtoall("***\n");

			dst.chip = src.chip = 0;
			bracket_l = bracket_r = -1;
			continue;
		}

		if('#' == val){
			tmp = run;
			while('\n' != buf[tmp])tmp++;
			logtoall("[%x,%x]ignore note: (%.*s)\n", run, tmp, tmp-run, buf+run);
			run = tmp;
			continue;
		}
//logtoall("@%c@\n",k);
		//(src) -> (dst)
		if('(' == val) {
			if(bracket_l < 0) {
				bracket_l = run+1;
			}
			else {
				bracket_r = run+1;
			}
		}
		if(')' == val) {
			if(bracket_r >= 0) {
				logtoall("[%x,%x)rrel: <%.*s>\n", bracket_r, run, run-bracket_r, buf+bracket_r);

				dst.chip = dst.foot = 0;
				dst.nodetype = dst.foottype = 0;
				parsehalfrel(buf+bracket_r, run-bracket_r,
					chip, clen, foot, flen,
					&dst);
			}
			else if(bracket_l >= 0) {
				logtoall("[%x,%x)lrel: <%.*s>\n", bracket_l, run, run-bracket_l, buf+bracket_l);

				src.chip = src.foot = 0;
				src.nodetype = src.foottype = 0;
				parsehalfrel(buf+bracket_l, run-bracket_l,
					chip, clen, foot, flen,
					&src);
			}
		}
	}//for
}




int role_test_node(u64 tier, int aaa, struct chiplist chip[], int clen, u8* buf, int len)
{
	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	int argc = 0;
	u8* argv[16];

	u8* arg = 0;
	u8 tmp[128];

	u64 hash = 0;

	u64 type = 0;
	u64 fmt = 0;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			str = -1;
			continue;
		}

		if('#' == k){
			while('\n' != buf[j])j++;
			str = -1;
			k = buf[j];
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'A') && (k <= 'Z')) |
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

				argv[argc] = buf+propname;
				argc += 1;

				//logtoall("propname = %.*s\n", j-propname, buf+propname);
				if(0 == ncmp(buf+propname, "fmt", 3)){
					parsefmt((void*)&type, buf+propdata);
					//logtoall("%llx\n", type);
				}
				if(0 == ncmp(buf+propname, "arg", 3)){
					copypath(tmp, buf+propdata);
					arg = tmp;
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			logtoall("nodestr=%.*s\n", j-nodename, buf+nodename);
			parsenameandtype(buf+nodename, j-nodename, (void*)&hash, (void*)&type);
			logtoall("name=%.8s,type=%.8s\n", &hash, &type);
		}
		if('}' == k) {
			if(nodename >= 0){
				//logtoall("haha:%llx,%llx\n", fmt, arg);

				chip[clen].tier = tier;		//_ent_ _sup_ _art_ _sys_ _dri_ _dev_ _wrk_
				chip[clen].type = type;
				chip[clen].hash = hash;
				switch(tier){
					case _ent_:
						chip[clen].addr = entity_alloc_fromtype(type);
						if(chip[clen].addr)entity_create(chip[clen].addr, arg, argc, argv);
						break;
					case _sup_:
						chip[clen].addr = supply_alloc_fromtype(type);
						if(chip[clen].addr)supply_create(chip[clen].addr, arg, argc, argv);
						break;
					case _art_:
						chip[clen].addr = artery_alloc_fromtype(type);
						if(chip[clen].addr)artery_create(chip[clen].addr, arg, argc, argv);
						break;
					case _sys_:
						chip[clen].addr = system_alloc_frompath(type, arg);
						if(chip[clen].addr)system_create(chip[clen].addr, arg, argc, argv);
						break;
					case _dri_:
						chip[clen].addr = driver_alloc_fromtype(type);
						if(chip[clen].addr)driver_create(chip[clen].addr, arg, argc, argv);
						break;
					case _dev_:
						chip[clen].addr = device_alloc_frompath(type, arg);
						if(chip[clen].addr)device_create(chip[clen].addr, arg, argc, argv);
						break;
					case _wrk_:
						chip[clen].addr = bootup_alloc_fromtype(type);
						if(chip[clen].addr)bootup_create(chip[clen].addr, arg, argc, argv);
						break;
				}
				logtoall("node%d:tier=%.8s,type=%.8s,hash=%.8s,addr=%p\n\n",clen, &tier,&type,&hash,chip[clen].addr);

				nodename = -1;
				clen += 1;

				argc = 0;
				arg = 0;
			}//if innode

			type = 0;
		}//if }
	}//for

	return clen;
}




int role_test_style(struct footlist foot[], int flen, u8* buf, int len)
{
	//logtoall("style:\n%.*s\n", len, buf);
	u64 hash;
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
				logtoall("propctx = %.*s\n", j-propdata, buf+propdata);
				propname = propdata = -1;
			}
*/
			str = -1;
			continue;
		}

		if('#' == k){
			while('\n' != buf[j])j++;
			j++;
			k = buf[j];
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'A') && (k <= 'Z')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				//logtoall("propname = %.*s\n", j-str, buf+str);
				propdata = j+1;
				propname = str;
				str = -1;

				switch(buf[propname]){
					case 'l':parsefv(sty->fshape.vl, 4, buf+propdata, 99);break;
					case 'r':parsefv(sty->fshape.vr, 4, buf+propdata, 99);break;
					case 'b':parsefv(sty->fshape.vb, 4, buf+propdata, 99);break;
					case 't':parsefv(sty->fshape.vt, 4, buf+propdata, 99);break;
					case 'n':parsefv(sty->fshape.vn, 4, buf+propdata, 99);break;
					case 'f':parsefv(sty->fshape.vf, 4, buf+propdata, 99);break;
					case 'q':parsefv(sty->fshape.vq, 4, buf+propdata, 99);break;
					case 'c':parsefv(sty->fshape.vc, 4, buf+propdata, 99);break;

					case 'L':parsefv(sty->frustum.vl, 4, buf+propdata, 99);break;
					case 'R':parsefv(sty->frustum.vr, 4, buf+propdata, 99);break;
					case 'B':parsefv(sty->frustum.vb, 4, buf+propdata, 99);break;
					case 'T':parsefv(sty->frustum.vt, 4, buf+propdata, 99);break;
					case 'N':parsefv(sty->frustum.vn, 4, buf+propdata, 99);break;
					case 'F':parsefv(sty->frustum.vf, 4, buf+propdata, 99);break;
					case 'Q':parsefv(sty->frustum.vq, 4, buf+propdata, 99);break;
					case 'C':parsefv(sty->frustum.vc, 4, buf+propdata, 99);break;

					case 'x':parsefv(sty->fmotion.displace_x, 4, buf+propdata, 99);break;
					case 'v':parsefv(sty->fmotion.displace_v, 4, buf+propdata, 99);break;
					case 'a':parsefv(sty->fmotion.displace_a, 4, buf+propdata, 99);break;
					case 'j':parsefv(sty->fmotion.displace_j, 4, buf+propdata, 99);break;
					case '0':parsefv(sty->fmotion.angular_x, 4, buf+propdata, 99);break;
					case '1':parsefv(sty->fmotion.angular_v, 4, buf+propdata, 99);break;
					case '2':parsefv(sty->fmotion.angular_a, 4, buf+propdata, 99);break;
					case '3':parsefv(sty->fmotion.angular_j, 4, buf+propdata, 99);break;
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			//logtoall("stynode=%.*s\n", j-nodename, buf+nodename);

			parsefmt((void*)&hash, buf+nodename);

			sty = style_alloc();
		}
		if('}' == k) {
			if(nodename >= 0){
				nodename = -1;
/*
				logtoall("l:%f, %f, %f\n", sty->vl[0], sty->vl[1], sty->vl[2]);
				logtoall("r:%f, %f, %f\n", sty->vr[0], sty->vr[1], sty->vr[2]);
				logtoall("b:%f, %f, %f\n", sty->vb[0], sty->vb[1], sty->vb[2]);
				logtoall("u:%f, %f, %f\n", sty->vu[0], sty->vu[1], sty->vu[2]);
				logtoall("n:%f, %f, %f\n", sty->vn[0], sty->vn[1], sty->vn[2]);
				logtoall("f:%f, %f, %f\n", sty->vf[0], sty->vf[1], sty->vf[2]);
				logtoall("q:%f, %f, %f\n", sty->vq[0], sty->vq[1], sty->vq[2]);
				logtoall("c:%f, %f, %f\n", sty->vc[0], sty->vc[1], sty->vc[2]);
*/
				foot[flen].hash = hash;
				foot[flen].addr = sty;
				logtoall("slot%d:hash=%.8s,addr=%p\n",flen,&hash,sty);

				flen += 1;
			}//if innode
		}//if }
	}//for

	return flen;
}
void role_test1(u8* buf, int len)
{
	int j,k;

	int head = 0;
	int line = 1;

	int typename = -1;
	int typedata = -1;

	int clen = 0;
	struct chiplist cbuf[80];

	int flen = 0;
	struct footlist fbuf[80];

	logtoall("parsing myml\n");
	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			//logtoall("%04d: %.*s\n", line, j-head, buf+head);

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
		if('>' == k) {
			if(typename < 0)continue;

			////logtoall("typename = %.*s\n", j-typename, buf+typename);
			typedata = j+1;
		}
		if('<' == k) {
			if(typename < 0) {
				typename = j+1;
			}
			else {
				if(0 == ncmp(buf+typename, "bootup", 6)) {
					clen = role_test_node(
						_wrk_, 0, cbuf, clen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "device", 6)) {
					clen = role_test_node(
						_dev_, 0, cbuf, clen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "driver", 6)) {
					clen = role_test_node(
						_dri_, 0, cbuf, clen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "system", 6)) {
					clen = role_test_node(
						_sys_, 0, cbuf, clen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "artery", 6)) {
					clen = role_test_node(
						_art_, 0, cbuf, clen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "supply", 6)) {
					clen = role_test_node(
						_sup_, 0, cbuf, clen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "entity", 6)) {
					clen = role_test_node(
						_ent_, 0, cbuf, clen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "style", 5)) {
					flen = role_test_style(
						fbuf, flen,
						buf + typedata, j-typedata
					);
				}
				if(0 == ncmp(buf+typename, "relation", 8)) {
					role_test_relation(
						cbuf, clen,
						fbuf, flen,
						buf + typedata, j-typedata
					);
				}
				typename = -1;
			}
		}
	}//for
/*
	for(j=0;j<clen;j++)
	{
		if(0 == cbuf[j].addr)break;
		logtoall("%.8s/%.8s: %.8s@%llx\n",
			&cbuf[j].tier, &cbuf[j].type,
			&cbuf[j].hash, cbuf[j].addr
		);
	}

	for(j=0;j<flen;j++)
	{
		if(0 == fbuf[j].addr)break;
		logtoall("foot: %.8s@%llx\n",
			&fbuf[j].hash, fbuf[j].addr
		);
	}
*/
}
int role_fromfile(u8* str, int len)
{
	u8 buf[0x2000];

	len = openreadclose(str, 0, buf, 0x2000);
	if(len <= 0)return 0;
	//logtoall("%s", buf);

    logtoall("----read done, now parse----\n");

	role_test1(buf, len);
	return 1;
}




int myml_create(struct item* obj, void* arg, int argc, u8** argv)
{
	int j;
	if(arg){
		role_fromfile(arg, 0);
	}
	else if(0 == argv){
		role_fromfile((u8*)"datafile/myml/index.myml", 0);
	}
	else{
		for(j=1;j<argc;j++){
			logtoall("arg[%d]=%s\n", j, argv[j]);
			role_fromfile(argv[j], 0);
		}
	}

    logtoall("----parse done, now loop or exit----\n");

	void* mpoller = poller_alloc();
	if(mpoller)poller(mpoller);

	logtoall("myml@%p exiting\n",obj);
	return 0;
}
int myml_delete(struct item* obj)
{
	logtoall("myml_delete:%p\n", obj);
	return 0;
}



void myml_init()
{
}
void myml_exit()
{
}
