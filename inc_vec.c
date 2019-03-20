#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define NTHREADS 2
#define VECTORSIZE 50

// estrutura de dados para passar argumentos
typedef struct {
  int size;
  int * addr;
} inc_args;

// função que irá incrementar
void *inc_vector(void *args) {
  printf("thread iniciada\n");
  // faz o cast
  inc_args *vec_args = (inc_args*) args;
  // inteiro para loop
  int i;
  // incrementa itens do vetor
  for (i = 0; i < vec_args->size; i++)
    vec_args->addr[i] += 1;
  // libera memória
  free(vec_args);

  pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

  // cria um vetor com duas threads
  pthread_t inc_threads[NTHREADS];
  int i,t;
  inc_args *arg;
  // vector com números
  int vetor[VECTORSIZE];
  // popula o vetor
  for ( i = 0; i < VECTORSIZE; i++ )
    vetor[i] = i;

  // imprime os primeiros itens
  for (i = 0; i < 10; i++)
    printf("item %d - %d\n", i, vetor[i]);

  // aloca argumentos e cria threads
  for(t=0; t < NTHREADS; t++) {
   // aloca a memória para os argumentos
    arg = malloc(sizeof(inc_args));
    if (arg == NULL)
      printf("--ERRO: malloc()\n"); exit(-1);
    // passa os argumentos
    arg->size = VECTORSIZE;
    arg->addr = vetor + (t * (VECTORSIZE/NTHREADS));
    // cria as threads
    printf(" cria thread %d\n", t+1);
    if (pthread_create(&inc_threads[t], NULL, inc_vector, (void*) arg))
      printf("ERRO: pthread_create()\n"); exit(-1);

  }

  for (i = 0; i < NTHREADS; i++) {
    if (pthread_join(inc_threads[i], NULL)) {
      printf("Erro: pthread_join \n"); exit(-1);
    }
  }

  // imprime novos valores do vetor
  for (i = 0; i < VECTORSIZE; i++)
    printf("item %d - %d\n", i, vetor[i]);

  printf("Thread principal terminou\n");

  return 0;
}
