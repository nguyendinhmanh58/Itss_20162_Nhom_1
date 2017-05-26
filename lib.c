#include "global.h"

int enSigNo(int SigNo) {
    return SigNo + SIGRTMIN;
}

int deSigNo(int SigNo) {
    return SigNo - SIGRTMIN;
}

int initShmPid(int** ptr_pid) {
    int shmid;
    if ((shmid = shmget(KEY_SHM_PID,SHM_PID_SIZE*sizeof(int),IPC_CREAT|0660)) == -1){
	    perror("shmget");
	    exit(1);
  	}
  	*ptr_pid = (int*)shmat(shmid,0,0);
  	if ((*ptr_pid == (int*)-1))
	{
	   perror("shmat");
	   exit(1);
	}
    return shmid;
}

int initShmLiftHeight(float** shmLiftHeight) {
    int shmLiftHeightId;
    if ((shmLiftHeightId = shmget(KEY_SHM_LIFT_HEIGHT,SHM_LIFT_HEIGHT_SIZE*sizeof(float),IPC_CREAT|0660)) == -1){
	    perror("shmget");
	    exit(1);
  	}
  	*shmLiftHeight = (float *)shmat(shmLiftHeightId, 0, 0);
    if ((*shmLiftHeight == (float*)-1))
	{
	   perror("shmat");
	   exit(1);
	}
	return shmLiftHeightId;
}
