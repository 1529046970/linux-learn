#ifndef BINARY_SEMS_H
#define BINARY_SEMS_H 
#include <stdbool.h>
extern bool bsUseSemUndo;	//控制是否在semop()调用中使用SEM_UNDO标志,信号量撤销值，指定这个标记时，内核会记录信号量操作的效果，然后在进程终止时撤销这个操作
extern bool bsRetryOnEintr;	//控制实现是否在semop()调用被信号中断之后自动重启该调用

int initSemAvailable(int semId, int semmNum);
int initSemInUse(int semId, int semNum);
int reverseSem(int semId, int semNum);

int releaseSem(int semId, int semNum);

#endif 
