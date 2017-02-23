#include "com_example_test.h"
int i = 0;
JNIEXPORT jint JNICALL Java_com_example_test_get(JNIEnv *env, jclass jc)
{
    return i;
}
JNIEXPORT void JNICALL Java_com_example_test_set(JNIEnv *env, jclass jc, jint j)
{
    i = j;
}
