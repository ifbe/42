struct pci
{
	//[0,f]
	u32 id;
	u32 stscmd;
	u32 class;
	u32 what;

	//[10,27]
	u32 bar0;
	u32 bar1;
	u32 bar2;
	u32 bar3;
	u32 bar4;
	u32 bar5;

	//[28,3f]
	u32 cardbus;
	u32 subsys;
	u32 expansion;
	u32 capability;
	u32 rsvd;
	u32 interrupt;
};
