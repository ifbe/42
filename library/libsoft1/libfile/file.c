#include "libsoft.h"
int mbrclient_start(struct object* obj, void* of, struct element* ele, void* ef, u8* buf, int len);
int gptclient_start(struct object* obj, void* of, struct element* ele, void* ef, u8* buf, int len);




//compress
#define _7z_ hex32('7','z',0,0)
int check_7z(void*);
int parse_7z(void*);
#define _cpio_ hex32('c','p','i','o')
int check_cpio(void*);
int parse_cpio(void*);
#define _gz_ hex32('g','z',0,0)
int check_gz(void*);
int parse_gz(void*);
#define _tar_ hex32('t','a','r',0)
int check_tar(void*);
int parse_tar(void*);
#define _zip_ hex32('z','i','p',0)
int check_zip(void*);
int parse_zip(void*);
//diskimage
#define _dmg_ hex32('c','p','i','o')
int check_dmg(void*);
int parse_dmg(void*);
#define _qcow2_ hex32('q','c','2',0)
int check_qcow2(void*);
int parse_qcow2(void*);
#define _vdi_ hex32('v','d','i',0)
int check_vdi(void*);
int parse_vdi(void*);
#define _vhd_ hex32('v','h','d',0)
int check_vhd(void*);
int parse_vhd(void*);
#define _vmdk_ hex32('v','m','d','k')
int check_vmdk(void*);
int parse_vmdk(void*);
//executable
#define _elf_ hex32('e','l','f',0)
int check_elf(void*);
int parse_elf(void*);
#define _macho_ hex32('m','a','c','o')
int check_macho(void*);
int parse_macho(void*);
#define _pe_ hex32('p','e',0,0)
int check_pe(void*);
int parse_pe(void*);
//filesystem
#define _ext_ hex32('e','x','t',0)
int check_ext(void*);
int parse_ext(void*, void*);
#define _fat_ hex32('f','a','t',0)
int check_fat(void*);
int parse_fat(void*, void*);
#define _hfs_ hex32('h','f','s',0)
int check_hfs(void*);
int parse_hfs(void*, void*);
#define _ntfs_ hex32('n','t','f','s')
int check_ntfs(void*);
int parse_ntfs(void*, void*);
//model
#define _3mf_ hex32('3','m','f',0)
int check_3mf(void*);
int parse_3mf(void*);
#define _dwg_ hex32('d','w','g',0)
int check_dwg(void*);
int parse_dwg(void*);
#define _stl_ hex32('s','t','l',0)
int check_stl(void*);
int parse_stl(void*);
//parttable
#define _applept_ hex32('a','p','t',0)
int check_applept(void*);
int parse_applept(void*, void*);
#define _gpt_ hex32('g','p','t',0)
int check_gpt(void*);
int parse_gpt(void*, void*);
#define _mbr_ hex32('m','b','r',0)
int check_mbr(void*);
int parse_mbr(void*, void*);
//picture
#define _bmp_ hex32('b','m','p',0)
int check_bmp(void*);
int parse_bmp(void*, int);
#define _flif_ hex32('f','l','i','f')
int check_flif(void*);
int parse_flif(void*, int);
#define _jpg_ hex32('j','p','g',0)
int check_jpg(void*);
int parse_jpg(void*, int);
#define _png_ hex32('p','n','g',0)
int check_png(void*);
int parse_png(void*, int);
#define _webp_ hex32('w','e','b','p')
int check_webp(void*);
int parse_webp(void*, int);
//
int startfile(void*, int);
int stopfile(int);
int readfile(u64 file, u64 off, u8* mem, u64 len);
int writefile(u64 file, u64 off, u8* mem, u64 len);




int openreadclose(void* name, int off, void* buf, int len)
{
	int ret;
	int fd = startfile(name, 'r');
	if(fd <= 0)return fd;

	ret = readfile(fd, off, buf, len);

	stopfile(fd);
	return ret;
}
int openwriteclose(void* name, int off, void* buf, int len)
{
	int ret;
	int fd = startfile(name, 'w');
	if(fd <= 0)return fd;

	ret = writefile(fd, off, buf, len);

	stopfile(fd);
	return ret;
}



/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(设备这一小块是逻辑上的哪) -> (内存这一小块想要逻辑上的哪)
(扇区，数量，是哪) -> (内存，数量，要哪)
*/
void cleverread(
	u64 src, u64 count, u64 where,
	u8* buf, u64 len, u64 want)
{
	u8* rdi = 0;    //内存地址
	u64 rsi = 0;    //扇区号
	u64 rcx = 0;    //扇区数量

	//改rdi,rsi,rcx数值
	if(where<want)	     //3和4
	{
		rdi = buf;
		rsi = src+(want-where);
		if(where+count <= want+len)
		{
			rcx = count-(want-where);
		}
		else
		{
			rcx = len;
		}
	}
	else
	{
		rdi = buf+(where-want);
		rsi = src;
		if(where+count <= want+len)
		{
			rcx = count;
		}
		else
		{
			rcx = want+len-where;
		}
	}

/*
	say(
		"(%llx,%llx,%llx)->(%llx,%llx,%llx)\n",
		src,count,where,    buf,len,want
	);
	say(
		"rdi=%llx,rsi=%llx,rcx=%llx\n",
		rdi,rsi,rcx
	);
*/
	readfile(0, rsi, rdi, rcx);
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(内存这一小块是逻辑上的哪) -> (设备这一小块想要逻辑上的哪)
(来源，数量，是哪) -> (目的，数量，要哪)
*/
void cleverwrite(
	u8* buf, u64 len, u64 where,
	u64 dst, u64 size, u64 want)
{
}




int file_check(u8* buf, int len)
{
	u64 type;

	//picture
	if(check_bmp(buf) != 0){
		type = _bmp_;
		parse_bmp(buf, len);
	}
	else if(check_flif(buf) != 0){
		type = _flif_;
		parse_flif(buf, len);
	}
	else if(check_jpg(buf) != 0){
		type = _jpg_;
		parse_jpg(buf, len);
	}
	else if(check_png(buf) != 0){
		type = _png_;
		parse_png(buf, len);
	}
	else if(check_webp(buf) != 0){
		type = _webp_;
		parse_webp(buf, len);
	}

	//compress
	else if(check_7z(buf) != 0){
		type = _7z_;
		parse_7z(buf);
	}
	else if(check_cpio(buf) != 0){
		type = _cpio_;
		parse_cpio(buf);
	}
	else if(check_gz(buf) != 0){
		type = _gz_;
		parse_gz(buf);
	}
	else if(check_tar(buf) != 0){
		type = _tar_;
		parse_tar(buf);
	}
	else if(check_zip(buf) != 0){
		type = _zip_;
		parse_zip(buf);
	}

	//executable
	else if(check_elf(buf) != 0){
		type = _elf_;
		parse_elf(buf);
	}
	else if(check_macho(buf) != 0){
		type = _macho_;
		parse_macho(buf);
	}
	else if(check_pe(buf) != 0){
		type = _pe_;
		parse_pe(buf);
	}

	//filesystem
	else if(check_ext(buf) != 0){
		type = _ext_;
		say("ext\n");
	}
	else if(check_fat(buf) != 0){
		type = _fat_;
		say("fat\n");
	}
	else if(check_hfs(buf) != 0){
		type = _hfs_;
		say("hfs\n");
	}
	else if(check_ntfs(buf) != 0){
		type = _ntfs_;
		say("ntfs\n");
	}

	//parttable
	else if(check_gpt(buf) != 0){
		type = _gpt_;
		//parse_gpt(buf);
	}
	else if(check_mbr(buf) != 0){
		type = _mbr_;
		//parse_mbr(buf, dirhome);
	}

	//unknown
	else
	{
		type = 0;
		printmemory(buf, 0x200);
	}

	return type;
}




int fileclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	u64 type;
	struct object* obj;

	ret = 0;
	for(ret=0;ret<0x1000;ret++){
		if(url[ret] < 0x20)break;
		buf[ret] = url[ret];
	}
	buf[ret] = 0;

	obj = systemcreate(_file_, buf);
	if(0 == obj)return 0;

	ret = readfile(obj->selffd, 0, buf, 0x1000);
	if(ret != 0x1000)goto fail;

	type = file_check(buf, 0x1000);
	if(0 == type)goto fail;

	ele->type = type;
	relationcreate(ele, 0, _art_, obj, 0, _fd_);

	if(_mbr_ == type)mbrclient_start(obj, 0, ele, 0, buf, ret);
	if(_gpt_ == type)gptclient_start(obj, 0, ele, 0, buf, ret);
	return 0;

fail:
	systemdelete(obj);
	return 0;
}