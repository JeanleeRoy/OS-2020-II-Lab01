#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_POINTS 1000000
#define N_THREADS 6

unsigned long int POINTS_IN = 0;
pthread_mutex_t mutex;

double double_ramdom() { return ((double)rand() / ((double)RAND_MAX)) * 2 - 1; }

void* montecarlo(void* arg) {
    for (size_t i = 0; i < N_POINTS; i++) {
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
    pthread_t child;

    pthread_create(&child, NULL, montecarlo, NULL);

    pthread_join(child, NULL);

    printf("Total puntos \t%d\n", N_POINTS);
    printf("Puntos dentro \t%d\n", POINTS_IN);
    
    double PI = 4 * (double)POINTS_IN / N_POINTS;
    printf("PI aproximado \t%f\n", PI);
    return 0;
}
