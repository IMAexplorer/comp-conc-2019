#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


#define len 5
#define NTHREADS 2

/* Variaveis globais */

pthread_mutex_t mutex;
pthread_cond_t x_cond;

int N = 0;

struct elt {
    struct elt *next;
    int value;
};

typedef struct queue {
    struct elt *head;  /* dequeue this next */
    struct elt *tail;  /* enqueue after this */
    int maxsize = 5;
    int size;
} queue;

/* create a new empty queue */
queue* init_queue(void)
{
    queue *q;
    q = malloc(sizeof(struct queue));
    q->head = q->tail = 0;
    q->size = 0;
    return q;
}

void enq(queue *q, int value)
{

    if (q->size == q->maxsize) {

    }

    if (q->size < q->maxsize) {
        struct elt *e;
        e = malloc(sizeof(struct elt));
        assert(e);
        e->value = value;
        /* Because I will be the tail, nobody is behind me */
        e->next = 0;

        if(q->head == 0) {
            /* If the queue was empty, become the head */
            q->head = e;
        } else {
            /* Otherwise get in line after the old tail */
            q->tail->next = e;
        }
        /* become the new tail */
        q->tail = e;

        q->size += 1;
    }
}

int ehPrimo(long unsigned int n) {
  int i;
  if(n<=1) return 0;
  if(n==2) return 1;
  if(n%2==0) return 0;
  for(i=3; i< sqrt(n); i+=2) {
    if(n%i==0) return 0;
  }
  return 1;
}

/* Thread Produtora */
void *producer(void * queue){

  int temp;
  Queue *fila = (Queue *) queue;
  fila->fibo1 = 1;
  fila->fibo2 = 1;

  while (fila->numberOfInsertions < 25){

    /* Adquire o lock*/
    pthread_mutex_lock(&x_mutex);

    /* Se fila cheia, aguarda sinal do consumidor para continuar produzindo */
    if(fila->lastPosition == 4){
      pthread_cond_wait(&x_cond, &x_mutex);
    }

    /* adiciona novo valor à fila */
    if(numberOfInsertions == 0){
      fila->buffer[fila->lastPosition] = fila->fibo1;
      fila->lastPosition++;
    }
    else
    {
      fila->buffer[fila->lastPosition] = fila->fibo2;
      temp = fila->fibo1;
      fila->fibo1 = fila->fibo2;
      fila->fibo2 = temp + fila->fibo1;
      fila->lastPosition++
    }

    fila->numberOfInsertions++;

    /* Libera o lock*/
    pthread_mutex_unlock(&x_mutex);

  }

  fila->numberOfInsertions = -1;

  pthread_exit(NULL);

}

/* Thread consumidora */
void *consumer(void * queue){

  	Queue *fila = (Queue *) queue
    int n

    /* Enquanto houver algo na fila para ser consumido */
    while(fila->numberOfInsertions != -1 && fila->buffer[0] != -1){

      /* Adquire o lock */
      pthread_mutex_lock(&x_mutex);

      /* Checa primalidade*/
      if(ehPrimo(fila->buffer[fila->lastPosition])){
        printf("\n O numero %d é primo \n", fila->buffer[fila->lastPosition]);
      }
      else{
        printf("\n O numero %d n é primo \n", fila->buffer[fila->lastPosition]);
      }

      for()

      pthread_mutex_unlock(&x_mutex);

    }


  	/*
  	pthread_mutex_lock(&x_mutex);
    faz algo
    printf("B: x = %d, vai sinalizar a condicao \n", x);
    pthread_cond_broadcast(&x_cond); --> libera todas as threads
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
    */


}

/* Funcao principal */
int main (int argc, char * argv[]){

  pthread_t tid[NTHREADS];
  Queue * fila;

  	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&x_cond, NULL);

  	/* Inicializa a fila com o buffer */
  fila = malloc(sizeof(Queue));
  fila->buffer = mallor(len * sizeof(int));
  fila->lastPosition = 0;
  fila->numberOfInsertions = 0;

  /* Cria as threads */
  if (pthread_create(&tid[0], NULL, producer, (void*) fila) {
        printf("--ERRO: pthread_create()\n"); exit(-1); }

  if (pthread_create(&tid[1], NULL, consumer, (void*) fila) {
        printf("--ERRO: pthread_create()\n"); exit(-1); }

	/* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&x_cond);
	pthread_exit(NULL);
  return 0;
}
