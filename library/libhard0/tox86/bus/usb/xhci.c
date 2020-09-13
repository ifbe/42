#include "libhard.h"
u32 in32(u16 port);
void out32(u16 port, u32 data);




int ownership(u8* addr)
{
	//set hc os owned semaphore
	volatile u32* temp = (u32*)addr;
	temp[0] = temp[0] | 0x1000000;

	//看看控制权到手没
	volatile int timeout = 0;
	while(1){
		if( (temp[0]&0x1000000) == 0x1000000 ){
			if( (temp[0]&0x10000) == 0 ){
				say("   bios gone\n");
				return 1;
			}
		}

		timeout++;
		if(timeout > 0xffffff){
			say("   failed to get ownership\n");
			return -1;
		}
	}
}
void supportedprotocol(u8* addr)
{
	u64 first = (*(u32*)addr) >> 16;
	u64 third = *(u32*)(addr+8);
	u64 start = third & 0xff;
	u64 count = (third>>8) & 0xff;

	//say("first:",first);
	if(first<0x100){         //[0,0xff]
		say("	usb?\n");
	}
	else if(first<=0x200){   //[0x100,0x1ff]
		say("	usb1:%x\n",start);
	}
	else if(first<=0x300){   //[0x200,0x2ff]
		say("	usb2:%x\n",start);
	}
	else{                   //[0x300,0x3ff]
		say("	usb3:%x\n",start);
	}
}
void explainxecp(u8* addr)
{
	say("xecp@%p:\n", addr);

	while(1){
		say("	cap@%p\n", addr);
		volatile u32 temp = *(u32*)addr;
		u8 type = temp&0xff;

		say("	type=%x\n",type);
		switch(type){
			case 1:{
				ownership(addr);
				break;
			}
			case 2:{
				supportedprotocol(addr);
				break;
			}
		}

		u64 next = (temp>>6)&0x3fc;
		if(0 == next)break;
		addr += next;
	}
}
int xhci_mmioinit(struct device* dev, u8* xhciaddr)
{
	say("xhci@mmio:%p{\n", xhciaddr);
	//printmmio(addr, 0x20);


//---------------clear home---------------------
	int j,k;
	u8* ptr = memorycreate(0x100000, 0);
	for(j=0;j<0x100000;j++)ptr[j] = 0;


//--------------basic infomation--------------------
	volatile u64 version = (*(u32*)xhciaddr) >> 16;
	volatile u64 caplen = (*(u32*)xhciaddr) & 0xffff;

	volatile u64 hcsparams1 = *(u32*)(xhciaddr+4);
	volatile u64 hcsparams2 = *(u32*)(xhciaddr+8);
	volatile u64 hcsparams3 = *(u32*)(xhciaddr+0xc);
	volatile u64 capparams = *(u32*)(xhciaddr+0x10);

	volatile u8* operational = xhciaddr + caplen;
	volatile u8* runtime = xhciaddr + (*(u32*)(xhciaddr+0x18));

	say("base information:\n");
	say("	version:%x\n", version);
	say("	caplength:%x\n", caplen);

	say("	hcsparams1:%x\n",hcsparams1);
	say("	hcsparams2:%x\n",hcsparams2);
	say("	hcsparams3:%x\n",hcsparams3);
	say("	capparams:%x\n",capparams);

	say("	operational@%p\n",operational);
	say("	runtime@%p\n",runtime);


//--------------grab ownership-----------------
	u8* xecp = xhciaddr+( (capparams >> 16) << 2 );
	explainxecp(xecp);


//--------------print operational---------------
	volatile u64 usbcommand = *(u32*)operational;
	volatile u64 usbstatus  = *(u32*)(operational+4);
	volatile u64 pagesize   = 0x1000;
	volatile u64 crcr       = *(u32*)(operational+0x18);
	volatile u64 dcbaa      = *(u32*)(operational+0x30);
	volatile u64 config     = *(u32*)(operational+0x38);
	volatile u64 psz        = *(u32*)(operational+8);
	while(1){
		if(0 == psz){
			say("psz:\n",psz);
			return -1;
		}
		if(1 == psz)break;

		psz >>= 1;
		pagesize <<= 1;
	}
	say("before anything:\n");
	say("	usbcommand:%x\n", usbcommand);
	say("	usbstatus:%x\n", usbstatus);
	say("	pagesize:%x\n", pagesize);
	say("	crcr:%x\n", crcr);
	say("	dcbaa:%x\n", dcbaa);
	say("	config:%x\n", config);


//-------------------xhci reset----------------
	say("init xhci:\n",0);

	//xhci正在运行吗
	if( (usbstatus&0x1) == 0){		//=0: runing
		say("	hc stopping\n");

		//按下停止按钮
		*(u32*)operational=usbcommand&0xfffffffe;

		//等一会
		u64 wait1 = 0xfffffff;
		for(;wait1>0;wait1--)asm("nop");

		//xhci停了吗
		usbstatus = *(u32*)(operational+4);
		if( (usbstatus&0x1) == 0){       //=0: still runing
			say("	not stop\n");
			return -2;
		}
	}
	say("	hc stopped\n");

	//按下复位按钮
	usbcommand = *(u32*)operational;
	*(u32*)operational = usbcommand|2;

	//等一会
	u64 wait2 = 0xfffffff;
	for(;wait2>0;wait2--)asm("nop");

	usbcommand = *(u32*)(operational);
	if((usbcommand&0x2) == 2){
		say("	reset failed:%x\n", usbcommand);
		return -3;
	}
	say("	hc reseted\n");

	//controller not ready=1就是没准备好
	usbstatus = *(u32*)(operational+4);
	if( (usbstatus&0x800) == 0x800 ){
		say("	not ready:%x\n",usbstatus);
		return -4;
	}
	say("	hc ready\n");


//----------------------2.xhci结构---------------------------
//program the max device slot enabled field in config register
//program the dcbaap
//program crcr,point to addr of first trb in command ring
//----------------------------------------------------------
	say("fill maxslot&dcbaa&crcr:\n");
	//maxslot=deviceslots
	*(u32*)(operational+0x38) = (*(u32*)(xhciaddr+4)) &0xff;
	say("	maxslot:%x\n", *(u32*)(operational+0x38) );
/*
	//dcbaa(scratchpad)
	*(u32*)(operational+0x30) = dcbahome;
	*(u32*)(operational+0x34) = 0;
	say("	dcbaa:%x\n", dcbahome);

	*(u32*)(dcbahome) = scratchpadhome;             //用了宏，加括号
	*(u32*)(dcbahome+4) = 0;                                  //高位
	say("	scratchpad:%x\n",scratchpadhome);

	//command linktrb:lastone point to firstone
	*(u64*)(cmdringhome+0xfff*0x10) = cmdringhome;
	*(u32*)(cmdringhome+0xfff*0x10+8) = 0;
	*(u32*)(cmdringhome+0xfff*0x10+0xc) = (6<<10)+2;

	//crcr
	*(u32*)(operational+0x18) = cmdringhome+1;
	*(u32*)(operational+0x1c) = 0;
	say("	crcr:%x\n",cmdringhome+1);


//-----------------------3.中断,eventring----------------------
//msix table,message addr,message data,enable vector
//allocate&init msix pending bit array
//point table&pba offset to message control table and pending bit array
//init message control register of msix capability structure
//-------------------------------------------------
	say("set interrupt:\n");

	//setupisr(xhciaddr);
	//----------------------if(msix)--------------------------
	//----------------------if(msi)--------------------------
	//----------------------if(intx)--------------------------

	//-------------define event ring-----------
	//build the "event ring segment table"
	*(u64*)(ersthome) = eventringhome;
	*(u64*)(ersthome+0x8) = 0x100;            //0x1000*0x10=0x10000
	//but!!!my asus n55 must set it below 0x100

	//ERSTSZ
	*(u32*)(runtime+0x28)=1;

	//ERSTBA
	*(u32*)(runtime+0x30)=ersthome;
	*(u32*)(runtime+0x34)=0;        //这一行必须有

	//ERDP
	*(u32*)(runtime+0x38)=eventringhome;
	*(u32*)(runtime+0x3c)=0;        //这一行也必须有

	//enable EWE|HSEIE|INTE(0x400|0x8|0x4) in USBCMD
//	*(u32*)operational |= 0x40c;
//	*(u32*)operational = (*(u32*)operational) | 0xc;      //no interrupt

	//IMOD
	*(u32*)(runtime+0x24)=4000;

	//IMAN
	*(u32*)(runtime+0x20) = (*(u32*)(runtime+0x20)) | 0x2;

	say("	event ring@%x\n",eventringhome);


//---------------------xhci启动---------------------
//write usbcmd,turn host controller on
//-------------------------------------------------
	say("turnon hc:\n");
	*(u32*)operational = (*(u32*)operational) | 0x1;

*/
	say("}\n");
	return 0;
}
int xhci_portinit(struct device* dev, u32 addr)
{
	u64 temp,high;
	say("xhci@port:%x{\n", addr);

	out32(0xcf8, addr+0x4);
	temp=in32(0xcfc) | (1<<10) | (1<<2);	//bus master=1
	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	say("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x10);
	temp = in32(0xcfc);
	say("bar0=%x\n", temp);			//lo
	out32(0xcf8, addr+0x14);
	high = in32(0xcfc);
	say("bar1=%x\n", high);			//hi
	out32(0xcf8, addr+0x18);
	say("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	say("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	say("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	say("bar5=%x\n", in32(0xcfc));

	say("}\n");


	//xhci mmio
	u8* mmio = (u8*)((temp&0xfffffff0) | (high<<32));
	return xhci_mmioinit(dev, mmio);
}