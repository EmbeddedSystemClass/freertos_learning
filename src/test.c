volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

void print_uart0(const char *s) {
	while(*s != '\0') { /* Loop until end of string */
		*UART0DR = *s; /* Transmit char */
		s++; /* Next char */
	}
}

void print_int() {
	TickType_t lastWakeTime;
	lastWakeTime = xTaskGetTickCount();
	for(;;) {
		print_uart0("\r15");
		vTaskDelayUntil( &lastWakeTime, 1000 / portTICK_RATE_MS );
	}
}

void print_char() {
	print_uart0("\rHello world");
	vTaskDelay( 1000 / portTICK_RATE_MS );
}


void c_entry() {
	const size_t xRegTestStackSize = 25U;

	xTaskCreate( 	print_int,			/* Function that implements the task. */
					"print_integer", 				/* Text name of the task. */
					xRegTestStackSize,		/* Stack allocated to the task. */
					NULL, 					/* The task parameter is not used. */
					3, 		/* The priority to assign to the task. */
					NULL );					/* Don't receive a handle back, it is not needed. */

	xTaskCreate( 	print_char,			/* Function that implements the task. */
					"print_char", 				/* Text name of the task. */
					xRegTestStackSize,		/* Stack allocated to the task. */
					NULL, 					/* The task parameter is not used. */
					2, 		/* The priority to assign to the task. */
					NULL );					/* Don't receive a handle back, it is not needed. */

	vTaskStartScheduler();

	for(;;) {
		//add();
		//print_uart0("Hello world!\n");
	}
}
