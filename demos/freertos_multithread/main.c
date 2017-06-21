#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <string.h>
#include "tiny-json.h"
static void setup_hardware( void );

volatile xQueueHandle serial_str_queue = NULL;
volatile xSemaphoreHandle serial_tx_wait_sem = NULL;
volatile xQueueHandle serial_rx_queue = NULL;

/* Queue structure used for passing messages. */
typedef struct
{
    char str[300];
} serial_str_msg;

/* Queue structure used for passing characters. */
typedef struct
{
    char ch;
} serial_ch_msg;

/* IRQ handler to handle USART2 interrupts (both transmit and receive
 * interrupts). */

#if 1
void USART2_IRQHandler(void)
{
    static signed portBASE_TYPE xHigherPriorityTaskWoken;
    serial_ch_msg rx_msg;
    serial_str_msg msg;
    /* If this interrupt is for a transmit... */
    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
        /* "give" the serial_tx_wait_sem semaphore to notify processes that
         * the buffer has a spot free for the next byte.
         */
        xSemaphoreGiveFromISR(serial_tx_wait_sem, &xHigherPriorityTaskWoken);
       // while(!xQueueSendToBack(serial_str_queue, "inside isr for tx", portMAX_DELAY));

        /* Disables the transmit interrupt. */
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    /* If this interrupt is for a receive... */
    } else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
       // while(!xQueueSendToBack(serial_str_queue, "inside isr for rx", portMAX_DELAY));
        /* Receive the byte from the buffer. */
        while(1);
        strcpy(msg.str, "inside read isr:");
        while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));

        rx_msg.ch = USART_ReceiveData(USART2);
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        /* Queue the received byte. */
        if(!xQueueSendToBackFromISR(serial_rx_queue, &rx_msg, &xHigherPriorityTaskWoken)) {
            /* If there was an error queueing the received byte, freeze. */
            while(1);
        }
    } else {
        /* Only transmit and receive interrupts should be enabled.  If this is
         * another type of interrupt, freeze.
         */
        while(1);
    }

    if(xHigherPriorityTaskWoken) {
        taskYIELD();
    }
}
#endif
void send_byte_rtos(char ch)
{
    /* Wait until the RS232 port can receive another byte (this semaphore is
     * "given" by the RS232 port interrupt when the buffer has room for another
     * byte.
     */
    while(!xSemaphoreTake(serial_tx_wait_sem, portMAX_DELAY));

    /* Send the byte and enable the transmit interrupt (it is disabled by the
     * interrupt).
     */
    USART_SendData(USART2, ch);
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}

char receive_byte_rtos(void)
{
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    serial_ch_msg msg;

    /* Wait for a byte to be queued by the receive interrupt handler. */
    while(!xQueueReceive(serial_rx_queue, &msg, portMAX_DELAY));
    //msg.ch = USART_ReceiveData(USART2);
    msg.ch = "\n";
    return msg.ch;
}


void rs232_xmit_msg_task( void *pvParameters )
{
    serial_str_msg msg;
    int curr_char;

    while(1) {
        /* Read from the queue.  Keep trying until a message is received.  This
         * will block for a period of time (specified by portMAX_DELAY). */
        while(!xQueueReceive(serial_str_queue, &msg, portMAX_DELAY));

        /* Write each character of the message to the RS232 port. */
        curr_char = 0;
        while(msg.str[curr_char] != '\0') {
            send_byte_rtos(msg.str[curr_char]);
            curr_char++;
        }
    }
}


/* Repeatedly queues a string to be sent to the RS232.
 *   delay - the time to wait between sending messages.  A delay of 1 means
 *           wait 1/100th of a second.
 */
void queue_str_task(const char *str, int delay)
{
    serial_str_msg msg;

    /* Prepare the message to be queued. */
    strcpy(msg.str, str);

    while(1) {
        /* Post the message.  Keep on trying until it is successful. */
        while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));

        /* Wait. */
        vTaskDelay(delay);
    }
}

void queue_str_task1( void *pvParameters )
{	 
    char str[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    //puts( str );
    json_t mem[32];
    json_t const* json = json_create( str, mem, sizeof mem / sizeof *mem );
    char const* firstNameVal = json_getPropertyValue( json, "firstName" );
    char const* lastName = json_getPropertyValue( json, "lastName" );
    //char const* city = json_getPropertyValue( json, "city" );
    //char const* state = json_getPropertyValue( json, "state" );
    int age= json_getPropertyValue(json,"time");
    //json_t const* age = json_getProperty( json, "age" );
    //int const ageVal = (int)json_getInteger( age );
    //int a =5,b=0;
    queue_str_task(age, 200);
}

void queue_str_task2( void *pvParameters )
{
    queue_str_task("Hello 2\n", 50);
}

void serial_readwrite_task( void *pvParameters )
{
    serial_str_msg msg;
    char ch;
    int curr_char;
    int done;

    /* Prepare the response message to be queued. */
    //strcpy(msg.str, "inside read task:");
    // while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));
    while(1) {


        curr_char = 4;
        done = 0;
        strcpy(msg.str, "inside read task:");
        while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));
       // while(!xQueueSendToBack(serial_str_queue, "before while", portMAX_DELAY));
        do {
            /* Receive a byte from the RS232 port (this call will block). */
            strcpy(msg.str, "\nInside while\n");
            while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));
            ch = receive_byte_rtos();
            /* If the byte is an end-of-line type character, then finish the
             * string and indicate we are done.
             */
            strcpy(msg.str, "\nAfter read byte\n");
            while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));
            if((ch == '\r') || (ch == '\n')) {
                msg.str[curr_char] = '\n';
                msg.str[curr_char+1] = '\0';
                done = 1;
                //while(!xQueueSendToBack(serial_str_queue, &done, portMAX_DELAY));
            /* Otherwise, add the character to the response string. */
            } else {
                msg.str[curr_char++] = ch;
            }
        } while(!done);
        strcpy(msg.str, "Outside while while\n");
        while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));
        /* Once we are done building the response string, queue the response to
         * be sent to the RS232 port.
         */
        vTaskDelay(500);
        while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));
    }
}

int main(void)
{

    //printf( "Last Name: %s.\n", lastName );

    init_rs232();
    enable_rs232_interrupts();
    enable_rs232();

    /* Create the queue to hold messages to be written to the RS232. */
    serial_str_queue = xQueueCreate( 10, sizeof( serial_str_msg ) );
    vSemaphoreCreateBinary(serial_tx_wait_sem);
    serial_rx_queue = xQueueCreate( 10, sizeof( serial_ch_msg ) );
#if 1

    /* Create tasks to queue a string to be written to the RS232 port. */
    xTaskCreate( queue_str_task1, ( signed portCHAR * ) "Serial Write 1", 1024 /* stack size */, NULL, tskIDLE_PRIORITY + 10, NULL );
    xTaskCreate( queue_str_task2, ( signed portCHAR * ) "Serial Write 2", 512 /* stack size */, NULL, tskIDLE_PRIORITY + 10, NULL );

    /* Create a task to write messages from the queue to the RS232 port. */
    xTaskCreate(rs232_xmit_msg_task, ( signed portCHAR * ) "Serial Xmit Str", 512 /* stack size */, NULL, tskIDLE_PRIORITY + 2, NULL );

    /* Create a task to receive characters from the RS232 port and echo them back to the RS232 port. */
    xTaskCreate(serial_readwrite_task, ( signed portCHAR * ) "Serial Read/Write", 512 /* stack size */, NULL, tskIDLE_PRIORITY + 20, NULL );

    /* Start running the tasks. */
    vTaskStartScheduler();
#endif
#if 0
    char str[] = "{\n"
// "\t\"firstName\": \"Bidhan\",\n"
// "\t\"lastName\": \"Chatterjee\",\n"
    "\t\"age\": 40,\n"
    "}\n";
    int curr_char = 0;
        while(str[curr_char] != '\0') {
            send_byte_rtos(str[curr_char]);
            curr_char++;
        }
#endif
#if 0
    init_led();
    USART_Cmd(USART2, ENABLE);

    while(1) {
        /* Loop until the USART2 has received a byte. */
       // while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
        uint8_t b;
        /* Capture the received byte and print it out. */
        b = (USART_ReceiveData(USART2) & 0x7F);
        send_byte('G');
        send_byte('o');
        send_byte('t');
        send_byte(':');
        send_byte(b);
        send_byte('\n');
}
#endif 

    return 0;
}

void vApplicationTickHook( void )
{
}
