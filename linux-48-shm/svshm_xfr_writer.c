#include <stdio.h>
#include "semun.h"
#include "svshm_xfr.h"
#include <unistd.h>

int main(int argc, char*argv[]){
	int semid, shmid, bytes, xfrs;
	struct shmseg *shmp;
	union semun dummy;
	
	semid = semget(SEM_KEY,2,IPC_CREAT| OBJ_PERMS);
	if(semid==-1){
		printf("semget err \n");
		return 0;
	}
	if(initSemAvailable(semid, WRITE_SEM)==-1){
		printf("initSemAvailable err\n");
		return 0;
	
	}
	if(initSemInUse(semid, READ_SEM)==-1){
		printf("initSemInUse err\n");
		return 0;
	}
	
	shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT|OBJ_PERMS);
	if(shmid==-1){
		printf("shmget err\n");
		return 0;
	}
	
	shmp = shmat(shmid, NULL, 0);
	if(shmp==(void*)-1){
		printf("shmat err\n");
		return 0;
	}
	
	for(xfrs=0, bytes=0; ;xfrs++,bytes+=shmp->cnt){
		if(reverseSem(semid, WRITE_SEM)==-1){
			printf("reverseSem err\n");
			return 0;
		}
		
		shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
		if(shmp->cnt==-1){
			printf("read err\n");
			return 0;
		}
		if(releaseSem(semid, READ_SEM)==-1){
			printf("releaseSem err\n");
			return 0;
		}
		if(shmp->cnt ==0 )
			break;
	}
	
	if(reverseSem(semid, WRITE_SEM)==-1){
		printf("reverseSem err\n");
		return 0;
	}
	
	if(semctl(semid, 0, IPC_RMID, dummy)==-1){
		printf("semctl err\n");
		return 0;
	}
	
	if(shmdt(shmp)==-1){
		printf("shmdt err\n");
		return 0;
	}
	
	if(shmctl(shmid, IPC_RMID, 0)==-1){
		printf("shmctl err\n");
		return 0;
	}
	
	fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
	return 0;
}
