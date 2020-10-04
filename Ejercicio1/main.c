#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_POINTS 100000000

unsigned long int POINTS_IN = 0;

double double_random() { return ((double)rand()) / ((double)RAND_MAX) * 2 - 1; }

void montecarlo() {
    srand(time(NULL));
    for (size_t i = 0; i < N_POINTS; i++) {
        double x = double_random();
        double y = double_random();
        // printf("%f\t %f\n", x, y);
        if ((x * x + y * y) <= 1) {
            POINTS_IN++;
        }
    }
}

int main(int argc, char const* argv[]) {
    montecarlo();
    printf("Total puntos: %d\n", N_POINTS);
    printf("Puntos dentro: %d\n", POINTS_IN);
    double PI = 4 * ((double)POINTS_IN / N_POINTS);
    printf("%f\n", PI);
    return 0;
}
