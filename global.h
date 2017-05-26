#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define PIPE_READ  0
#define PIPE_WRITE 1
#define PIPE_READ_WRITE  2
#define BUFFSIZE 80

#define KEY_SHM_2_5 1111
#define KEY_SHM_PID 1112
#define SHM_PID_SIZE 10

#define KEY_SHM_LIFT_HEIGHT 1113
#define SHM_LIFT_HEIGHT_SIZE 1


#define SIGNAL_FINISH_REQUEST 0
#define SIGNAL_SENSOR_1ND_ON 1
#define SIGNAL_SENSOR_2ND_ON 2
#define SIGNAL_SENSOR_3ND_ON 3
#define SIGNAL_SENSOR_4ND_ON 4
#define SIGNAL_SENSOR_5ND_ON 5


#define SIGNAL_SENSOR_1ND_OFF 6
#define SIGNAL_SENSOR_2ND_OFF 7
#define SIGNAL_SENSOR_3ND_OFF 8
#define SIGNAL_SENSOR_4ND_OFF 9
#define SIGNAL_SENSOR_5ND_OFF 10

#define SIGNAL_ELEVATOR_CALL_122 11
#define SIGNAL_ELEVATOR_CALL_123 12
#define SIGNAL_ELEVATOR_CALL_124 13
#define SIGNAL_ELEVATOR_CALL_125 14
#define SIGNAL_ELEVATOR_CALL_521 15
#define SIGNAL_ELEVATOR_CALL_421 16
#define SIGNAL_ELEVATOR_CALL_321 17
#define SIGNAL_ELEVATOR_CALL_221 18



#define SIGNAL_LIFT_UP 19
#define SIGNAL_LIFT_DOWN 20
#define SIGNAL_LIFT_STOP 21

#define SIGNAL_START_MOVE_UP 22
#define SIGNAL_START_MOVE_DOWN 23
#define SIGNAL_STOP 24

#define SIGNAL_ARRIVAL_1ND 25
#define SIGNAL_ARRIVAL_2ND 26
#define SIGNAL_ARRIVAL_3ND 27
#define SIGNAL_ARRIVAL_4ND 28
#define SIGNAL_ARRIVAL_5ND 29
#define SIGNAL_MOVEOUT 30


#define ON 1
#define OFF 0
enum ProcessIndex{
        FLOOR_1ND,
        FLOOR_2ND,
        FLOOR_3ND,
        FLOOR_4ND,
        FLOOR_5ND,
        LIFT_MNG,
        LIFT_CTR,
        LIFT_BODY,
        LIFT_SENSOR
};

enum LiftStatus {
    MOVEMENT,
    ARRIVAL_1ND,
    ARRIVAL_2ND,
    ARRIVAL_3ND,
    ARRIVAL_4ND,
    ARRIVAL_5ND
};

int enSigNo(int SigNo);
int deSigNo(int SigNo);
int initShmPid(int** ptr_pid);
int initShmLiftStatus(int** ptr_lift_status);
