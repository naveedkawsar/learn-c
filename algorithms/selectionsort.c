/*
 * Selection sort implementation in C
 * Time complexity: O(n^2) due to nested for-loops
 * Space complexity: O(1)
 * Created by Naveed Kawsar
 * Date: Oct 25 2017
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define ARRAYSIZE(a)            (sizeof(a) / sizeof(a[0]))
#define DEFAULT_ARRAY_LENGTH    50

static void fill_array(int32_t *arr, ssize_t length);
static void selection_sort(int32_t *arr, ssize_t length);
static void print_array(int32_t *arr, ssize_t length);
static void swap(int32_t *x, int32_t *y);

int main(int argc, char *argv[])
{
    int arr_length;
    struct timespec start, end;
    
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [arr_length]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 2)
        arr_length = atoi(argv[1]);
    else
        arr_length = DEFAULT_ARRAY_LENGTH;

    int32_t arr[arr_length];
    
    fill_array(arr, ARRAYSIZE(arr));
    printf("Contents of original array: \n");
    print_array(arr, ARRAYSIZE(arr));
   
    clock_gettime(CLOCK_MONOTONIC, &start);
    selection_sort(arr, ARRAYSIZE(arr));
    clock_gettime(CLOCK_MONOTONIC, &end);

    uint64_t elapsed = ((uint64_t)1e9 * ((uint64_t)end.tv_sec - (uint64_t)start.tv_sec)) 
                                    + (uint64_t)end.tv_nsec - (uint64_t)start.tv_nsec;
    printf("Contents of sorted array: \n");
    print_array(arr, ARRAYSIZE(arr));
    
    printf("Time required to sort: %" PRIu64 "ns\n", elapsed);
    exit(EXIT_SUCCESS);
}

void fill_array(int32_t *arr, ssize_t length)
{
    srand(time(NULL));
    for (ssize_t i = 0; i < length; i++) {
        *(arr+i) = rand();
    }

}

void selection_sort(int32_t *arr, ssize_t length)
{
    int min;
    /* Move through array  */
    for (ssize_t i = 0; i < length - 1; i++) {
        min = i;
        /* 
         * Find smallest element in array by comparing 
         * local min with elements on the right
         */
        for (ssize_t j = i+1; j < length; j++) {
            if (*(arr+j) < *(arr+min)) {
                min = j;
            }
        }
        /* Swap new minimum with current element */
        swap(&arr[min], &arr[i]);
    }

}

void print_array(int32_t *arr, ssize_t length)
{
    for (ssize_t i = 0; i < length; i++) {
        printf("%" PRId32 "\t\t", *(arr+i));
    }
    printf("\n");
}

void swap(int32_t *x, int32_t *y)
{
    int32_t tmp = *x;
    *x = *y;
    *y = tmp;
}
