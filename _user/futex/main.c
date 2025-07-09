#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/futex.h>

volatile int futex = 0;

void *thread1(void *arg)
{
	printf("thread1\n");
	printf("thread1: Before futex_wait()\n");
	int err = futex_wait((unsigned int *)&futex, 0, 5);
	printf("thread1: After futex_wait()\n");
	printf("thread1: err = %d\n", err);
	return NULL;
}

void *thread2(void *arg)
{
	printf("thread1\n");
	printf("thread2: Before futex_wakeup()\n");
	int err = futex_wakeup((unsigned int *)&futex, 1);
	printf("thread2: After futex_wakeup()\n");
	printf("thread2: err = %d\n", err);
	return NULL;
}

int main(void)
{
	printf("Futex test application\n");

	pthread_t t1;
	pthread_create(&t1, NULL, &thread1, NULL);
	pthread_t t2;
	pthread_create(&t2, NULL, &thread2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}
