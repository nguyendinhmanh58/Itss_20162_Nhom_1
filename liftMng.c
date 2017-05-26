#include <stdio.h>
#include "lib.c"
void handle(int sigNo);
int initPipe();
int readPipe();
void writePipe(int sigNo);
void processRequest(int sigNo);

int pipe_c2p[PIPE_READ_WRITE];
int *pid_list;
int shm_pid_id, shm_pipe_id, shm_lift_operation_status_id;
bool canDeRequest = true;
int main () {
	int requestSignal;
	shm_pid_id = initShmPid(&pid_list);
	pid_list[LIFT_MNG] = getpid();
	if(initPipe() == -1) {
		printf("Init Pipe error\n");
		exit(1);
	}

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
        // check can read next request
        if(canDeRequest == true) {
            requestSignal = readPipe();
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
		case SIGNAL_ELEVATOR_CALL_123:
		case SIGNAL_ELEVATOR_CALL_124:
		case SIGNAL_ELEVATOR_CALL_125:
		case SIGNAL_ELEVATOR_CALL_221:
		case SIGNAL_ELEVATOR_CALL_321:
		case SIGNAL_ELEVATOR_CALL_421:
		case SIGNAL_ELEVATOR_CALL_521:
			writePipe(sigNo);
			break;
        case SIGNAL_ARRIVAL_1ND:
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_1ND));
            break;
        case SIGNAL_ARRIVAL_2ND:
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_2ND));
            break;
        case SIGNAL_ARRIVAL_3ND:
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_3ND));
            break;
        case SIGNAL_ARRIVAL_4ND:
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_4ND));
            break;
        case SIGNAL_ARRIVAL_5ND:
            sendNotifiLiftArrival(enSigNo(SIGNAL_ARRIVAL_5ND));
            break;

        case SIGNAL_FINISH_REQUEST:
            canDeRequest = true;
            break;
        default:
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

void sendNotifiLiftArrival(int sigNo)
{
    kill(pid_list[FLOOR_1ND], sigNo);
    kill(pid_list[FLOOR_2ND], sigNo);
    kill(pid_list[FLOOR_3ND], sigNo);
    kill(pid_list[FLOOR_4ND], sigNo);
    kill(pid_list[FLOOR_5ND], sigNo);
}
