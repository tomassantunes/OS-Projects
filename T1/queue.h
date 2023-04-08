#ifndef _Queue_h
#define _Queue_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "simulador.h"

typedef int ElementType;

struct QueueRecord;
typedef struct QueueRecord *Queue;

Queue CreateQueue( int MaxElements );
void DisposeQueue( Queue Q );

bool IsEmptyQueue( Queue Q );
bool IsFullQueue( Queue Q );
void MakeEmptyQueue( Queue Q );

ElementType Front( Queue Q );
void Enqueue( ElementType X, Queue Q );
ElementType Dequeue( Queue Q );
void Remove(ElementType X, Queue Q);

#endif
