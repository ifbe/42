#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
struct stack
{
	int cur;
	int len;
	u64 data[0];
};




void stack_generate(struct stack* this, int len)
{
	if(this == 0)return;
	this->cur = 0;
	this->len = (len>>3) - 1;
}
void stack_destory(struct stack* this)
{
}
int stack_push(struct stack* this, u64* data)
{
	if(data == 0)return 0;
	if(this == 0)return 0;
	if(this->cur + 1 > this->len)return 0;

	this->data[this->cur] = *data;
	this->cur += 1;
	return 1;
}
int stack_pop(struct stack* this, u64* data)
{
	if(data == 0)return 0;
	if(this == 0)return 0;
	if(this->cur < 1)return 0;

	this->cur -= 1;
	*data = this->data[this->cur];
	return 1;
}
