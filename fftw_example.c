/* Start reading here */

#include <fftw3.h>

#define NUM_POINTS 128


/* Never mind this bit */

#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include <random>
#include <chrono>

#define REAL 0
#define IMAG 1

void acquire_from_somewhere(fftw_complex* signal, int offset) {
    /* Generate two sine waves of different frequencies and
     * amplitudes.
     */

    long unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine engine{seed};
    std::uniform_real_distribution<float> distribution{0, 1}; 
    std::uniform_real_distribution<float> distribution2{0, 0.05}; 
    
    int i;
    for (i = 0; i < NUM_POINTS; ++i) {
        double theta = (double)i / (double)NUM_POINTS * M_PI;

        signal[i][REAL] = 1.0 * cos((55.0-28*sin(offset/30.0)+3*distribution(engine)) * theta) +
            0.75 * cos((25.0+15*cos(offset/45.0)+distribution(engine)) * theta) + distribution2(engine);

        signal[i][IMAG] = 0; //1.0 * sin((55.0-28*sin(offset/30.0)+distribution(engine)) * theta) +
            /* 0.75 * cos((25.0+15*cos(offset/45.0)+distribution(engine)) * theta) + distribution2(engine); */
 
    }
}

void do_something_with(fftw_complex* result) {
    int i;
    for (i = 0; i < NUM_POINTS/2; ++i) {
        double mag = sqrt(result[i][REAL] * result[i][REAL] +
                          result[i][IMAG] * result[i][IMAG]) / (0.3*NUM_POINTS);

        /* printf("\n%7.3g   ", mag); */
        while(mag > 0)
        {
            printf("|");
            mag -= 0.05;
        }

        printf("\n");
    }
}


/* Resume reading here */

int main() {
    fftw_complex signal[NUM_POINTS];
    fftw_complex result[NUM_POINTS];

    fftw_plan plan = fftw_plan_dft_1d(NUM_POINTS,
                                      signal,
                                      result,
                                      FFTW_FORWARD,
                                      FFTW_ESTIMATE);

    for(int i = 0; i < 3000; i++)
    {
        printf("\n\n\n\n\n\n");
        acquire_from_somewhere(signal, i);
        fftw_execute(plan);
        do_something_with(result);
        usleep(32000);
    }
    
    fftw_destroy_plan(plan);

    return 0;
}
