#include "com_example_test.h"
void birth();
void death();
void say(void*, ...);




JNIEXPORT void JNICALL Java_com_example_test_birth(JNIEnv *env, jclass jc)
{
	birth();
}
JNIEXPORT void JNICALL Java_com_example_test_death(JNIEnv *env, jclass jc)
{
	death();
}
JNIEXPORT void JNICALL Java_com_example_test_print(JNIEnv *env, jclass jc)
{
	say("here %d\n", 666666);
}
