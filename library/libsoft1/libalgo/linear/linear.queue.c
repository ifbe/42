#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
struct queue
{
	int abc;
	int len;
	int enq;
	int deq;
	u64 data[0];
};




void queue_generate(struct queue* this, int len)
{
	if(this == 0)return;
	this->len = (len>>3) - 2;
	this->enq = 0;
	this->deq = 0;
}
void queue_destory(struct queue* this)
{
}
int queue_in(struct queue* this, u64* data)
{
	if(this == 0)return 0;
	if((this->enq + 1)%(this->len) == this->deq)return 0;

	this->data[(this->enq)] = *data;
	this->enq = (this->enq + 1)%(this->len);
	return 1;
}
int queue_out(struct queue* this, u64* data)
{
	if(this == 0)return 0;
	if(this->enq == this->deq)return 0;

	*data = this->data[this->deq];
	this->deq = (this->deq + 1)%(this->len);
	return 1;
}
