//  Count_mutex.c
//  Assignment5
//
//  Created by Ahmad on 19/11/2023.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long *array;
long array_length = 1000000000;
int correct_count = 0;
int count = 0;
int ThreadsNumber = 32;
pthread_mutex_t lock;
int count1s(void);
void *threadCounting(void *id);

int count1s() {
    int local_correct_count = 0;

    for (int i = 0; i < array_length; i++) {
        if (array[i] == 1) {
            local_correct_count++;
        }
    }
    return local_correct_count;
}

void *threadCounting(void *id) {
    long ID = (long)id;
    long numberofIterations = array_length / ThreadsNumber;
    long startIndex = ID * numberofIterations;
    long endIndex = startIndex + numberofIterations;
    
    if (ID == ThreadsNumber - 1) {
        endIndex = array_length;
    }

    for (long i = startIndex; i < endIndex; i++) {
        if (array[i] == 1) {
            pthread_mutex_lock(&lock);
            count++;
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&lock, NULL);

    clock_t start_time, check_time1, end_time;
    
    array = (long*)malloc(array_length * sizeof(long));
    if (!array) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    srand(time(NULL));
    for (long i = 0; i < array_length; i++) {
        array[i] = rand() % 5 + 1;
    }
    start_time = clock();
    correct_count = count1s();
    check_time1 = clock();

    pthread_t threads[ThreadsNumber];
    for (int i = 0; i < ThreadsNumber; i++) {
        if (pthread_create(&threads[i], NULL, threadCounting, (void *)(long)i)) {
            perror("Failed to create thread");
            free(array);
            pthread_mutex_destroy(&lock);
            return 1;
        }
    }
    for (int i = 0; i < ThreadsNumber; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("Failed to join thread");
        }
    }

    end_time = clock();
    double total_Time_sequential = (double)(check_time1 - start_time) / CLOCKS_PER_SEC;
    double total_Time_parallel = (double)(end_time - check_time1) / CLOCKS_PER_SEC;

    printf(" # Threads: %d\n", ThreadsNumber);
    printf("Time Taken sequentially: %f\n", total_Time_sequential);
    printf("Correct count of 1's= %d\n", correct_count);
    printf("Time Taken - parallel programming: %f\n", total_Time_parallel);
    printf("Count of 1s with multiple threads: %d\n", count);
    free(array);
    pthread_mutex_destroy(&lock);

    return 0;
}
