#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_POINTS 1000000
#define N_THREADS 5

unsigned long int POINTS_IN = 0;
pthread_mutex_t mutex;

double double_ramdom() { return ((double)rand() / ((double)RAND_MAX)) * 2 - 1; }

void* montecarlo(void* arg) {
    int points = *(int*)arg;

    for (size_t i = 0; i < points; i++) {
        double x = double_ramdom(), y = double_ramdom();
        int is_inside = (x * x + y * y) <= 1;

        pthread_mutex_lock(&mutex);
        if (is_inside) {
            POINTS_IN++;
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char const* argv[]) {
    srand(time(NULL));

    pthread_t th_children[N_THREADS];
    int points_per_thread = N_POINTS / N_THREADS;
    int points_remaining = N_POINTS % N_THREADS;

    pthread_mutex_init(&mutex, NULL);

    printf("Puntos por hilo\t%d ", points_per_thread);

    if (points_remaining) {
        printf("aprox.\n-> se reparten los %d puntos restantes", points_remaining);
    }

    for (size_t i = 0; i < N_THREADS; i++) {
        int points =
            i < points_remaining ? points_per_thread + 1 : points_per_thread;
        pthread_create(&th_children[i], NULL, montecarlo, (void*)&points);
    }

    for (size_t i = 0; i < N_THREADS; i++) {
        pthread_join(th_children[i], NULL);
    }

    printf("\nTotal puntos \t%d\n", N_POINTS);
    printf("Puntos dentro \t%d\n", POINTS_IN);

    double PI = 4 * (double)POINTS_IN / N_POINTS;
    printf("\nPI aproximado \t%f\n\n", PI);

    return 0;
}
