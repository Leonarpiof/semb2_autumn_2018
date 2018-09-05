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
 * @file    freertos_example.c
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

void dummy_task1(void*arg)
{
	uint8_t counter = 0;
	for (;;)
	{
		PRINTF("IN TASK 1: %i +++++++++++++++\r\n", counter);
		counter++;
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
}

void dummy_task2(void*arg)
{
	uint8_t counter = 0;
	for (;;)
	{
		PRINTF("IN TASK 2: %i ***************\r\n", counter);
		counter++;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void dummy_task3(void*arg)
{
	uint8_t counter = 0;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		PRINTF("IN TASK 3: %i ---------------\r\n", counter);
		counter++;
		vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(4000));
	}
}

int main(void)
{

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    xTaskCreate(dummy_task1,"Task1", 20+configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(dummy_task2,"Task2", 20+configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(dummy_task3,"Task3", 20+configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

	for (;;)
	{
		__asm("NOP");
	}
}
