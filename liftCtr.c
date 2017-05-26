#include "lib.c"

int *ptr_pid, *ptr_lift_status;
int shm_pid_id, shm_pipe_id, shm_lift_operation_status_id;

int destinationFloor = -1;
int requestFromFloor = -1;
void handle(int sigNo);
int main ()
{
    int requestSigNo;
    shm_pid_id = initShmPid(&ptr_pid);
    shm_lift_operation_status_id = initShmLiftStatus(&ptr_lift_status);
    ptr_pid[LIFT_CTR] = getpid();
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_122), handle);
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_123), handle);
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_124), handle);
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_125), handle);
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_221), handle);
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_321), handle);
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_421), handle);
    signal(enSigNo(SIGNAL_ELEVATOR_CALL_521), handle);

    signal(enSigNo(SIGNAL_SENSOR_1ND_ON), handle);
    signal(enSigNo(SIGNAL_SENSOR_1ND_OFF), handle);
    signal(enSigNo(SIGNAL_SENSOR_2ND_ON), handle);
    signal(enSigNo(SIGNAL_SENSOR_2ND_OFF), handle);
    signal(enSigNo(SIGNAL_SENSOR_3ND_ON), handle);
    signal(enSigNo(SIGNAL_SENSOR_3ND_OFF), handle);
    signal(enSigNo(SIGNAL_SENSOR_4ND_ON), handle);
    signal(enSigNo(SIGNAL_SENSOR_4ND_OFF), handle);
    signal(enSigNo(SIGNAL_SENSOR_5ND_ON), handle);
    signal(enSigNo(SIGNAL_SENSOR_5ND_OFF), handle);

    while(1)
    {
        sleep(1);
    }
    return 0;
}

void handle(int sigNo)
{
    switch(deSigNo(sigNo))
    {
    case SIGNAL_ELEVATOR_CALL_122:
        printf("1 TO 2\n");
        destinationFloor = 2;
        requestFromFloor = 1;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;
    case SIGNAL_ELEVATOR_CALL_123:
        printf("1 TO 3\n");
        destinationFloor = 3;
        requestFromFloor = 1;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;
    case SIGNAL_ELEVATOR_CALL_124:
        printf("1 TO 4\n");
        destinationFloor = 4;
        requestFromFloor = 1;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;
    case SIGNAL_ELEVATOR_CALL_125:
        printf("1 TO 5\n");
        destinationFloor = 5;
        requestFromFloor = 1;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;
    case SIGNAL_ELEVATOR_CALL_221:
        printf("2 TO 1\n");
        destinationFloor = 2;
        requestFromFloor = 2;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;
    case SIGNAL_ELEVATOR_CALL_321:
        printf("3 TO 1\n");
        destinationFloor = 3;
        requestFromFloor = 3;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;
    case SIGNAL_ELEVATOR_CALL_421:
        printf("4 TO 1\n");
        destinationFloor = 4;
        requestFromFloor = 4;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;
    case SIGNAL_ELEVATOR_CALL_521:
        printf("5 TO 1\n");
        destinationFloor = 5;
        requestFromFloor = 5;
        kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_UP));
        break;



    case SIGNAL_SENSOR_1ND_ON:
        printf("__notifi__ 1nd sensor ON\n");
        kill(ptr_pid[LIFT_MNG], enSigNo(SIGNAL_ARRIVAL_1ND));
        if(destinationFloor == 1)
        {
            // check if lift come back
            kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_STOP));
            kill(ptr_pid[LIFT_MNG], enSigNo(SIGNAL_FINISH_REQUEST));
            printf("--> END!!!\n");
        }
        break;
    case SIGNAL_SENSOR_2ND_ON:
        printf("__notifi__ 2nd sensor ON\n");
        kill(ptr_pid[LIFT_MNG], enSigNo(SIGNAL_ARRIVAL_2ND));
        if(destinationFloor == 2)
        {
            kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_STOP));
            destinationFloor = 1;
            if(requestFromFloor == 1)
            {
                printf("-->put out luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
            else if(requestFromFloor == 2)
            {
                printf("-->put luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
        }
        break;
    case SIGNAL_SENSOR_3ND_ON:
        printf("__notifi__ 3nd sensor ON\n");
        kill(ptr_pid[LIFT_MNG], enSigNo(SIGNAL_ARRIVAL_3ND));
        if(destinationFloor == 3)
        {
            kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_STOP));
            destinationFloor = 1;
            if(requestFromFloor == 1)
            {
                printf("-->put out luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
            else if(requestFromFloor == 3)
            {
                printf("-->put luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
        }

        break;
    case SIGNAL_SENSOR_4ND_ON:
        printf("__notifi__ 4nd sensor ON\n");
        kill(ptr_pid[LIFT_MNG], enSigNo(SIGNAL_ARRIVAL_4ND));
        if(destinationFloor == 4)
        {
            destinationFloor = 1;
            kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_STOP));
            if(requestFromFloor == 1)
            {
                printf("-->put out luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
            else if(requestFromFloor == 4)
            {
                printf("-->put luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
        }

        break;
    case SIGNAL_SENSOR_5ND_ON:
        printf("__notifi__ 5nd sensor ON\n");
        kill(ptr_pid[LIFT_MNG], enSigNo(SIGNAL_ARRIVAL_5ND));
        if(destinationFloor == 5)
        {
            destinationFloor = 1;
            kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_STOP));
            if(requestFromFloor == 1)
            {
                printf("-->put out luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
            else if(requestFromFloor == 5)
            {
                printf("-->put luggage\n");
                sleep(3);
                kill(ptr_pid[LIFT_BODY], enSigNo(SIGNAL_LIFT_DOWN));
            }
        }
        break;
    case SIGNAL_SENSOR_1ND_OFF:
        printf("__notifi__ 1nd sensor OFF\n");
        break;
    case SIGNAL_SENSOR_2ND_OFF:
        printf("__notifi__ 2nd sensor OFF\n");
        break;
    case SIGNAL_SENSOR_3ND_OFF:
        printf("__notifi__ 3nd sensor OFF\n");
        break;
    case SIGNAL_SENSOR_4ND_OFF:
        printf("__notifi__ 4nd sensor OFF\n");
        break;
    case SIGNAL_SENSOR_5ND_OFF:
        printf("__notifi__ 5nd sensor OFF\n");
        break;
    }
}
