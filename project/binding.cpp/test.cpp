#include<iostream>
extern "C" {
	void birth();
	void death();
	void say(const void*, ...);
}
int main()
{
	birth();

	say("here\n");

	death();
	return 0;
}
