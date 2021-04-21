#include "libboot.h"
#define maxlen 0x100000




struct nondirrel
{
	//0x00,0x1f
	u16 lock[4];
	u16 refcnt[4];
	u32 chiptype;
	u32 foottype;
	u32 prev;
	u32 next;

	//0x20,0x7f
	struct halfrel{
        u64 chip;
        u64 foot;
        u32 chiptype;
        u32 foottype;
        u32 prev;
        u32 next;
	}rel[3];
};




void* findprev(struct halfrel* rel){
    tmp = offs2addr(rel->next);
}
void* findnext(struct halfrel* rel){
    tmp = offs2addr(rel->next);
}




void* findpeer(struct halfrel* rel){
    off = rel->chip&3;
    xxx = ((void*)rel) - off*sizeof(struct halfrel);
    for(j=1;j<3;j++){
        if(j == off)continue;
        return xxx->rel[j];
    }
}
void* findpeer_all(struct halfrel* rel){
}
void* findpeer_type(struct halfrel* rel){
}





void* newrel_twoguy(struct halfrel* a,struct halfrel* b){
    tmp = nondirrel_alloc();
    copytoxxxfrom(tmp->rel[0], a);
    copytoxxxfrom(tmp->rel[1], b);
}
void* newrel_addguy(struct nondirrel* r, struct halfrel* a){
    copytoxxxfrom(r->rel[2],a);
}
