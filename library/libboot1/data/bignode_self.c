#include "libboot.h"
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);




int relation_read(struct item* item,void* foot, p64 arg, int cmd, void* buf, int len)
{
	if(item->onreader){
		return item->onreader(item,foot, arg,cmd, buf,len);
	}
	switch(item->tier){
		case _ori_:return origin_reader(item,foot, arg, cmd, buf, len);
		case _wrk_:return bootup_reader(item,foot, arg, cmd, buf, len);
		case _dev_:return device_reader(item,foot, arg, cmd, buf, len);
		case _dri_:return driver_reader(item,foot, arg, cmd, buf, len);
		case _sys_:return system_reader((void*)item,foot, arg, cmd, buf, len);
		case _art_:return artery_reader((void*)item,foot, arg, cmd, buf, len);
		case _sup_:return supply_reader((void*)item,foot, arg, cmd, buf, len);
		case _ent_:return entity_reader((void*)item,foot, arg, cmd, buf, len);
	}
	return 0;
}
int reading_data_from_peer(void* chip,int ftype, p64 arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->dstfoottype){
			return relation_read(rel->psrcchip, rel->psrcfoot, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->srcfoottype){
			return relation_read(rel->pdstchip, rel->pdstfoot, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int relation_write(struct item* item,void* foot, p64 arg,int cmd, void* buf,int len)
{
	if(item->onwriter){
		return item->onwriter(item,foot, arg,cmd, buf,len);
	}
	switch(item->tier){
		case _ori_:return origin_writer(item,foot, arg,cmd, buf,len);
		case _wrk_:return bootup_writer(item,foot, arg,cmd, buf,len);
		case _dev_:return device_writer(item,foot, arg,cmd, buf,len);
		case _dri_:return driver_writer(item,foot, arg,cmd, buf,len);
		case _sys_:return system_writer((void*)item,foot, arg,cmd, buf,len);
		case _art_:return artery_writer((void*)item,foot, arg,cmd, buf,len);
		case _sup_:return supply_writer((void*)item,foot, arg,cmd, buf,len);
		case _ent_:return entity_writer((void*)item,foot, arg,cmd, buf,len);
	}
	return 0;
}
int writing_data_into_peer(void* chip,int ftype, p64 arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		//say("irel:%.8s\n",&rel->dstfoottype);
		if(ftype == rel->dstfoottype){
			relation_write(rel->psrcchip, rel->psrcfoot, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		//say("orel:%.8s\n",&rel->srcfoottype);
		if(ftype == rel->srcfoottype){
			relation_write(rel->pdstchip, rel->pdstfoot, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




void relcopy(struct halfrel* dst, struct halfrel* src)
{
	dst->chip = src->chip;
	dst->foot = src->foot;
	dst->nodetype = src->nodetype;
	dst->foottype = src->foottype;
}




int relation_take(struct item* item,void* foot, struct halfrel* stack,int sp, p64 arg, int cmd, void* buf, int len)
{
	if(item->ontaking){
		return item->ontaking(item,foot, stack,sp, arg,cmd, buf,len);
	}
	switch(item->tier){
		case _ori_:return origin_takeby(item,foot, stack,sp, arg, cmd, buf, len);
		case _wrk_:return bootup_takeby(item,foot, stack,sp, arg, cmd, buf, len);
		case _dev_:return device_takeby(item,foot, stack,sp, arg, cmd, buf, len);
		case _dri_:return driver_takeby(item,foot, stack,sp, arg, cmd, buf, len);
		case _sys_:return system_takeby((void*)item,foot, stack,sp, arg, cmd, buf, len);
		case _art_:return artery_takeby((void*)item,foot, stack,sp, arg, cmd, buf, len);
		case _sup_:return supply_takeby((void*)item,foot, stack,sp, arg, cmd, buf, len);
		case _ent_:return entity_takeby((void*)item,foot, stack,sp, arg, cmd, buf, len);
	}
	return 0;
}
int take_data_from_peer(void* chip,int ftype, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->dstfoottype){
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
		if(ftype == rel->srcfoottype){
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
int take_data_from_them(void* chip,int ftype, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		if(ftype == rel->dstfoottype){
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
		if(ftype == rel->srcfoottype){
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
int take_data_from_peer_temp_stack(void* chip,int ftype, p64 arg,int cmd, void* buf,int len)
{
	struct halfrel stack[0x80];
	return take_data_from_peer(chip,ftype, stack,0, arg,cmd, buf,len);
}
int take_data_from_them_temp_stack(void* chip,int ftype, p64 arg,int cmd, void* buf,int len)
{
	struct halfrel stack[0x80];
	return take_data_from_them(chip,ftype, stack,0, arg,cmd, buf,len);
}




int relation_give(struct item* item,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	if(item->ontaking){
		return item->ongiving(item,foot, stack,sp, arg,cmd, buf,len);
	}
	switch(item->tier){
		case _ori_:return origin_giveby(item,foot, stack,sp, arg,cmd, buf,len);
		case _wrk_:return bootup_giveby(item,foot, stack,sp, arg,cmd, buf,len);
		case _dev_:return device_giveby(item,foot, stack,sp, arg,cmd, buf,len);
		case _dri_:return driver_giveby(item,foot, stack,sp, arg,cmd, buf,len);
		case _sys_:return system_giveby((void*)item,foot, stack,sp, arg,cmd, buf,len);
		case _art_:return artery_giveby((void*)item,foot, stack,sp, arg,cmd, buf,len);
		case _sup_:return supply_giveby((void*)item,foot, stack,sp, arg,cmd, buf,len);
		case _ent_:return entity_giveby((void*)item,foot, stack,sp, arg,cmd, buf,len);
	}
	return 0;
}
int give_data_into_orel(void* chip,int ftype, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	struct item* item = chip;
	if(0 == item)return 0;

	struct relation* rel = item->orel0;
	if(0 == rel)return 0;

	struct item* peer = rel->pdstchip;
	if(0 == peer)return 0;

	return peer->give(chip,0, stack,sp, arg,cmd, buf,len);
}
int give_data_into_peer(void* chip,int ftype, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		//say("irel:%.8s\n",&rel->dstfoottype);
		if(ftype == rel->dstfoottype){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->dst);
				relcopy(&stack[sp+1], (void*)rel->src);
			}
			relation_give(rel->psrcchip, rel->psrcfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		//say("orel:%.8s\n",&rel->srcfoottype);
		if(ftype == rel->srcfoottype){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->src);
				relcopy(&stack[sp+1], (void*)rel->dst);
			}
			relation_give(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int give_data_into_them(void* chip,int ftype, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	struct item* item;
	struct relation* rel;

	if(0 == chip)return 0;
	item = chip;

	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		//say("irel:%.8s\n",&rel->dstfoottype);
		if(ftype == rel->dstfoottype){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->dst);
				relcopy(&stack[sp+1], (void*)rel->src);
			}
			relation_give(rel->psrcchip, rel->psrcfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		//say("orel:%.8s\n",&rel->srcfoottype);
		if(ftype == rel->srcfoottype){
			if(stack){
				relcopy(&stack[sp+0], (void*)rel->src);
				relcopy(&stack[sp+1], (void*)rel->dst);
			}
			relation_give(rel->pdstchip, rel->pdstfoot, stack,sp+2, arg,cmd, buf,len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int give_data_into_orel_temp_stack(void* chip,int ftype, p64 arg,int cmd, void* buf,int len)
{
	struct halfrel stack[0x80];
	return give_data_into_orel(chip,ftype, stack,0, arg,cmd, buf,len);
}
int give_data_into_peer_temp_stack(void* chip,int ftype, p64 arg,int cmd, void* buf,int len)
{
	struct halfrel stack[0x80];
	return give_data_into_peer(chip,ftype, stack,0, arg,cmd, buf,len);
}
int give_data_into_them_temp_stack(void* chip,int ftype, p64 arg,int cmd, void* buf,int len)
{
	struct halfrel stack[0x80];
	return give_data_into_them(chip,ftype, stack,0, arg,cmd, buf,len);
}