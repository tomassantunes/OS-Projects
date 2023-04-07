#ifndef _Simulador_h
#define _Simulador_h

#define NUMSTATES 8

typedef struct {
    int start;
    int exec;
    int state;
    int* time;
} Program;

#endif
