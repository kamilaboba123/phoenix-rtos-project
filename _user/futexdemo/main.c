#include <stdio.h>

#include <pthread.h>

#include <sys/futex.h>

void lock(volatile int *l)
{
    int v = __sync_val_compare_and_swap(l, 0, 1);
    if (v != 0) {
        do {
            if (v == 2 || __sync_val_compare_and_swap(l, 1, 2) != 0) {
                futex_wait((int *)l, 2, 0);
            }
        } while((v = __sync_val_compare_and_swap(l, 0, 2)) != 0);
    }
}

void unlock(volatile int *l)
{
    if (__sync_sub_and_fetch(l, 1) != 1) {
        __sync_synchronize();
        *l = 0;
        __sync_synchronize();
        futex_wakeup((int *)l, 1, 1);
    }
}

int counter = 0;
volatile int counter_lock = 0;

void *thread(void *arg)
{
    printf("spawned thread: %d\n", gettid());

    for (int i = 0; i < 10; i++) {
        lock(&counter_lock);
        printf("counter = %d\n", counter);
        counter += 1;
        unlock(&counter_lock);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, &thread, NULL);
    pthread_create(&t2, NULL, &thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

