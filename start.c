#include "lib.c"
void main(int argc, char const *argv[])
{	
	pid_t pid;
	int* pid_list;
	int shmid = shmget(KEY_SHM_PID,10*sizeof(pid_t),IPC_CREAT);
	pid_list=(pid_t*)shmat(shmid,0,0);
	if(pid_list==(int*)-1){
		perror("shmat");
		exit(0);
	}	
	shmdt(pid_list);
    if((pid=fork()) !=0) {
		printf("FORK MNG\n");
		execl("./liftMng","./liftMng",NULL);
	}
	if((pid=fork())==0) {
		printf("FORK CTR\n");
		execl("./liftCtr","./liftCtr",NULL);
		return;
	}
	if((pid=fork())==0) {
		printf("FORK BODY\n");
		execl("./liftBody","./liftBody",NULL);
		return;
	}
	if((pid=fork())==0){
		printf("FORK 1ND\n");
		execl("./1nd_floor","./1nd_floor",NULL);
	}

	if((pid=fork())==0) {
			printf("FORK 2 FLOOR\n");
			execl("./x_floor","./x_floor","2",NULL);
			return;
		}
	
	if((pid=fork())==0) {
		printf("FORK 3 FLOOR\n");
		execl("./x_floor","./x_floor","3",NULL);		
		return;
	}
	if((pid=fork())==0) {
		printf("FORK 4 FLOOR\n");
		execl("./x_floor","./x_floor","4",NULL);
		return;
	}
	if((pid=fork())==0) {
		printf("FORK 5 FLOOR\n");
		execl("./x_floor","./x_floor","5",NULL);
		return;
	}
	if((pid=fork())==0) {
		printf("FORK SENSOR\n");
		execl("./liftSensor","./liftSensor",NULL);
		return;
	}
	return;
}