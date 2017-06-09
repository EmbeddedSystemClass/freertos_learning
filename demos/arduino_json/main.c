#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <string.h>

/* JSON includes */
#include "ArduinoJson.h"

static void setup_hardware( void );

volatile xQueueHandle serial_str_queue = NULL;
volatile xSemaphoreHandle serial_tx_wait_sem = NULL;
volatile xQueueHandle serial_rx_queue = NULL;

/* Queue structure used for passing messages. */
typedef struct
{
    char str[100];
} serial_str_msg;

/* Queue structure used for passing characters. */
typedef struct
{
    char ch;
} serial_ch_msg;

/* IRQ handler to handle USART2 interrupts (both transmit and receive
 * interrupts). */
void USART2_IRQHandler(void)
{
    static signed portBASE_TYPE xHigherPriorityTaskWoken;
    serial_ch_msg rx_msg;

    /* If this interrupt is for a transmit... */
    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
        /* "give" the serial_tx_wait_sem semaphore to notify processes that
         * the buffer has a spot free for the next byte.
         */
        xSemaphoreGiveFromISR(serial_tx_wait_sem, &xHigherPriorityTaskWoken);

        /* Disables the transmit interrupt. */
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    /* If this interrupt is for a receive... */
    } else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        /* Receive the byte from the buffer. */
        rx_msg.ch = USART_ReceiveData(USART2);

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
    serial_ch_msg msg;

    /* Wait for a byte to be queued by the receive interrupt handler. */
    while(!xQueueReceive(serial_rx_queue, &msg, portMAX_DELAY));

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
    queue_str_task("Hello 1\n", 200);
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
    strcpy(msg.str, "Got:");

    while(1) {
        curr_char = 4;
        done = 0;
        do {
            /* Receive a byte from the RS232 port (this call will block). */
            ch = receive_byte_rtos();

            /* If the byte is an end-of-line type character, then finish the
             * string and indicate we are done.
             */
            if((ch == '\r') || (ch == '\n')) {
                msg.str[curr_char] = '\n';
                msg.str[curr_char+1] = '\0';
                done = -1;
            /* Otherwise, add the character to the response string. */
            } else {
                msg.str[curr_char++] = ch;
            }
        } while(!done);

        /* Once we are done building the response string, queue the response to
         * be sent to the RS232 port.
         */
        while(!xQueueSendToBack(serial_str_queue, &msg, portMAX_DELAY));
    }
}

void encoding_JSON()
{
    int rpm = 1000;
    //  JSON buffer
    StaticJsonBuffer<500> json_buffer;

    //  JSON object
    JsonObject & sensor_object = json_buffer.createObject();

    //  JSON nested object for motor
    JsonObject & motor_object = sensor_object.createNestedObject("Motor");
    motor_object["Sensor"] = "motor";
    motor_object["Rpm"] = rpm;
    motor_object["Direction"] = "clockwise";

    // JSON double nested object
    JsonObject & motor_nested_object =
        motor_object.createNestedObject("Surrondings");

    //  JSON triple nested object
    JsonObject & motor_double_nested_object =
        motor_nested_object.createNestedObject("Temperature");
    motor_double_nested_object["C"] = 35;
    motor_double_nested_object["F"] = 95;

    //  Going back to doubled nested object
    motor_nested_object["Current"] = 20;
    //std::cout<<std::endl<<"The size is "<<json_buffer.size()<<std::endl;
    //  end of JSON object motor
    //  JSON nested object for camera
    JsonObject & camera_object = sensor_object.createNestedObject("Camera");
    camera_object["Sensor"] = "Logitech C930e webcam";
    camera_object["Resolution"] = "1280x720";
    camera_object["HD"] = "yes";
    camera_object["Field of View"] = 90;

    //  JSON nested object for LIDAR
    JsonObject & lidar_object = sensor_object.createNestedObject("Lidar");
    lidar_object["Sensor"] = "LIDAR";

    //  JSON nested array for LIDAR
    JsonArray & alignment_array_object =
        lidar_object.createNestedArray("Alignment Array");

    //  JSON nested object inside the nested array for LIDAR
    JsonObject & alignment_object_1 =
        alignment_array_object.createNestedObject();
    alignment_object_1["Distance_m"] = 1280;
    /*alignment_object_1["Mount Position"] = "Front";
    alignment_object_1["Field of View"] = 180;

    //  JSON nested object inside the nested array for LIDAR
    JsonObject & alignment_object_2 =
        alignment_array_object.createNestedObject();
    alignment_object_2["Distance_m"] = 1280;
    alignment_object_2["Mount Position"] = "Rear";
    alignment_object_2["Field of View"] = 180;

    //  JSON nested object inside the nested array for LIDAR
    JsonObject & alignment_object_3 =
        alignment_array_object.createNestedObject();
    alignment_object_3["Distance_m"] = 1280;
    alignment_object_3["Mount Position"] = "left";
    alignment_object_3["Field of View"] = 180;

    //  JSON nested object inside the nested array for LIDAR
    JsonObject & alignment_object_4 =
        alignment_array_object.createNestedObject();
    alignment_object_4["Distance_m"] = 1280;
    alignment_object_4["Mount Position"] = "right";
    alignment_object_4["Field of View"] = 180;*/

    //  JSON encoding
    std::string message;
    sensor_object.printTo(message);
    //std::cout<<std::endl<<"The size is "<<json_buffer.size()<<std::endl;
    //std::cout<<std::endl<<"the encoded message is "<<message<<std::endl;
    return message;
}

int main(void)
{

    init_rs232();
    enable_rs232_interrupts();
    enable_rs232();

    /* Create the queue to hold messages to be written to the RS232. */
    serial_str_queue = xQueueCreate( 10, sizeof( serial_str_msg ) );
    vSemaphoreCreateBinary(serial_tx_wait_sem);
    serial_rx_queue = xQueueCreate( 1, sizeof( serial_ch_msg ) );


    /* Create tasks to queue a string to be written to the RS232 port. */
    xTaskCreate( queue_str_task1, ( signed portCHAR * ) "Serial Write 1", 512 /* stack size */, NULL, tskIDLE_PRIORITY + 10, NULL );
    xTaskCreate( queue_str_task2, ( signed portCHAR * ) "Serial Write 2", 512 /* stack size */, NULL, tskIDLE_PRIORITY + 10, NULL );

    /* Create a task to write messages from the queue to the RS232 port. */
    xTaskCreate(rs232_xmit_msg_task, ( signed portCHAR * ) "Serial Xmit Str", 512 /* stack size */, NULL, tskIDLE_PRIORITY + 2, NULL );

    /* Create a task to receive characters from the RS232 port and echo them back to the RS232 port. */
    xTaskCreate(serial_readwrite_task, ( signed portCHAR * ) "Serial Read/Write", 512 /* stack size */, NULL, tskIDLE_PRIORITY + 10, NULL );

    /* Create a task to receive characters from the RS232 port and echo them back to the RS232 port. */
    xTaskCreate(json_encoding_task, ( signed portCHAR * ) "JSON encoding and Serial write", 1024 /* stack size */, NULL, tskIDLE_PRIORITY + 15, NULL );

    /* Start running the tasks. */
    vTaskStartScheduler();

    return 0;
}

void vApplicationTickHook( void )
{
}
