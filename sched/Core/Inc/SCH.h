/*
 * SCH.h
 *
 *  Created on: Nov 14, 2022
 *      Author: ADMIN
 */

#ifndef INC_SCH_H_
#define INC_SCH_H_
#include "main.h"

void SCH_Init(void);

void SCH_Add_Task ( void (*pFunction)() ,
					uint32_t DELAY,
					uint32_t PERIOD, uint32_t ONESHOT);

void SCH_Update(void);

void SCH_Dispatch_Tasks(void);

void SCH_Delete_Tasks(uint32_t taskID);

#endif /* INC_SCH_H_ */
