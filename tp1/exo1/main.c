#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <cpuid.h>
#include <stdbool.h>

void change_affinity(int cpu, pthread_t thread) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    pthread_setaffinity_np(thread, sizeof(mask), &mask);
    printf("Changed affinity of thread %lu to CPU %d\n", thread, cpu); // Log the change of affinity
}

void mange_cpu(void) {
    /*
     On mange du CPU en mettant du code qui ne devrait pas
     etre supprimé par le compilateur
     */
    int i = 1;
    time_t start_time = time(NULL); // Get the current time

    while (1) {
        i = i * 2 + 1;
        if (i % 2 == 0)
            break;

        if (time(NULL) - start_time >= 5) // If 10 seconds have passed
            break;
    }
    printf("CPU has been consumed for 10 seconds\n"); // Log the CPU consumption
}

int main(void) {
    bool same_core = false;

    while (true){
        printf("Starting new iteration\n"); // Log the start of the iteration
        // create 2 threads
        pthread_t thread1;
        pthread_t thread2;
        pthread_create(&thread1, NULL, (void *(*)(void *)) mange_cpu, NULL);
        pthread_create(&thread2, NULL, (void *(*)(void *)) mange_cpu, NULL);
        // set the affinity of the threads to CPU 1
        if (same_core) {
            printf("Threads are on the same core\n");
            change_affinity(1, thread1);
            change_affinity(1, thread2);
        } else {
            printf("Threads are on different cores\n");
            change_affinity(0, thread1);
            change_affinity(1, thread2);
        }
        // wait for the threads to finish
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        printf("Program ended\n"); // Log the end of the program
        same_core = !same_core;
    }
    return 0;
}