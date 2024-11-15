#define _GNU_SOURCE

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

#define NUM_THREADS 4
#define NUM_ITERATIONS 100000000
static __thread unsigned long x;
static __thread unsigned long y = 362436069;
static __thread unsigned long z = 521288629;
unsigned long long results_array[4];
struct results_with_padding {
    unsigned long result0;
    char padding1[PADDING];
    unsigned long result1;
    char padding2[PADDING];
    unsigned long result2;
    char padding3[PADDING];
    unsigned long result3;
} __attribute__ ((packed)) results_with_padding __attribute__ ((aligned (64)));

unsigned long xorshf96(void) {
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

void *benchmark_and_store_in_global_array(void *threadid) {
    long tid;
    tid = (long) threadid;

    // Set thread affinity
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(tid, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    // Initialize x with thread id and current timestamp
    struct timeval tv;
    gettimeofday(&tv, NULL);
    x = tid + tv.tv_usec;

    // Call xorshf96() a large number of times and add result modulo 2 to results_array[tid]
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        unsigned long result = xorshf96();
        results_array[tid] += result % 2;
    }

    pthread_exit(NULL);
}

void *benchmark_and_store_in_struct(void *threadid) {
    long tid;
    tid = (long) threadid;

    // Set thread affinity
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(tid, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    // Initialize x with thread id and current timestamp
    struct timeval tv;
    gettimeofday(&tv, NULL);
    x = tid + tv.tv_usec;

    // Call xorshf96() a large number of times and add result modulo 2 to results_array[tid]
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        unsigned long result = xorshf96();
        switch (tid) {
            case 0:
                results_with_padding.result0 += result % 2;
                break;
            case 1:
                results_with_padding.result1 += result % 2;
                break;
            case 2:
                results_with_padding.result2 += result % 2;
                break;
            case 3:
                results_with_padding.result3 += result % 2;
                break;
            default:
                break;
        }
    }

    pthread_exit(NULL);
}

// function to convert the results from the struct to the array
void convert_struct_to_array() {
    results_array[0] = results_with_padding.result0;
    results_array[1] = results_with_padding.result1;
    results_array[2] = results_with_padding.result2;
    results_array[3] = results_with_padding.result3;
}


void calculate_stats(const unsigned long long *results) {
    // Print results
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %d: %llu\n", i, results[i]);
    }
    // calculate standard deviation between threads
    unsigned long long sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        sum += results[i];
    }
    double mean = (double) sum / NUM_THREADS;
    double variance = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        variance += pow(((double) results[i] - mean), 2);
    }
    double std_dev = sqrt(variance / NUM_THREADS);
    printf("Standard deviation: %f\n", std_dev);
}


int main() {
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    printf("USE_STRUCT: %d\n", USE_STRUCT);
    printf("NUM_THREADS: %d\n", NUM_THREADS);
    printf("NUM_ITERATIONS: %d\n", NUM_ITERATIONS);
    printf("PADDING: %d\n", PADDING);


    for (t = 0; t < NUM_THREADS; t++) {
        if (USE_STRUCT) {
            rc = pthread_create(&threads[t], NULL, benchmark_and_store_in_struct, (void *) t);
        } else {
            rc = pthread_create(&threads[t], NULL, benchmark_and_store_in_global_array, (void *) t);
        }
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = ((double) end.tv_sec - (double) start.tv_sec) * 1e9;
    time_taken = (time_taken + ((double) end.tv_nsec - (double) start.tv_nsec)) * 1e-9;
    printf("Execution time: %.6f seconds\n", time_taken);
    if (USE_STRUCT) {
        convert_struct_to_array();
    }
    calculate_stats(
            results_array
    );
    return 0;
}

