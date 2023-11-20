//  count_private.c
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
int ThreadsNumber = 32;
long *threads_independantResults;
int count1s(void);
void *threadCounting(void *id);

int count1s() {
    int local_count = 0;
    for (int i = 0; i < array_length; i++) {
        if (array[i] == 1) {
            local_count++;
        }
    }
    return local_count;
}

void *threadCounting(void *id) {
    long ID = (long)id;
    long numberofIterations = array_length / ThreadsNumber;
    long startIndex = ID * numberofIterations;
    long endIndex = startIndex + numberofIterations;

    if (ID == ThreadsNumber - 1) {
        endIndex = array_length;
    }

    int private_counter = 0;
    for (long i = startIndex; i < endIndex; i++) {
        if (array[i] == 1) {
            private_counter++;
        }
    }

    threads_independantResults[ID] = private_counter;
    return NULL;
}

int main() {
    int count = 0;
    clock_t start_time, check_time1, end_time;

    array = (long *)malloc(array_length * sizeof(long));
    threads_independantResults = (long *)malloc(ThreadsNumber * sizeof(long));

    srand(time(NULL));
    for (long i = 0; i < array_length; i++) {
        array[i] = rand() % 5 + 1;
    }

    start_time = clock();
    correct_count = count1s();
    check_time1 = clock();

    pthread_t T[ThreadsNumber];
    for (int i = 0; i < ThreadsNumber; i++) {
        pthread_create(&T[i], NULL, threadCounting, (void *)(long)i);
    }

    for (int i = 0; i < ThreadsNumber; i++) {
        pthread_join(T[i], NULL);
    }

    end_time = clock();

    for (int j = 0; j < ThreadsNumber; j++) {
        count += threads_independantResults[j];
    }

    double total_Time_sequential = ((double)(check_time1 - start_time)) / CLOCKS_PER_SEC;
    double total_Time_parallel = ((double)(end_time - check_time1)) / CLOCKS_PER_SEC;

    printf(" # Threads: %d\n", ThreadsNumber);
    printf("Time Taken sequentially: %f\n", total_Time_sequential);
    printf("Correct count of 1's= %d\n", correct_count);
    printf("Time Taken - parallel programming: %f\n", total_Time_parallel);
    printf("Count of 1s with multiple threads: %d\n", count);

    free(array);
    free(threads_independantResults);

    return 0;
}
