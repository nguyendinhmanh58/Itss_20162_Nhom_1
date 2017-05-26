#include <stdio.h>
#include "lib.c"
void handle(int sigNo);

int pipe_c2p[PIPE_READ_WRITE];
int initPipe();
int readPipe();
void writePipe(int sigNo);
void processRequest(int sigNo);

int *pid_list, *ptr_lift_status;
int shm_pid_id, shm_pipe_id, shm_lift_operation_status_id;
bool canDeRequest = true;
int main () {
	int requestSignal;
    // init
	shm_pid_id = initShmPid(&pid_list);
	shm_lift_operation_status_id = initShmLiftStatus(&ptr_lift_status);
	//
	pid_list[LIFT_MNG] = getpid();
    // default lift is opePanel 1
	ptr_lift_status[0] = ARRIVAL_1ND;
	if(initPipe() == -1) {
		printf("Init Pipe error\n");
		exit(1);
	}

	printf("pid: %d\n", pid_list[0]);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_122), handle);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_123), handle);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_124), handle);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_125), handle);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_221), handle);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_321), handle);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_421), handle);
	signal(enSigNo(SIGNAL_ELEVATOR_CALL_521), handle);

    signal(enSigNo(SIGNAL_ARRIVAL_1ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_2ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_3ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_4ND), handle);
    signal(enSigNo(SIGNAL_ARRIVAL_5ND), handle);

    signal(enSigNo(SIGNAL_FINISH_REQUEST), handle);


	while(1){
        // check if lift status is opePanel 1 --> read next request
        if(getLiftStatus() == ARRIVAL_1ND && canDeRequest == true) {
            requestSignal = readPipe();
            printf("-> read request: %d\n",requestSignal);
            if(requestSignal != -1) {
                processRequest(requestSignal);
                canDeRequest = false;
            }
        }
  	}
}

void handle(int sigNo) {
	switch(deSigNo(sigNo)) {
		case SIGNAL_ELEVATOR_CALL_122:
			printf("1 floor request to 2 floor\n");
			writePipe(sigNo);
			break;
		case SIGNAL_ELEVATOR_CALL_123:
			printf("1 floor request to 3 floor\n");
			writePipe(sigNo);
			break;
		case SIGNAL_ELEVATOR_CALL_124:
			printf("1 floor request to 4 floor\n");
			writePipe(sigNo);
			break;
		case SIGNAL_ELEVATOR_CALL_125:
			printf("1 floor request to 5 floor\n");
			writePipe(sigNo);
			break;
        case SIGNAL_ELEVATOR_CALL_221:
			printf("2 floor request to 1 floor\n");
			writePipe(sigNo);
			break;
        case SIGNAL_ELEVATOR_CALL_321:
			printf("3 floor request to 1 floor\n");
			writePipe(sigNo);
			break;
		case SIGNAL_ELEVATOR_CALL_421:
			printf("4 floor request to 1 floor\n");
			writePipe(sigNo);
			break;
        case SIGNAL_ELEVATOR_CALL_521:
			printf("5 floor request to 1 floor\n");
			writePipe(sigNo);
			break;


        case SIGNAL_ARRIVAL_1ND:
            ptr_lift_status[0] = ARRIVAL_1ND;
            printf("arrival 1nd\n");
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_1ND));
            break;
        case SIGNAL_ARRIVAL_2ND:
            ptr_lift_status[0] = ARRIVAL_2ND;
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_2ND));
            break;
        case SIGNAL_ARRIVAL_3ND:
            ptr_lift_status[0] = ARRIVAL_3ND;
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_3ND));
            break;
        case SIGNAL_ARRIVAL_4ND:
            ptr_lift_status[0] = ARRIVAL_4ND;
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_4ND));
            break;
        case SIGNAL_ARRIVAL_5ND:
            ptr_lift_status[0] = ARRIVAL_5ND;
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_5ND));
            break;
        case SIGNAL_FINISH_REQUEST:
            canDeRequest = true;
            break;
        default:
            printf("Not found!");
            break;
	}
}

int initPipe() {
	memset(pipe_c2p, 0, sizeof(pipe_c2p));
	int result;
	result = pipe(pipe_c2p);
	return result;
}

int readPipe() {
	int sigNo;
  	int  readSize = 0;
	readSize = read(pipe_c2p[PIPE_READ], &sigNo, sizeof(int));
	if(readSize == 0) return -1;
	return sigNo;
}

void writePipe(int sigNo) {
	write(pipe_c2p[PIPE_WRITE], &sigNo, sizeof(int));
}

void processRequest(int sigNo){
    kill(pid_list[LIFT_CTR], sigNo);
}


int getLiftStatus () {
    // attach share memory lift operation status
	return ptr_lift_status[0];
}

void sendNotifiLiftArrival(int sigNo)
{
    kill(pid_list[FLOOR_1ND], sigNo);
    kill(pid_list[FLOOR_2ND], sigNo);
    kill(pid_list[FLOOR_3ND], sigNo);
    kill(pid_list[FLOOR_4ND], sigNo);
    kill(pid_list[FLOOR_5ND], sigNo);
}
