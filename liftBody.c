#include "lib.c"
int *pid_list;
int shm_pid_id;
float* shmLiftHeight;
int shmLiftHeightId;
bool isStop = false;
enum LiftMoveStatus{
    UP,
    DOWN,
    STOP
};

enum LiftMoveStatus status = STOP;
int handle(int sigNo);

int main() {
    shm_pid_id = initShmPid(&pid_list);
    pid_list[LIFT_BODY] = getpid();
    // create share memory for save lift height
    shmLiftHeightId = initShmLiftHeight(&shmLiftHeight);
    // set default lift height is 1m
  	shmLiftHeight[0] = 1.5;

  	signal(enSigNo(SIGNAL_LIFT_UP), handle);
  	signal(enSigNo(SIGNAL_LIFT_DOWN), handle);
  	signal(enSigNo(SIGNAL_LIFT_STOP), handle);
    while(1){
    }
    return 0;
}

int handle(int sigNo) {
    switch(deSigNo(sigNo)) {
        case  SIGNAL_LIFT_UP:
            //isStop = false;
            if(status != UP) {
                status = UP;
                sendMoveStatusToFloors(status);
            } else status = UP;
            moveLiftUp();
            break;
        case  SIGNAL_LIFT_DOWN:
            //isStop = false;
            if(status != DOWN) {
                status = DOWN;
                sendMoveStatusToFloors(status);
            } else status = DOWN;
            moveLiftDown();
            break;
        case  SIGNAL_LIFT_STOP:
            //isStop = true;
            if(status != STOP) {
                status = STOP;
                sendMoveStatusToFloors(STOP);
            } else status = STOP;
            break;
    }
}

void moveLiftUp() {
    while (1) {
        if(status != STOP){
            sleep(1); // sleep 1 seconds
            shmLiftHeight[0]  += 0.5; // + lift height 0.5m
        } else break;
    }
    return;
}

void moveLiftDown() {
    while(1) {
        if(status != STOP){
            sleep(1); // sleep 1 seconds
            shmLiftHeight[0]  -= 0.5; // - lift height 0.5m
        } else break;
    }
    return;
}

void sendMoveStatusToFloors(enum LiftMoveStatus status) {
    int sigNo;
    if(status == UP) sigNo = SIGNAL_START_MOVE_UP;
    if(status == DOWN) sigNo = SIGNAL_START_MOVE_DOWN;
    if(status == STOP) sigNo = SIGNAL_STOP;
    kill(pid_list[FLOOR_1ND], enSigNo(sigNo));
    kill(pid_list[FLOOR_2ND], enSigNo(sigNo));
    kill(pid_list[FLOOR_3ND], enSigNo(sigNo));
    kill(pid_list[FLOOR_4ND], enSigNo(sigNo));
    kill(pid_list[FLOOR_5ND], enSigNo(sigNo));
}
