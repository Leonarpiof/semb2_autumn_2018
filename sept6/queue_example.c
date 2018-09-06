/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MK64FN1M0xxx12_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define GET_ARGS(args,type) *((type*)args)

typedef struct
{
	uint32_t inc_counter;
	uint32_t dec_counter;
}msg_t;

void task_producer(void*args)
{
	QueueHandle_t mailbox = GET_ARGS(args,QueueHandle_t);
	msg_t msg = {0,0xffff};
	for(;;)
	{
		msg.inc_counter++;
		msg.dec_counter--;
		xQueueSend(mailbox,&msg,portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void task_consumer(void*args)
{
	QueueHandle_t mailbox = GET_ARGS(args,QueueHandle_t);
	msg_t msg;
	for(;;)
	{
		xQueueReceive(mailbox,&msg,portMAX_DELAY);
		PRINTF("\rMessage received| inc: %i dec: %i\n",msg.inc_counter,msg.dec_counter);
	}
}

int main(void)
{
	static QueueHandle_t mailbox;
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();

	mailbox = xQueueCreate(10,sizeof(msg_t));

	xTaskCreate(task_consumer, "consumer", 110, (void*)&mailbox, 1, NULL);
	xTaskCreate(task_producer, "producer", 110, (void*)&mailbox, 1, NULL);
	vTaskStartScheduler();
	while(1) {}
	return 0 ;
}
