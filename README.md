# Autonomous-Plastic--Bottle-Collecting-Vehicle
Embedded System Design Course Project. I've added source code's of Raspberry Pi and Arduino UNO.

Raspberry Pi was coded in python language to detect plastic bottles utilizing OpenCV:
It initializes the PiCamera with specified parameters and loads a classifier from a file named "bottle.xml". After that, it captures video frames from the camera and performs object detection on the image using the loaded classifier. If the object is detected, the script draws rectangles around the detected object and 
displays the image. If the object is not found, the script displays a text message indicating so. The script also communicates with an Arduino device through the serial port, sending a message based on the position of the detected object in the frame. The script continues capturing and processing frames until the user presses "q"

Arduino UNO was coded in C/C++ to operate the servo motors used to operate on the cage and operate the machine/vehicle:
The code is an implementation of a state machine for a robot that searches for a bottle, captures it, and returns to its starting position. The robot receives commands from a Raspberry Pi via a serial connection. The code implements functions for turning right, turning left, going straight, searching, lowering the cage to capture the bottle, and raising the cage. The code is written in C++ for the Arduino platform and uses the AFMotor and Servo libraries to control the motors and servo. The state of the robot is determined by the "state" variable and its behavior is controlled by the switch case statement based on the command received from the Raspberry Pi.

