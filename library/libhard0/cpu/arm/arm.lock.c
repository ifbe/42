int snatch(int* lock)
{
	while(__sync_lock_test_and_set(lock,1) == 1);
}
int release(int* lock)
{
	__sync_lock_release(lock);
}
