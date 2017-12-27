#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
struct eachdata
{
	//if(leaf)		//if(index)
	u32 hash0;		//same
	u32 hash1;		//same
	u32 buf;		//ignore
	u32 len;		//ignore
	u64 irel;		//bigger childnode
	u64 orel;		//ignore
};
struct bplusnode
{
	//if(leaf)		//if(index)
	u32 type;		//same
	u32 len;		//same
	u64 parentnode;		//same
	u64 prevnode;		//smaller childnode
	u64 nextnode;		//ignore
	struct eachdata node[3];
};




void* bplus_grow(struct bplusnode* info)
{
	if(info == 0)return 0;
}
void* bplus_recycle(struct bplusnode* info)
{
	if(info == 0)return 0;
}




void* bplus_split(struct bplusnode* info, struct bplusnode* node)
{
	int j;
	u8* dst;
	u8* src;
	struct bplusnode* right;
	struct bplusnode* upper;
}
void* bplus_merge(struct bplusnode* info, struct bplusnode* node)
{
}




void* bplus_insert(struct bplusnode* info, void* data)
{
	if(info == 0)return 0;
}
void* bplus_destory(struct bplusnode* info, u64 hash)
{
	if(info == 0)return 0;
}




void bplus_prepare(void* info)
{
	if(info == 0)return;
	//memory_prepare(info+0x20);
}
void bplus_stop(struct bplusnode* info)
{
}
