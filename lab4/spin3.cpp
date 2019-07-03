// https://stackoverflow.com/questions/1383363/is-my-spin-lock-implementation-correct-and-optimal
#include "stdio.h"
#include "pthread.h"
#include <iostream>
#include <atomic>
#include <thread>
#include <time.h>
volatile int exclusion = 0, number = 0;

inline void unlock(volatile int *locked);

inline void lock(volatile int *locked) {
    while (__sync_val_compare_and_swap(locked, 0, 1));
    __sync_synchronize(); // Memory barrier.
}

inline void unlock(volatile int *locked) {
    *locked=0;
    __sync_synchronize(); // Memory barrier.
}


void * increment(void *id)
{
    int myId = (int)(size_t)(id);
    for (int i = 0; i < 20000000; ++i)    {
		lock(&exclusion);
        ++number;
		unlock(&exclusion);
    }
    return 0;
}
int main()
{
    pthread_t threads[2];
    int startTime, timeElapsed, endTime;
    startTime = (float)clock()/CLOCKS_PER_SEC;
    for (int i = 0; i < 2; ++i)
        pthread_create(&threads[i], 0, increment, (void*)(uint64_t)(i));
    for (int i = 0; i < 2; ++i)
        pthread_join(threads[i], 0);
    endTime = (float)clock()/CLOCKS_PER_SEC;
	timeElapsed = endTime - startTime;
	fprintf(stderr, "Final value: %d Elapsed time: %d seconds\n",number,timeElapsed);
    return 0;
}
