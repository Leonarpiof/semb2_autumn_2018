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
 * @file    random_print.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

const char msg1[] = "\rAnother useless message here!\n";
const char msg2[] = "\rThis is a useless message!\n";


typedef struct {
	SemaphoreHandle_t xSemaphore;
	char *msg;
}Args_t;


void print_task(void *arg)
{

	Args_t config = *((Args_t*) arg);

	for(;;)
	{
		xSemaphoreTake( config.xSemaphore, portMAX_DELAY );
		PRINTF(config.msg);
		xSemaphoreGive( config.xSemaphore );
		vTaskDelay(rand()%3);
	}
}

int main(void)
{
	static Args_t Args1;
	static Args_t Args2;
    SemaphoreHandle_t xSemaphore;

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    srand(0x15458523);

    xSemaphore = xSemaphoreCreateMutex();

    Args1.msg = (char*)msg1;
    Args1.xSemaphore = xSemaphore;
    Args2.msg = (char*)msg2;
    Args2.xSemaphore = xSemaphore;

    xTaskCreate(print_task, "print1", 110, (void*) &Args1, configMAX_PRIORITIES, NULL);
    xTaskCreate(print_task, "print2", 110, (void*) &Args2, configMAX_PRIORITIES-1, NULL);
    vTaskStartScheduler();

    for(;;)
    {

    }
    return 0 ;
}
