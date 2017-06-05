volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include <stdio.h>

void print_int() {
	//TickType_t lastWakeTime;
	//lastWakeTime = xTaskGetTickCount();
	printf("Hello World!\n");
	for(;;) {
//		exit(0);
		printf("Hello World!\n");
		//vTaskDelayUntil( &lastWakeTime, 1000 / portTICK_RATE_MS );
	}
}

void print_char() {
	
	vTaskDelay( 1000 / portTICK_RATE_MS );
}


void main() {
	const size_t xRegTestStackSize = 300;
	
	xTaskCreate( 	print_int,			/* Function that implements the task. */
					"print_integer", 				/* Text name of the task. */
					xRegTestStackSize,		/* Stack allocated to the task. */
					NULL, 					/* The task parameter is not used. */
					3, 		/* The priority to assign to the task. */
					NULL );					/* Don't receive a handle back, it is not needed. */
	
	xTaskCreate( 	print_int	,			/* Function that implements the task. */
					"print_char", 				/* Text name of the task. */
					xRegTestStackSize,		/* Stack allocated to the task. */
					NULL, 					/* The task parameter is not used. */
					2, 		/* The priority to assign to the task. */
					NULL );					/* Don't receive a handle back, it is not needed. */
	printf("Hello World!\n");
	vTaskStartScheduler();

	for(;;) {
		//add();
		//print_uart0("Hello world!\n");
	}
}
