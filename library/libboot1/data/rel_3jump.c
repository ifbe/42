//3jump: nodeA to slotA to slotB to nodeB
#include "libboot.h"
#define maxlen 0x100000




struct complexrelation{
    u64 nodechip;
    u64 nodefoot;
    u32 nodechiptype;
    u32 nodefoottype;
    u32 nodeprev;
    u32 nodenext;

    u64 relchip;    //voltage of wire body?
    u64 relfoot;    //current of wire branch?
    u32 relchiptype;
    u32 relfoottype;
    u32 relprev;
    u32 relnext;
};
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
	struct testhalfrel{
        u64 chip;
        u64 foot;
        u32 chiptype;
        u32 foottype;
        u32 prev;
        u32 next;
	}rel[3];
};



/*
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
*/