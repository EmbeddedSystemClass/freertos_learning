#include "ArduinoJson.h"
//#include <FreeRTOS.h>
//#include <task.h>

//#include <iostream>

void encoding_JSON(int rpm)
{
	//  JSON buffer
	//char *pcTaskName;
	//const TickType_t xDelay250ms = pdMS_TO_TICKS( 250 );

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
#if 0
	for(;;) {
		std::string message;
		sensor_object.printTo(message);
		vPrintString("Inside the task1 and the message is");
		vPrintString(message);
		vTaskDelay(xDelay250ms);
	}
#endif
	//std::cout<<std::endl<<"The size is "<<json_buffer.size()<<std::endl;
	//std::cout<<std::endl<<"the encoded message is "<<message<<std::endl;
	
}


int main()
{
#if 0
	if ( pdPASS != xTaskCreate(encoding_JSON, "encoding", 1000/*heap size*/, 1000/*fucntion argument*/,
                               1, NULL) )
    {
        FreeRTOS_Error("Cannot create tasks\r\n");
    }

    if ( pdPASS != xTaskCreate(simple_printing, "sample task", 4/*heap size*/, NULL/*fucntion argument*/,
                               2, NULL) )
    {
        FreeRTOS_Error("Cannot create tasks\r\n");
    }
    vTaskStartScheduler();    
#endif
    for(;;){
    	encoding_JSON(1000);
    }
}