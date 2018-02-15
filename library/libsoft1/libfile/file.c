#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
//compress
#define __7z__ hex32('7','z',0,0)
int check_7z(void*);
int parse_7z(void*);
#define __cpio__ hex32('c','p','i','o')
int check_cpio(void*);
int parse_cpio(void*);
#define __gz__ hex32('g','z',0,0)
int check_gz(void*);
int parse_gz(void*);
#define __tar__ hex32('t','a','r',0)
int check_tar(void*);
int parse_tar(void*);
#define __zip__ hex32('z','i','p',0)
int check_zip(void*);
int parse_zip(void*);
//diskimage
#define __dmg__ hex32('c','p','i','o')
int check_dmg(void*);
int parse_dmg(void*);
#define __qcow2__ hex32('q','c','2',0)
int check_qcow2(void*);
int parse_qcow2(void*);
#define __vdi__ hex32('v','d','i',0)
int check_vdi(void*);
int parse_vdi(void*);
#define __vhd__ hex32('v','h','d',0)
int check_vhd(void*);
int parse_vhd(void*);
#define __vmdk__ hex32('v','m','d','k')
int check_vmdk(void*);
int parse_vmdk(void*);
//executable
#define __elf__ hex32('e','l','f',0)
int check_elf(void*);
int parse_elf(void*);
#define __macho__ hex32('m','a','c','o')
int check_macho(void*);
int parse_macho(void*);
#define __pe__ hex32('p','e',0,0)
int check_pe(void*);
int parse_pe(void*);
//filesystem
#define __ext__ hex32('e','x','t',0)
int check_ext(void*);
int parse_ext(void*, void*);
#define __fat__ hex32('f','a','t',0)
int check_fat(void*);
int parse_fat(void*, void*);
#define __hfs__ hex32('h','f','s',0)
int check_hfs(void*);
int parse_hfs(void*, void*);
#define __ntfs__ hex32('n','t','f','s')
int check_ntfs(void*);
int parse_ntfs(void*, void*);
//model
#define __3mf__ hex32('3','m','f',0)
int check_3mf(void*);
int parse_3mf(void*);
#define __dwg__ hex32('d','w','g',0)
int check_dwg(void*);
int parse_dwg(void*);
#define __stl__ hex32('s','t','l',0)
int check_stl(void*);
int parse_stl(void*);
//parttable
#define __applept__ hex32('a','p','t',0)
int check_applept(void*);
int parse_applept(void*, void*);
#define __gpt__ hex32('g','p','t',0)
int check_gpt(void*);
int parse_gpt(void*, void*);
#define __mbr__ hex32('m','b','r',0)
int check_mbr(void*);
int parse_mbr(void*, void*);
//picture
#define __bmp__ hex32('b','m','p',0)
int check_bmp(void*);
int parse_bmp(void*, int);
#define __flif__ hex32('f','l','i','f')
int check_flif(void*);
int parse_flif(void*, int);
#define __jpg__ hex32('j','p','g',0)
int check_jpg(void*);
int parse_jpg(void*, int);
#define __png__ hex32('p','n','g',0)
int check_png(void*);
int parse_png(void*, int);
#define __webp__ hex32('w','e','b','p')
int check_webp(void*);
int parse_webp(void*, int);
//
int startfile(u8*);
int stopfile(int);
int readfile(u64 file, u8* mem, u64 off, u64 len);
int writefile(u64 file, u8* mem, u64 off, u64 len);
//
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	u64 type0;
	u64 stage0;
	u64 type1;
	u64 stage1;
	u64 type2;
	u64 stage2;
	u64 type3;
	u64 stage3;

	u8 data[0xc0];
};
struct filesystem
{
	//
	u64 magic;	//mnt
	u64 parent;	//fs[fs[this].parent].read?
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	//
	u8 data[0xc0];
};
static struct object* obj;
static struct filesystem* fs;
static u8* dirhome;
static u8* datahome;




int openreadclose(void* name, void* buf, int off, int len)
{
	int ret;
	int fd = startfile(name);
	if(fd <= 0)return fd;

	ret = readfile(fd, buf, off, len);

	stopfile(fd);
	return ret;
}
int openwriteclose(void* name, void* buf, int off, int len)
{
	int ret;
	int fd = startfile(name);
	if(fd <= 0)return fd;

	ret = writefile(fd, buf, off, len);

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
	readfile(0, rdi, rsi, rcx);
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




int file_read(u8* buf, int len)
{
	//picture
	if(check_bmp(buf) != 0)return __bmp__;
	else if(check_flif(buf) != 0)return __flif__;
	else if(check_jpg(buf) != 0)return __jpg__;
	else if(check_png(buf) != 0)return __png__;
	else if(check_webp(buf) != 0)return __webp__;

	//compress
	else if(check_7z(buf) != 0)return __7z__;
	else if(check_cpio(buf) != 0)return __cpio__;
	else if(check_gz(buf) != 0)return __gz__;
	else if(check_tar(buf) != 0)return __tar__;
	else if(check_zip(buf) != 0)return __zip__;

	//executable
	else if(check_elf(buf) != 0)return __elf__;
	else if(check_macho(buf) != 0)return __macho__;
	else if(check_pe(buf) != 0)return __pe__;

	//filesystem
	else if(check_ext(buf) != 0)return __ext__;
	else if(check_fat(buf) != 0)return __fat__;
	else if(check_hfs(buf) != 0)return __hfs__;
	else if(check_ntfs(buf) != 0)return __ntfs__;

	//parttable
	else if(check_gpt(buf) != 0)return __gpt__;
	else if(check_mbr(buf) != 0)return __mbr__;

	return 0;
}
int file_write(u8* p)
{
	int j,k;
	int len;
	u64 type;
	u8* q;

	//read
	len = openreadclose(p, datahome, 0, 0x8000);
	if(len <= 0)
	{
		say("error@openreadclose\n");
		return 0;
	}

	//11111111
	type = file_read(datahome, len);
	if(0 == type)
	{
		j = 0;
		while(p[j] >= 0x20)j++;

		for(;j >= 0;j--)
		{
			if('.' == p[j])
			{
				j++;
				q = (void*)&type;
				for(k=0;k<4;k++)
				{
					if(0 == p[j+k])break;
					q[k] = p[j+k];
				}
				break;
			}
		}
	}

	//picture
	if(__bmp__ == type)parse_bmp(datahome, len);
	else if(__flif__ == type)parse_flif(datahome, len);
	else if(__jpg__ == type)parse_jpg(datahome, len);
	else if(__png__ == type)parse_png(datahome, len);
	else if(__webp__ == type)parse_webp(datahome, len);

	//compress
	else if(__7z__ == type)parse_7z(datahome);
	else if(__cpio__ == type)parse_cpio(datahome);
	else if(__gz__ == type)parse_gz(datahome);
	else if(__tar__ == type)parse_tar(datahome);
	else if(__zip__ == type)parse_zip(datahome);

	//executable
	else if(__elf__ == type)parse_elf(datahome);
	else if(__macho__ == type)parse_macho(datahome);
	else if(__pe__ == type)parse_pe(datahome);

	//filesystem
	else if(__ext__ == type)say("ext\n");
	else if(__fat__ == type)say("fat\n");
	else if(__hfs__ == type)say("hfs\n");
	else if(__ntfs__ == type)say("ntfs\n");

	//model
	else if(__stl__ == type)parse_stl(datahome);

	//parttable
	else if(__gpt__ == type)parse_gpt(datahome, dirhome);
	else if(__mbr__ == type)parse_mbr(datahome, dirhome);

	//unknown
	else printmemory(datahome, 0x200);

	return 0;
}
int file_ls()
{
	int j;
	u8* p8;
	u64* p64;
	for(j=0;j<0x10;j++)
	{
		p8 = dirhome + j*0x80;
		p64 = (void*)p8;
		if(p64[0] == 0)break;

		say("%-16s%-16s%s\n", p8, p8+8, p8+0x40);
	}
	return 0;
}
int file_cd()
{
	return 0;
}
int file_start()
{
	return 0;
}
int file_stop()
{
	return 0;
}
int file_create(void* softaddr, u64* p)
{
	obj = softaddr;
	fs = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;
}
int file_delete()
{
	return 0;
}
