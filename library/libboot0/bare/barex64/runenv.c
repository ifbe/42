#include "libboot.h"
#define RSD_PTR_ hex64('R','S','D',' ','P','T','R',' ')




void* getmemmap()
{
	return 0;
}
void* getdevmap()
{
	int j;
	u64* p = (void*)0xe0000;
	for(j=0;j<0x4000;j++){
		if(RSD_PTR_ == p[j])return &p[j];
	}
	return 0;
}




void freerunenv()
{
}
void initrunenv()
{
}
