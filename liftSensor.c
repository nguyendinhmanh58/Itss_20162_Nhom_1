#include "lib.c"
int *pid_list;
float* shmLiftHeight;
int shm_pid_id, shmLiftHeightId;
int sensor_1nd = ON, sensor_2nd = OFF, sensor_3nd = OFF, sensor_4nd = OFF, sensor_5nd = OFF;
int main() {
    shm_pid_id = initShmPid(&pid_list);
    shmLiftHeightId = initShmLiftHeight(&shmLiftHeight);
    while(pid_list[LIFT_CTR] != 0) {
       if(shmLiftHeight[0] >= 1.5 && shmLiftHeight[0] <= 2.5 && sensor_1nd == OFF) {
            sensor_1nd = ON;
            kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_1ND_ON));
       } else if(shmLiftHeight[0] > 2.5 && shmLiftHeight[0] < 4.5){
            if(sensor_1nd == ON) {
                sensor_1nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_1ND_OFF));
            }
            if(sensor_2nd == ON) {
                sensor_2nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_2ND_OFF));
            }
       } else if(shmLiftHeight[0] >= 4.5 && shmLiftHeight[0] <= 5.5 && sensor_2nd == OFF) {
            sensor_2nd = ON;
            kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_2ND_ON));
       } else if(shmLiftHeight[0] > 5.5 && shmLiftHeight[0] < 7.5){
            if(sensor_2nd == ON) {
                sensor_2nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_2ND_OFF));
            }
            if(sensor_3nd == ON) {
                sensor_3nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_3ND_OFF));
            }

       } else if(shmLiftHeight[0] >= 7.5 && shmLiftHeight[0] <= 8.5 && sensor_3nd == OFF) {
            sensor_3nd = ON;
            kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_3ND_ON));
       } else if(shmLiftHeight[0] > 8.5 && shmLiftHeight[0] < 10.5) {
            if(sensor_3nd == ON) {
                sensor_3nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_3ND_OFF));
            }
            if(sensor_4nd == ON) {
                sensor_4nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_4ND_OFF));
            }
       } else if(shmLiftHeight[0] >= 10.5 && shmLiftHeight[0] <= 11.5 && sensor_4nd == OFF) {
            sensor_4nd = ON;
            kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_4ND_ON));
       } else if(shmLiftHeight[0] > 11.5 && shmLiftHeight[0] < 13.5) {
            if(sensor_4nd == ON) {
                sensor_4nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_4ND_OFF));
            }
            if(sensor_5nd == ON) {
                sensor_5nd = OFF;
                kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_5ND_OFF));
            }
       } else if(shmLiftHeight[0] >= 13.5 && shmLiftHeight[0] <=14.5 && sensor_5nd == OFF) {
            sensor_5nd = ON;
            kill(pid_list[LIFT_CTR], enSigNo(SIGNAL_SENSOR_5ND_ON));
       }
    }
}
