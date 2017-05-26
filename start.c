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
	 //Delete the shared memory
	if( shmctl(shmid,IPC_RMID,0) == -1 ){
	   perror("shmctl");
	}

    if((pid=fork()) !=0) {
		execl("./liftMng","./liftMng",NULL);
	}
	usleep(2000);
	if((pid=fork())==0) {
		execl("./liftCtr","./liftCtr",NULL);
		return;
	}
	usleep(2000);
	if((pid=fork())==0) {
		execl("./liftBody","./liftBody",NULL);
		return;
	}
	usleep(2000);
	if((pid=fork())==0) {
		execl("./liftSensor","./liftSensor",NULL);
		return;
	}
	usleep(2000);
	if((pid=fork())==0){
		execl("./1nd_floor","./1nd_floor",NULL);
	}

	if((pid=fork())==0) {
		execl("./x_floor","./x_floor","2",NULL);
		return;
	}
	
	if((pid=fork())==0) {
		execl("./x_floor","./x_floor","3",NULL);		
		return;
	}
	if((pid=fork())==0) {
		execl("./x_floor","./x_floor","4",NULL);
		return;
	}
	if((pid=fork())==0) {
		execl("./x_floor","./x_floor","5",NULL);
		return;
	}
	
	return;
}