// https://stackoverflow.com/questions/1383363/is-my-spin-lock-implementation-correct-and-optimal
#include "stdio.h"
#include "pthread.h"
#include <iostream>
#include <atomic>
#include <thread>
#include <time.h>

volatile int exclusion = 0, number = 0;

inline void unlock() {

	__sync_synchronize(); // Memory barrier.
    exclusion = 0;
}

inline void lock(volatile int *exclusion)
{
    while(true) {
    	while (*exclusion) {}   ; // spin
    	if (!(__sync_lock_test_and_set(exclusion, 1)))
    		return;
    }
}
void * increment(void *id)
{
    int myId = (int)(size_t)(id);
    for (int i = 0; i < 100000000; ++i)    {
		lock(&exclusion);
        ++number;
		unlock();
    }
    return 0;
}
int main()
{
    pthread_t threads[2];
    double startTime, timeElapsed, endTime;
    startTime = ((float)clock())/CLOCKS_PER_SEC;
		// cria duas threads
		for (int i = 0; i < 2; ++i)
        pthread_create(&threads[i], 0, increment, (void*)(uint64_t)(i));
		// espera pelo termino da execução das threads
		for (int i = 0; i < 2; ++i)
        pthread_join(threads[i], 0);
    endTime = ((float)clock())/CLOCKS_PER_SEC;
	timeElapsed = endTime - startTime;
	fprintf(stderr, "Final value: %d Elapsed time: %f seconds\n",number,timeElapsed);
    return 0;
}
