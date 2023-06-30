#include "simulador.h"

Program programs[NUMPROGRAMS + 1];
Queue R; // READY queue
Queue B; // BLOCKED queue
Queue UBL; // UNBLOCK queue
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
        case NOTCREATED:               // process not created -> NEW
            if(programs[pid].start <= instant)
                programs[pid].state = NEW;

            break;
        case NEW:                      // NEW -> READY
            if(running == NONE && IsEmptyQueue(R)) {
                programs[pid].state = RUN;
                running = pid;

                if(programs[pid].time[programs[pid].exec + 1] > -1) {
                    Enqueue(programs[pid].time[programs[pid].exec + 1] - 1, UBL);
                }
                break;
            }

            programs[pid].state = READY;
            Enqueue(pid, R);

            break;
        case READY:                    // READY -> RUN
            if(Front(R) == pid && running == NONE) {
                Dequeue(R);
                programs[pid].state = RUN;
                running = pid;

                if(programs[pid].time[programs[pid].exec + 1] > -1) {
                    Enqueue(programs[pid].time[programs[pid].exec + 1] - 1, UBL);
                }
            }
            break;
        case RUN:                      // RUN -> BLOCKED
            if(running != pid || programs[pid].time[programs[pid].exec] > 0) break;

            if(programs[pid].exec >= NUMPROCESS - 1) {
                programs[pid].state = EXIT;
                running = NONE;
                break;
            }

            programs[pid].state = BLOCKED;
            programs[pid].exec += 2;

            running = NONE;

            Enqueue(pid, B);
            break;
        case BLOCKED:                  // BLOCKED -> READY
            if(programs[pid].time[programs[pid].exec] > 0) break;

            if(Front(B) == pid) {
                Enqueue(Dequeue(B), R);
                programs[pid].state = READY;
                programs[pid].exec++;
            }

            break;
        case UNBLOCK:
            if(programs[pid].unblockTime < 1) {
                programs[pid].unblockTime++;
                break;
            }

            programs[pid].state = READY;
            programs[pid].exec++;
            programs[pid].unblockTime = 0;

            if(programs[pid].time[programs[pid].exec + 1] > -1) {
                Enqueue(programs[pid].time[programs[pid].exec + 1] - 1, UBL);
            }

            break;
        case EXIT:                     // EXIT -> FINISHED
            programs[pid].state = FINISHED;
            break;
        default:
            break;
    }
}

void unblockPro() {
    if(IsEmptyQueue(UBL)) return;

    while(!IsEmptyQueue(UBL)) {
        int pid = Dequeue(UBL);
        if(programs[pid].state == BLOCKED){
            programs[pid].state = UNBLOCK;
            Remove(pid, B);
            Enqueue(pid, R);
        }
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
        case UNBLOCK:
            printf("  UBL  |");
            break;
        case EXIT:
            printf("  EXI  |");
            break;
        default:
            break;
    }
}

void run() {
    /* for(int i = 0; i < NUMPROGRAMS; i++) {
        changeState(i);
    }

    instant++; */

    while(notFinished()) {
        if(instant < 10)
            printf("%d   |", instant);
        else if(instant < 100)
            printf("%d  |", instant);
        else
            printf("%d |", instant);

        for(int i = 0; i < NUMPROGRAMS; i++) {
            if(programs[i].time[programs[i].exec] <= 0 && (programs[i].exec >= NUMPROCESS - 1 || programs[i].time[programs[i].exec+1] <= 0)) {
                if(programs[i].state != EXIT && programs[i].state != FINISHED) {
                    programs[i].state = EXIT;
                    Remove(i, B);
                    Remove(i, R);
                    if(running == i) running = NONE;
                } else {
                    programs[i].state = FINISHED;
                }
            }

            if(programs[i].state == RUN || (Front(B) == i && programs[i].state == BLOCKED)) {
                programs[i].time[programs[i].exec]--;
            }
            
            changeState(i);
        }

        
        for(int i = 0; i < NUMPROGRAMS; i++) showState(i);
        unblockPro();

        printf("\n");

        instant++;

        if(instant == 50) break;
    }

    if(instant < 10)
        printf("%d   |", instant);
    else if(instant < 100)
        printf("%d  |", instant);
    else
        printf("%d |", instant);

    for(int i = 0; i < NUMPROGRAMS; i++)
        showState(i);

    printf("\nDONE.\n");
}

void createPrograms(int p[NUMPROGRAMS][NUMPROCESS]) {
    R = CreateQueue(MAX);
    B = CreateQueue(MAX);
    UBL = CreateQueue(MAX);

    printf("i   |");
    for(int i = 0; i < NUMPROGRAMS; i++) {
        programs[i].start = p[i][0];
        programs[i].exec = 1;
        programs[i].state = NOTCREATED;
        programs[i].time = p[i];
        programs[i].unblockTime = 0;

        if(i < 9) {
            printf("  p0%d  |", i+1);
        } else {
            printf("  p%d  |", i+1);
        }
    }
    printf("\n");
}

void printPrograms(int p[NUMPROGRAMS][NUMPROCESS]) {
    for(int i = 0; i < NUMPROGRAMS; i++) {
        for(int j = 0; j < NUMPROCESS; j++) {
            printf("%d ", p[i][j]);
        }
        printf("\n");
    }
}

int main() {
    FILE* file = fopen("example2.txt", "r");
    int programas[NUMPROGRAMS][NUMPROCESS];
    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(0);
    }

    for(int i = 0; i < NUMPROGRAMS; i++) {
        for(int j = 0; j < NUMPROCESS; j++) {
            fscanf(file, "%d", &programas[i][j]);
        }
    }
        
    fclose(file);

    createPrograms(programas);
    run();
    
    // printPrograms(programas);
}


