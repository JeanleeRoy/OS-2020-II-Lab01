#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long int fibo_t;
#define FIBO_MAX 93  // maximun 94

sem_t position_free;
sem_t position_taken;
fibo_t *fibo_numbers;

void fibonacci_init(int n);
fibo_t m_fibonacci(int n);

void *f_parent(void *v) {
    int number = *(int *)v;
    sem_wait(&position_taken);
    printf("Los primeros %d numeros Fibonacci:\n", number);
    for (size_t i = 0; i < number; i++) {
        printf("%lu\n", fibo_numbers[i]);
    }
    return NULL;
}

void *f_child(void *v) {
    int number = *(int *)v;
    sem_wait(&position_free);
    m_fibonacci(number);
    sem_post(&position_taken);
    return NULL;
}

int main(int argc, char const *argv[]) {
    if (argc == 1 || argc > 2) {
        fprintf(stderr, "Error: Se esperaba un argumento\n");
        exit(-1);
    }

    int number = atoi(argv[1]);
    fibonacci_init(number);
    sem_init(&position_free, 0, 1);
    sem_init(&position_taken, 0, 0);

    pthread_t parent;
    pthread_t child;
    pthread_create(&parent, NULL, f_parent, (void *)&number);
    pthread_create(&child, NULL, f_child, (void *)&number);

    sem_destroy(&position_free);
    sem_destroy(&position_taken);

    pthread_join(child, NULL);
    pthread_join(parent, NULL);

    free(fibo_numbers);
    fibo_numbers = NULL;

    return 0;
}

void fibonacci_init(int n) {
    if (n > FIBO_MAX || n <= 0) {
        fprintf(stderr, "Operacion invalida: Fuera de limite\n");
        exit(-1);
    }

    fibo_numbers = malloc((n + 1) * sizeof(fibo_t));
    fibo_numbers[0] = 0;
    fibo_numbers[1] = 1;
    for (size_t i = 2; i <= n; i++) {
        fibo_numbers[i] = 0;
    }
}

fibo_t m_fibonacci(int n) {
    if (n != 0 && fibo_numbers[n] == 0) {
        fibo_t result = m_fibonacci(n - 1) + m_fibonacci(n - 2);
        fibo_numbers[n] = result;
    }
    return fibo_numbers[n];
}
