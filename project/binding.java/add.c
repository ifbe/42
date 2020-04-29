#include"test.h"

JNIEXPORT jint JNICALL Java_test_add
  (JNIEnv* env, jclass class, jint a, jint b)
{
return a+b;
}
