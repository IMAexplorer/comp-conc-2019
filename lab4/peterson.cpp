// https://stackoverflow.com/questions/43406311/my-peterson-lock-failed-in-this-situation
// https://en.wikipedia.org/wiki/Peterson%27s_algorithm

#include <pthread.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>


typedef struct {
    volatile bool flag[2];
    volatile int victim;
} peterson_lock_t;

void peterson_lock_init(peterson_lock_t &lock) {
    lock.flag[0] = lock.flag[1] = false;
    lock.victim = 0;
}

void peterson_lock(peterson_lock_t &lock, int id) {
 	lock.flag[id] = true; // Mark as B  
    usleep(1); 

    lock.victim = id; // Mark as A
    usleep(1); 

    asm volatile ("mfence" : : : "memory");
    while (lock.flag[1 - id] && lock.victim == id);
}

void peterson_unlock(peterson_lock_t &lock, int id) {
    lock.flag[id] = false;
    lock.victim = id;
}

peterson_lock_t lock;
int count = 0;

void *routine0(void *arg) {
    int *cnt = (int *)arg;
    for (int i = 0; i < *cnt; ++i) {
        peterson_lock(lock, 0);
        ++count;
        peterson_unlock(lock, 0);
    }

    return NULL;
}

void *routine1(void *arg) {
    int *cnt = (int *)arg;
    for (int i = 0; i < *cnt; ++i) {
        peterson_lock(lock, 1);
        ++count;
        peterson_unlock(lock, 1);
    }
    return NULL;
}

int main(int argc, char **argv) {
    peterson_lock_init(lock);
    pthread_t thread0, thread1;
    int count0 = 3000000;
    int count1 = 3000000;
    pthread_create(&thread0, NULL, routine0, (void *)&count0);
    pthread_create(&thread1, NULL, routine1, (void *)&count1);

    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);

    printf("Expected: %d\n", (count0 + count1));
    printf("Reality : %d\n", count);

    return 0;
}