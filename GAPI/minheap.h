#ifndef _MINHEAP_H_
#define _MINHEAP_H_

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 100

#define NUM_TEST_INSERTS 1000000
#define NUM_TOTAL_OPS 10000000
#define PRINT 0

struct heapStruct {
    int* heaparray;
    int capacity;
    int size;
};

// Used to create a heap.
struct heapStruct *initHeap();
struct heapStruct * initHeapfromArray(int* values, int length);
void heapify(struct heapStruct *h);

// Helper functions.
extern void percolateDown(struct heapStruct *h, int index);
extern void percolateUp(struct heapStruct *h, int index);
extern int insert(struct heapStruct *h, int value);

// Two key functions to implement priority queue.
extern int removeMin(struct heapStruct *h);
extern void printHeap(struct heapStruct *h);

// More helper functions.
extern void swap(struct heapStruct *h, int index1, int index2);
extern int minimum(int a, int indexa, int b, int indexb);
extern void freeHeap(struct heapStruct *h);

// Runs a heap sort.
extern void sort(int values[], int length);

#endif