#include "libboot.h"
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);




void relcopy(struct halfrel* dst, struct halfrel* src)
{
	dst->chip = src->chip;
	dst->foot = src->foot;
	dst->type = src->type;
	dst->flag = src->flag;
}
int relation_take(struct item* item,void* foot, struct halfrel* stack,int sp, void* arg, int cmd, void* buf, int len)
{
	if(item->ontaking){
		return item->ontaking(item,foot, stack,sp, arg,cmd, buf,len);
	}
	switch(item->tier){
		case _ori_:return origin_take(item,foot, stack,sp, arg, cmd, buf, len);
		case _wrk_:return bootup_take(item,foot, stack,sp, arg, cmd, buf, len);
		case _dev_:return device_take(item,foot, stack,sp, arg, cmd, buf, len);
		case _dri_:return driver_take(item,foot, stack,sp, arg, cmd, buf, len);
		case _sys_:return system_take((void*)item,foot, stack,sp, arg, cmd, buf, len);
		case _art_:return artery_take((void*)item,foot, stack,sp, arg, cmd, buf, len);
		case _sup_:return supply_take((void*)item,foot, stack,sp, arg, cmd, buf, len);
		case _ent_:return entity_take((void*)item,foot, stack,sp, arg, cmd, buf, len);
	}
	return 0;
}
int take_data_from_peer(void* chip,int ftype, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->dstflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->dst);
				relcopy(&stack[sp+1], (void*)rel->src);
			}
			return relation_take(rel->psrcchip, rel->psrcfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->srcflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->src);
				relcopy(&stack[sp+1], (void*)rel->dst);
			}
			return relation_take(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int take_data_from_them(void* chip,int ftype, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->dstflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->dst);
				relcopy(&stack[sp+1], (void*)rel->src);
			}
			relation_take(rel->psrcchip, rel->psrcfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->srcflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->src);
				relcopy(&stack[sp+1], (void*)rel->dst);
			}
			relation_take(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int relation_w(struct item* item,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	if(item->ontaking){
		return item->ongiving(item,foot, stack,sp, arg,cmd, buf,len);
	}
	switch(item->tier){
		case _ori_:return origin_give(item,foot, stack,sp, arg,cmd, buf,len);
		case _wrk_:return bootup_give(item,foot, stack,sp, arg,cmd, buf,len);
		case _dev_:return device_give(item,foot, stack,sp, arg,cmd, buf,len);
		case _dri_:return driver_give(item,foot, stack,sp, arg,cmd, buf,len);
		case _sys_:return system_give((void*)item,foot, stack,sp, arg,cmd, buf,len);
		case _art_:return artery_give((void*)item,foot, stack,sp, arg,cmd, buf,len);
		case _sup_:return supply_give((void*)item,foot, stack,sp, arg,cmd, buf,len);
		case _ent_:return entity_give((void*)item,foot, stack,sp, arg,cmd, buf,len);
	}
	return 0;
}
int give_data_into_orel(void* chip,int ftype, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	struct item* item = chip;
	if(0 == item)return 0;

	struct relation* rel = item->orel0;
	if(0 == rel)return 0;

	struct item* peer = rel->pdstchip;
	if(0 == peer)return 0;

	return peer->give(chip,0, stack,sp, arg,cmd, buf,len);
}
int give_data_into_peer(void* chip,int ftype, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		//say("irel:%.8s\n",&rel->dstflag);
		if(ftype == rel->dstflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->dst);
				relcopy(&stack[sp+1], (void*)rel->src);
			}
			relation_w(rel->psrcchip, rel->psrcfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		//say("orel:%.8s\n",&rel->srcflag);
		if(ftype == rel->srcflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->src);
				relcopy(&stack[sp+1], (void*)rel->dst);
			}
			relation_w(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int give_data_into_them(void* chip,int ftype, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		//say("irel:%.8s\n",&rel->dstflag);
		if(ftype == rel->dstflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->dst);
				relcopy(&stack[sp+1], (void*)rel->src);
			}
			relation_w(rel->psrcchip, rel->psrcfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		//say("orel:%.8s\n",&rel->srcflag);
		if(ftype == rel->srcflag){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->src);
				relcopy(&stack[sp+1], (void*)rel->dst);
			}
			relation_w(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}