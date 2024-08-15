#include "svshm_xfr.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char*argv[]){
	int semid, shmid, xfrs, bytes;
	struct shmseg *shmp;
	
	semid = semget(SEM_KEY, 00, 0);
	if(semid==-1){
		printf("semget err\n");
		return 0;
	}
	
	shmid = shmget(SHM_KEY,0,0);
	if(shmid==-1){
		printf("shmget err\n");
		return 0;
	}
	
	shmp = shmat(shmid, NULL, SHM_RDONLY);
	
	if(shmp==(void*)-1){
		printf("shmat err\n");
		return 0;
	}
	
	for(xfrs=0,bytes=0;;xfrs++){
		if(reverseSem(semid, READ_SEM)==-1){
			printf("reverseSem err\n");
			return 0;
		}
		if(shmp->cnt==0)	break;
		bytes += shmp->cnt;
		
		if(write(STDOUT_FILENO, shmp->buf, shmp->cnt)!=shmp->cnt){
			printf("write err\n");
			return 0;
		}
		if(releaseSem(semid, WRITE_SEM)==-1){
			printf("releaseSem err\n");
			return 0;
		}
	}
	
	if(shmdt(shmp)==-1){
		printf("shmdt err\n");
		return 0;
	}
	
	if(releaseSem(semid, WRITE_SEM)==-1){
		printf("releaseSem err\n");
		return 0;
	}
	fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
	return 0;
}
