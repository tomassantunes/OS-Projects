#ifndef _Queue_h
#define _Queue_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MinQueueSize 4
#define Error(Str)        FatalError(Str)
#define FatalError(Str)   fprintf(stderr, "%s\n", Str ), exit(1)

typedef int ElementType;

struct QueueRecord;
typedef struct QueueRecord *Queue;

#endif

struct QueueRecord {
    int Capacity;
    int Front;
    int Rear;
    ElementType *Array;
};

int size(Queue Q) {
    return (Q->Capacity - Q->Front + Q->Rear) % Q->Capacity;
}

int successor(int i, Queue Q) {
    if (i == Q->Capacity - 1) {
        return 0;
    } else {
        return i + 1;
    }
}

int predecessor(int i, Queue Q) {
    if (i == 0) {
        return Q->Capacity - 1;
    } else {
        return i - 1;
    }
}

void MakeEmptyQueue(Queue Q) {
    Q->Front = 0;
    Q->Rear = 0;
}

Queue CreateQueue(int MaxElements) {
    Queue Q;

    if (MaxElements < MinQueueSize)
        Error("Queue size is too small");

    Q = (Queue) malloc(sizeof(struct QueueRecord));
    if (Q == NULL)
        FatalError("Out of space!!!");

    Q->Array = (ElementType*) malloc(sizeof(ElementType) * MaxElements);
    if (Q->Array == NULL)
        FatalError("Out of space!!!");

    Q->Capacity = MaxElements+1;
    MakeEmptyQueue(Q);

    return Q;
}

void DisposeQueue(Queue Q) {
    if (Q != NULL){
        free(Q->Array);
        free(Q);
    }
}

bool IsEmptyQueue(Queue Q) {
    return Q->Front == Q->Rear;
}                               

bool IsFullQueue(Queue Q) {
    return size(Q) == Q->Capacity - 1;
}

void Enqueue(ElementType X, Queue Q) {
    if (IsFullQueue(Q))
        Error("Queue is already full.");
    Q->Array[Q->Rear] = X;
    Q->Rear = successor(Q->Rear, Q);
}

ElementType Front(Queue Q) {
    if (IsEmptyQueue(Q))
        return -1;
    return Q->Array[Q->Front];
}

ElementType Dequeue( Queue Q ) {
    ElementType X;
    if (IsEmptyQueue(Q)) {
        Error("Queue is empty."); 
    } else {
        X = Q->Array[Q->Front];
        Q->Front = successor(Q->Front, Q);
    }
    return X;
}

void Remove(ElementType X, Queue Q) {
    if (IsEmptyQueue(Q)) return;
    
    int i, j;
    for(i = Q->Front, j = 0; j < size(Q); i = successor(i, Q), j++) {
        if (Q->Array[i] == X) {
            Q->Array[i] = Q->Array[successor(i, Q)];
            Q->Rear = predecessor(Q->Rear, Q);
        }
    }
}

void printQueue(Queue Q) {
    if (IsEmptyQueue(Q)) 
        Error("Queue is Empty.");

    for(int i = 0; i < size(Q); i++) {
        printf("%d ", Q->Array[i]);
    }
    printf("\n");
}
