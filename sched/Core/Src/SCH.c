/*
 * SCH.c
 *
 *  Created on: Nov 14, 2022
 *      Author: ADMIN
 */
#include "SCH.h"
#define SCH_MAX_TASKS	40
#define NO_TASK_ID 		0
#define TICK			10

typedef struct {
	void (* pTask) (void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
	uint32_t oneshot;
} sTask ;
sTask SCH_tasks_G [SCH_MAX_TASKS];

void SCH_Init(void){
	for(int i = 0;i<SCH_MAX_TASKS;i++){
		SCH_tasks_G[i].pTask = 0x0000;
		SCH_tasks_G[i].Delay = 0;
		SCH_tasks_G[i].Period = 0;
		SCH_tasks_G[i].RunMe = 0;
		SCH_tasks_G[i].oneshot = 0;
	}
}
void SCH_Add_Task ( void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD,uint32_t ONESHOT){
//	int i = 0;
//	while((i < SCH_MAX_TASKS) && (SCH_tasks_G[i].pTask != 0)){
//		i++;
//	}
//	if(i == SCH_MAX_TASKS){
//		return;
//	}
//	SCH_tasks_G[i].pTask = pFunction;
//	SCH_tasks_G[i].Delay = DELAY/TICK;
//	SCH_tasks_G[i].Period =  PERIOD/TICK;
//	SCH_tasks_G[i].RunMe = 0;
//	SCH_tasks_G[i].oneshot = ONESHOT;
	int pos;
	for(pos = 0;pos<SCH_MAX_TASKS;pos++){
		if(SCH_tasks_G[pos].pTask == 0x0000) break;
	}
	if(pos == SCH_MAX_TASKS) return;
	if(pos == 0){
		SCH_tasks_G[0].pTask = pFunction;
		SCH_tasks_G[0].Delay = DELAY/TICK;
		SCH_tasks_G[0].Period = PERIOD/TICK;
		SCH_tasks_G[0].RunMe = 0;
		SCH_tasks_G[0].oneshot = ONESHOT;
	}
	else{
		int delay_ms = DELAY/TICK;
		int pos2;
		for(int i = 0;i<=pos;i++){
			if(SCH_tasks_G[i].Delay <= delay_ms && SCH_tasks_G[i].pTask != 0){
				delay_ms-=SCH_tasks_G[i].Delay;
			}
			else{
				pos2 = i;
				if(SCH_tasks_G[i].pTask){
					SCH_tasks_G[i].Delay-=delay_ms;
				}
				break;
			}
		}
		for(int i = pos;i>pos2;i--){
			SCH_tasks_G[i].pTask = SCH_tasks_G[i-1].pTask;
			SCH_tasks_G[i].Delay = SCH_tasks_G[i-1].Delay;
			SCH_tasks_G[i].Period = SCH_tasks_G[i-1].Period;
			SCH_tasks_G[i].RunMe = SCH_tasks_G[i-1].RunMe;
			SCH_tasks_G[i].oneshot = SCH_tasks_G[i-1].oneshot;
		}
		SCH_tasks_G[pos2].pTask = pFunction;
		SCH_tasks_G[pos2].Delay = delay_ms;
		SCH_tasks_G[pos2].Period = PERIOD/TICK;
		SCH_tasks_G[pos2].RunMe = 0;
		SCH_tasks_G[pos2].oneshot = ONESHOT;
	}

}

void SCH_Delete_Task(uint32_t taskID){
//	if(SCH_tasks_G[taskID].pTask == 0) return;
//	SCH_tasks_G[taskID].pTask = 0x0000;
//	SCH_tasks_G[taskID].Delay = 0;
//	SCH_tasks_G[taskID].Period =  0;
//	SCH_tasks_G[taskID].RunMe = 0;
//	SCH_tasks_G[taskID].oneshot = 0;
	for(int i = taskID+1;i<SCH_MAX_TASKS;i++){
		SCH_tasks_G[i-1].pTask = SCH_tasks_G[i].pTask;
		SCH_tasks_G[i-1].Delay = SCH_tasks_G[i].Delay;
		SCH_tasks_G[i-1].Period = SCH_tasks_G[i].Period;
		SCH_tasks_G[i-1].RunMe = SCH_tasks_G[i].RunMe;
		SCH_tasks_G[i-1].oneshot = SCH_tasks_G[i].oneshot;
	}
	SCH_tasks_G[SCH_MAX_TASKS-1].pTask = 0x0000;
	SCH_tasks_G[SCH_MAX_TASKS-1].Delay = 0;
	SCH_tasks_G[SCH_MAX_TASKS-1].Period = 0;
	SCH_tasks_G[SCH_MAX_TASKS-1].RunMe = 0;
	SCH_tasks_G[SCH_MAX_TASKS-1].oneshot = 0;
}

void SCH_Update(void){
	if(SCH_tasks_G[0].pTask){
		if(SCH_tasks_G[0].Delay <=0){
			SCH_tasks_G[0].RunMe++;
		}
		else{
			SCH_tasks_G[0].Delay--;
		}
	}
}

void SCH_Dispatch_Tasks(void){
	sTask tmp;
	for(int i = 0; i < SCH_MAX_TASKS; i++){
		if(SCH_tasks_G[i].RunMe > 0){
			SCH_tasks_G[i].RunMe--;
			(*SCH_tasks_G[i].pTask)();
			tmp.Period = SCH_tasks_G[i].Period;
			tmp.pTask = SCH_tasks_G[i].pTask;
			tmp.oneshot = SCH_tasks_G[i].oneshot;
			SCH_Delete_Task(i);
			if(tmp.oneshot == 0){
				SCH_Add_Task(tmp.pTask, tmp.Period*TICK, tmp.Period*TICK, 0);
			}
		}
	}
}











