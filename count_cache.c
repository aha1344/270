//  count_cache.c
//  Assignment5
//
//  Created by Ahmad on 19/11/2023.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct fill_cache {
    long id;
    char *characters;
};

long *array;
long array_length = 1000000000;
int correct_count = 0;
int ThreadsNumber = 32;
long *threads_independantResults;

int count1s() {
    int local_count = 0;
    for (int i = 0; i < array_length; i++) {
        if (array[i] == 1) {
            local_count++;
        }
    }
    return local_count;
}

void *threadCounting(void *my_struct) {
    struct fill_cache *struct_ptr = (struct fill_cache *)my_struct;
    long numberofIterations = array_length / ThreadsNumber;
    long startIndex = struct_ptr->id * numberofIterations;
    long endIndex = startIndex + numberofIterations;

    int private_counter = 0;
    int remaining_bytes = 256000 - ((numberofIterations * 8) + (3 * sizeof(long)) + sizeof(int));
    struct_ptr->characters = (char *)malloc(remaining_bytes * sizeof(char));

    if (array_length - endIndex < numberofIterations) {
        endIndex = array_length;
    }

    for (long i = startIndex; i < endIndex; i++) {
        if (array[i] == 1) {
            private_counter++;
        }
    }

    threads_independantResults[struct_ptr->id] = private_counter;
    free(struct_ptr->characters);
    return NULL;
}

int main() {
    int count = 0;

    clock_t start_time, end_time;
    array = (long *)malloc(array_length * sizeof(long));
    threads_independantResults = (long *)malloc(ThreadsNumber * sizeof(long));

    srand(time(NULL));
    for (long i = 0; i < array_length; i++) {
        array[i] = rand() % 5 + 1;
    }

    start_time = clock();
    correct_count = count1s();

    pthread_t T[ThreadsNumber];
    struct fill_cache fill[ThreadsNumber];

    for (int i = 0; i < ThreadsNumber; i++) {
        fill[i].id = i;
        pthread_create(&T[i], NULL, threadCounting, &fill[i]);
    }

    for (int i = 0; i < ThreadsNumber; i++) {
        pthread_join(T[i], NULL);
    }

    end_time = clock();

    for (int j = 0; j < ThreadsNumber; j++) {
        count += threads_independantResults[j];
    }

    double total_Time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("# of Threads: %d\n", ThreadsNumber);
    printf("Time Taken: %f\n", total_Time);
    printf("Correct count of 1's= %d\n", correct_count);
    printf("Count of 1s with multiple threads: %d\n", count);

    free(array);
    free(threads_independantResults);
    return 0;
}
