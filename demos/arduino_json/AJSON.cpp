// Arduino JSON library
#include "ArduinoJson.h"
// Custom library that contains the declaration of encoding function
#include "JSON.h"

void encoding_JSON()
{
	//  JSON buffer
	char *pcTaskName;

	StaticJsonBuffer<500> json_buffer;
	int rpm = 1000;
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
}

main()
{
	encoding_JSON();
}



