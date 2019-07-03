#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ansicolors.h"

#define N 5 // number of philosophers

#define MODULO(n) ((((n) % N) + N) % N)
#define LEFT  (MODULO(ph_num + 0))
#define RIGHT (MODULO(ph_num - 1))

pthread_mutex_t chopsticks[N]; // mutexes controling the access to each chopstick

void *philosopher(void *num);
void take_chopstick(int);
void put_chopstick(int);


int main()
{
    int ph_num;
    for (ph_num = 0; ph_num < N; ph_num++) {
        printf(ANSI_TEXT_COLOR_BLUE "Philosopher %d needs left chopstick %d and then right chopstick %d\n" ANSI_RESET, ph_num + 1, LEFT + 1, RIGHT + 1);
    }

    int phil_num[N];
    int i;
    pthread_t thread_id[N]; // each thread will simulate the behavior of one philosopher

    for (i = 0; i < N; i++)
        pthread_mutex_init(&chopsticks[i], NULL);

    for (i = 0; i < N; i++)
    {
        phil_num[i] = i;
        pthread_create(&thread_id[i], NULL, philosopher, &phil_num[i]);
    }

    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    for (i = 0; i < N; i++)
        pthread_mutex_destroy(&chopsticks[i]);
}

void *philosopher(void *num)
{
    int i = *((int*)num);
    printf("Philosopher %d is thinking\n", i + 1);
    while (1)
    {
        take_chopstick(i);
        sleep(0);
        put_chopstick(i);
    }
}

void take_chopstick(int ph_num)
{
		// para que haja melhor eficiência faz com que um dos filosofos procure o garfo direito primeiro
		if (ph_num != 0) {
			// waiting for left chopstick
	    printf("Philosopher %d is Hungry and waits for chopstick %d\n", ph_num + 1, LEFT + 1);
	    pthread_mutex_lock(&chopsticks[LEFT]);
	    // got left chopstick
	    printf("Philosopher %d got chopstick %d\n", ph_num + 1, LEFT + 1);

	    sleep(1);

	    // waiting for right chopstick
	    printf("Philosopher %d now waits for chopstick %d\n", ph_num + 1, RIGHT + 1);
	    pthread_mutex_lock(&chopsticks[RIGHT]);
	    // got right chopstick
	    printf("Philosopher %d took chopstick %d and %d, and now is Eating\n", ph_num + 1, LEFT + 1, RIGHT + 1);
		} else {
			// waiting for right chopstick
	    printf("Philosopher %d now waits for chopstick %d\n", ph_num + 1, RIGHT + 1);
	    pthread_mutex_lock(&chopsticks[RIGHT]);
	    // got right chopstick
	    printf("Philosopher %d took chopstick %d and %d, and now is Eating\n", ph_num + 1, LEFT + 1, RIGHT + 1);

	    sleep(1);

			// waiting for left chopstick
	    printf("Philosopher %d is Hungry and waits for chopstick %d\n", ph_num + 1, LEFT + 1);
	    pthread_mutex_lock(&chopsticks[LEFT]);
	    // got left chopstick
	    printf("Philosopher %d got chopstick %d\n", ph_num + 1, LEFT + 1);
		}
}

void put_chopstick(int ph_num)
{
    printf("Philosopher %d putting chopstick %d and %d down\n", ph_num + 1, LEFT + 1, RIGHT + 1);
    printf("Philosopher %d is thinking\n", ph_num + 1);
    pthread_mutex_unlock(&chopsticks[LEFT]);
    sleep(1);
    pthread_mutex_unlock(&chopsticks[RIGHT]);
    sleep(1);
}
