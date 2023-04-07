#include "simulador.h"
#include "queue.c"
#include "queue.h"
#include <bits/types/cookie_io_functions_t.h>

/***************** 
 *  - 0 -> NEW
 *  - 1 -> READY
 *  - 2 -> RUN
 *  - 3 -> BLOCKED
 *  - 4 -> EXIT
 *****************/

#define MAX 100
#define NUMPROCESS 8
#define NUMPROGRAMS 3
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define NOTCREATED -1
#define NEW 0
#define READY 1
#define RUN 2
#define BLOCKED 3
#define EXIT 4

Program programs[NUMPROGRAMS + 1];
Queue Q;
int instant = 0;

void runProgram(int p[]) {
    int procCount = 0;
    Queue program = CreateQueue(NUMPROCESS);

    for(int i = 0; i < NUMPROCESS; i++) {
        if(p[i] > 0) {
            Program process;
            // process.state = "NEW";
            // process.time = p[i];
            // process.num = i + 1;
            Enqueue(process, program);
            procCount++;
        }
    }

    printf("instance | ");
    for(int i = 1; i <= procCount; i++) {
        printf("proc%d | ", i);
    }
    printf("\n");

    int i = 1;
    while(!IsEmptyQueue(program)) {
        Program process = Dequeue(program);
        procCount--;

        for(; process.time > 0; process.time--) {
            if(i < 10) printf("%d        ", i++);
            else printf("%d       ", i++);

            // run(&process);
            for(int j = 0; j < procCount; j++) {
                // printf("%s   ", program->Array[j].state);
            }
            printf("\n");
        }
    }
    printf("\n");
    DisposeQueue(program);
}

int checkRunning() {
    int n = 0;
    for(int i = 0; i < NUMPROGRAMS; i++) {
        if(programs[i].state != 4) {
            n++;
        }
    }

    return n;
}

void changeState(int pid) {
    switch(programs[pid].state) {
        case NOTCREATED: // process not created -> NEW
            if(programs[pid].start >= instant)
                programs[pid].state = NEW;
            break;
        case NEW:  // NEW -> READY
            programs[pid].state = READY;
            Enqueue(pid, Q);
            break;
        case READY:  // READY -> RUN
            programs[pid].state = RUN;
            break;
        case RUN:  // RUN -> BLOCKED
            programs[pid].state = BLOCKED;
            Enqueue(pid, Q);
            break;
        case BLOCKED:  // BLOCKED -> RUN
            programs[pid].state = RUN;
            break;
    }
}

void run() {
    // p->state = "RUN";
    //
    // for(int i = 1; i < p->num; i++) {
    //   printf("        ");
    // }
    //
    // printf("   %s    ", p->state);
    
    Q = CreateQueue(MAX);
    
    for(int i = 0; i < NUMPROGRAMS; i++) {
        changeState(i);
    }

    int nRunning = checkRunning();
    instant++;

    while(nRunning != 0) {
        printf("%d |", instant);

        for(int i = 0; i < NUMPROGRAMS; i++) {
            if(programs[i].state == RUN || (Q->Front = i && programs[i].state == BLOCKED)) {
                programs[i].time[programs[i].exec]--;
            }
        }

        /*! TODO:
         *
         * @todo create function to process programs state
         */
    }

}

void createPrograms(int p[NUMPROGRAMS][NUMPROCESS]) {
    for(int i = 0; i < NUMPROGRAMS; i++) {
        programs[i].time = p[i] + 1;
        programs[i].exec = 1;
        programs[i].start = p[i][0];
        programs[i].state = NOTCREATED;

        if(i < 9) {
            printf("proc0%d | ", i+1);
        } else {
            printf("proc%d | ", i+1);
        }
    }

    run();
}

int main() {

    int programas[3][NUMPROCESS] = {
        {0, 6, 9, 3, 3, 4, 0, 0 },
        {1, 7, 2, 4, 1, 2, 0, 0 },
        {2, 1, 1, 5, 1, 1, 0, 0}};

    createPrograms(programas);
}
