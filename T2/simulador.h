#ifndef _Simulador_h
#define _Simulador_h

#include "queue.h"
#include "string.h"

#define MAX 100
#define NUMPROGRAMS 3
#define NUMPROCESS 10

#define NONE -1
#define NOTCREATED -1
#define NEW 0
#define READY 1
#define RUN 2
#define BLOCKED 3
#define EXIT 4
#define FINISHED 5
#define UNBLOCK 6

typedef struct {
    int start;
    int exec;
    int state;
    int* time;
} Program;

int notFinished();
void changeState(int pid);
void showState(int pid);
void run();
void createPrograms(int p[NUMPROGRAMS][NUMPROCESS]);

#endif
