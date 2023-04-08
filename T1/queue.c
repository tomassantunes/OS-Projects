#include "queue.h"

#define MinQueueSize 4
#define Error(Str)        FatalError(Str)
#define FatalError(Str)   fprintf(stderr, "%s\n", Str ), exit(1)

struct QueueRecord{
    int Capacity;
    int Front;
    int Rear;
    ElementType *Array;
};

/* FUNCOES AUXILIARES */
/* numero de elementos na fila */
int size( Queue Q ){
    return ( Q->Capacity - Q->Front + Q->Rear ) % Q->Capacity;
}

/* indice do proximo elemento  */
int successor( int i, Queue Q ){
    if (i == Q->Capacity - 1){
        return 0;
    } else {
        return i + 1;
    }
}

int predecessor(int i, Queue Q) {
    if(i == 0) {
        return Q->Capacity - 1;
    } else {
        return i - 1;
    }
}

/* FUNCOES DE MANIPULACAO DE QUEUES */
Queue CreateQueue( int MaxElements ){
    Queue Q;

    if( MaxElements < MinQueueSize )
        Error( "Queue size is too small" );

    Q = malloc( sizeof( struct QueueRecord ) );
    if( Q == NULL )
        FatalError( "Out of space!!!" );

    Q->Array = malloc( sizeof( ElementType ) * MaxElements );
    if( Q->Array == NULL )
        FatalError( "Out of space!!!" );

    Q->Capacity = MaxElements+1;
    MakeEmptyQueue( Q );

    return Q;
}

void DisposeQueue( Queue Q ){
    if( Q != NULL ){
        free( Q->Array );
        free( Q );
    }
}

bool IsEmptyQueue( Queue Q ){ // verificar se a queue estiver vazia
    return Q->Front == Q->Rear; // se o front e o rear forem iguais a queue está vazia
}                               

bool IsFullQueue( Queue Q ){ // verificar se a queue está cheia
    return size( Q ) == Q->Capacity - 1;
}

void MakeEmptyQueue( Queue Q ){ // esvaziar a queue
    Q->Front = 0; // para esvaziar a queue temos de dar reset
    Q->Rear = 0;  // às variáveis front e rear, dando-lhes o valor 0
}

void Enqueue( ElementType X, Queue Q ){ // adicionar um elemento à queue
    if(IsFullQueue(Q))
        Error("Queue is already full.");
    Q->Array[Q->Rear] = X; // definir o último elemento (na posição rear)
    Q->Rear = successor(Q->Rear, Q); // definir o próximo índice com a função successor
}

ElementType Front( Queue Q ){ // mostrar o primeiro elemento da queue
    if(IsEmptyQueue(Q))
        return -1;
    return Q->Array[Q->Front];
}

ElementType Dequeue( Queue Q ){ // remover o primeiro elemento da queue
    ElementType X;          // respeitando o formato First In First Out
    if(IsEmptyQueue(Q)){
        Error("Queue is empty."); 
    } else {
        X = Q->Array[Q->Front];
        Q->Front = successor(Q->Front, Q); // para remover o primeiro elemento adicionamos 1 
    }                                      // ao índice front
    return X;
}

void Remove(ElementType X, Queue Q) {
    if(IsEmptyQueue(Q)) return;
    
    int i, j;
    bool found = false;

    // iterate over the elements in the queue
    for(i = Q->Front, j = 0; j < size(Q); i = successor(i, Q), j++) {
        if(Q->Array[i] == X) {
            found = true;
            break;
        }
    }

    if(found) {
        // shift the remaining elements down one position to fill the gap left by the removed element
        for(; i != Q->Rear; i = successor(i, Q)) {
            Q->Array[i] = Q->Array[successor(i, Q)];
        }
        Q->Rear = predecessor(Q->Rear, Q); // decrement Rear after removing the element
    }
}

void printQueue(Queue Q) {
    if(IsEmptyQueue(Q)) 
        Error("Queue is Empty.");

    for(int i = 0; i < size(Q); i++) {
        printf("%d ", Q->Array[i]);
    }
    printf("\n");
}
