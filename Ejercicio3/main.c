#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long int fibo_t;
#define FIBO_MAX 93  // maximun 94

pthread_mutex_t mutex;
fibo_t* fibo_numbers;

void fibonacci_init(int n);

fibo_t m_fibonacci(int n) {
    if (n != 0 && fibo_numbers[n] == 0) {
        fibo_t result = m_fibonacci(n - 1) + m_fibonacci(n - 2);

        // pthread_mutex_lock(&mutex);
        fibo_numbers[n] = result;
        // pthread_mutex_unlock(&mutex);
    }
    return fibo_numbers[n];
}

void* fibonacci(void* n) {
    int number = *(int*)n;
    m_fibonacci(number);
    return NULL;
}

int main(int argc, char const* argv[]) {
    if (argc == 1 || argc > 2) {
        fprintf(stderr, "Error: Se esperaba un argumento\n");
        exit(-1);
    }

    int number = atoi(argv[1]);
    fibonacci_init(number);

    // pthread_mutex_init(&mutex, NULL);
    pthread_t child;
    pthread_create(&child, NULL, fibonacci, (void*)&number);
    pthread_join(child, NULL);

    printf("Los primeros %d numeros Fibonacci:\n", number);
    for (size_t i = 0; i < number; i++) {
        printf("%lu\n", fibo_numbers[i]);
    }

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
