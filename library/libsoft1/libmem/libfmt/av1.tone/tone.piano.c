#define stage 1.0594630943592953




//00: 27.5
//12: 55
//24: 110
//36: 220
//48: 440
//60: 880
//72: 1760
//84: 3520
int piano_freq(int a)
{
	int q = a/12;
	int r = a%12;
	double j = 27.5;
	for(;q>0;q--)j *= 2;
	for(;r>0;r--)j *= stage;
	return (int)j;
}
