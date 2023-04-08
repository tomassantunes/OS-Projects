#include "simulador.h"
#include "queue.c"
#include "queue.h"

#define MAX 100
#define NUMPROGRAMS 3
#define NUMPROCESS 8

#define NONE -1
#define NOTCREATED -1
#define NEW 0
#define READY 1
#define RUN 2
#define BLOCKED 3
#define EXIT 4
#define FINISHED 5

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

Program programs[NUMPROGRAMS + 1];
Queue R; // READY queue
Queue B; // BLOCKED queue
int instant = 0;
int running = NONE;

int notFinished() {
    int n = 0;
    for(int i = 0; i < NUMPROGRAMS; i++) {
        if(programs[i].state < 4) {
            n++;
        }
    }

    if(n > 0) return 1;
    return 0;
}

void changeState(int pid) {
    switch(programs[pid].state) {
        case NOTCREATED:  // process not created -> NEW
            if(programs[pid].start <= instant)
                programs[pid].state = NEW;
            break;
        case NEW:      // NEW -> READY
            if(running == NONE && IsEmptyQueue(R)) {
                programs[pid].state = RUN;
                running = pid;
                break;
            }
            programs[pid].state = READY;
            Enqueue(pid, R);
            break;
        case READY:    // READY -> RUN
            if(running != NONE) break;
            if(Front(R) == pid) {
                Dequeue(R);
                programs[pid].state = RUN;
                running = pid;
            }
            break;
        case RUN:      // RUN -> BLOCKED
            programs[pid].state = BLOCKED;
            if(running == pid)
                running = NONE;
            Enqueue(pid, B);
            break;
        case BLOCKED:  // BLOCKED -> READY
            if(Front(B) == pid) {
                Enqueue(Dequeue(B), R);
                programs[pid].state = READY;
            }
            break;
        case EXIT:     // EXIT -> FINISHED
            programs[pid].state = FINISHED;
            break;
        default:
            break;
    }
}

void showState(int pid) {
    switch(programs[pid].state) {
        case NOTCREATED:
        case FINISHED:
            printf("       |");
            break;
        case NEW:
            printf("  NEW  |");
            break;
        case READY:
            printf("  RDY  |");
            break;
        case RUN:
            printf("  RUN  |");
            break;
        case BLOCKED:
            printf("  BLK  |");
            break;
        case EXIT:
            printf("  EXI  |");
            break;
        default:
            break;
    }
}

/*! TODO:
 *
 * @todo corrigir a forma como o programa corre os estados, olhar para o enunciado
 */

void run() {
    R = CreateQueue(MAX);
    B = CreateQueue(MAX);
    
    for(int i = 0; i < NUMPROGRAMS; i++) {
        changeState(i);
    }

    instant++;

    while(notFinished()) {
        if(instant < 10)
            printf("%d   |", instant);
        else
            printf("%d  |", instant);

        for(int i = 0; i < NUMPROGRAMS; i++) {
            if(programs[i].state == RUN || (Front(B) == i && programs[i].state == BLOCKED)) {
                programs[i].time[programs[i].exec]--;
            }

            showState(i);
        }
        printf("\n");

        for(int i = 0; i < NUMPROGRAMS; i++) {
            if(programs[i].time[programs[i].exec] == 0) {
                programs[i].exec++;
                if(programs[i].time[programs[i].exec] == 0) {
                    if(programs[i].state != EXIT && programs[i].state != FINISHED) {
                        programs[i].state = EXIT;
                        Remove(i, B);
                        Remove(i, R);
                    } else {
                        programs[i].state = FINISHED;
                    }
                } else {
                    changeState(i);
                }
            } else {
                changeState(i);
            }
        }

        running = NONE;
        instant++;

        if(instant == 100) break;
    }

    if(instant < 10)
        printf("%d   |", instant);
    else
        printf("%d  |", instant);

    for(int i = 0; i < NUMPROGRAMS; i++)
        showState(i);

    printf("\nDONE.\n");

}

void createPrograms(int p[NUMPROGRAMS][NUMPROCESS]) {
    printf("i   |");
    for(int i = 0; i < NUMPROGRAMS; i++) {
        programs[i].start = p[i][0];
        programs[i].exec = 0;
        programs[i].state = NOTCREATED;
        programs[i].time = p[i] + 1;

        if(i < 9) {
            printf("  p0%d  |", i+1);
        } else {
            printf("  p%d  |", i+1);
        }
    }
    printf("\n");

    run();
}

int main() {
    int programas[NUMPROGRAMS][NUMPROCESS] = {
        {0, 6, 9, 3, 3, 4, 0, 0 },
        {1, 7, 2, 4, 1, 2, 0, 0 },
        {2, 1, 1, 5, 1, 1, 0, 0}};
    
    createPrograms(programas);
}
