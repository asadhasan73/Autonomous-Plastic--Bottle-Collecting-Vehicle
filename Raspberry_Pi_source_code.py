'''
Raspberry Pi Source Code designed to detect plastic bottles:
This script is using a library named "ptvsd" to enable remote debugging, followed by importing various other libraries, 
including OpenCV, "PiCamera" and "serial" libraries. Then, it initializes the PiCamera with specified parameters and loads 
a classifier from a file named "bottle.xml". After that, it captures video frames from the camera and performs object detection 
on the image using the loaded classifier. If the object is detected, the script draws rectangles around the detected object and 
displays the image. If the object is not found, the script displays a text message indicating so. The script also communicates 
with an Arduino device through the serial port, sending a message based on the position of the detected object in the frame. 
The script continues capturing and processing frames until the user presses "q"
'''

import ptvsd
import cv2
import time
import serial
from picamera import PiCamera
from picamera.array import PiRGBArray

ptvsd.enable_attach(secret='my_secret')

# Load classifier
try:
    CASCADE_FILE = 'bottle.xml'
    cascade = cv2.CascadeClassifier(CASCADE_FILE)
except:
    print("Error loading bottle.xml file")

# Initialize camera
try:
    camera = PiCamera()
    camera.resolution = (320, 240)
    camera.framerate = 32
    camera.awb_mode = 'sunlight'
    camera.meter_mode = 'average'
    camera.iso = 1600
    camera.exposure_mode = 'fixedfps'
    raw_capture = PiRGBArray(camera, size=(320, 240))
except:
    print("Error initializing camera")

# Connect to Arduino
try:
    ser = serial.Serial('/dev/ttyACM0', 9600)
except:
    print("Error connecting to Arduino")

# Allow the camera to warm up
time.sleep(0.1)

# Capture frames from the camera
for frame in camera.capture_continuous(raw_capture, format="bgr", use_video_port=True):
    image = frame.array
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    rectangles = cascade.detectMultiScale(gray, scaleFactor=1.05, minNeighbors=5, minSize=(10, 10))

    right_boxes = 0
    left_boxes = 0

    # Draw rectangles around detected objects and display image
    if len(rectangles):
        for (i, (x, y, w, h)) in enumerate(rectangles):
            cv2.rectangle(image, (x, y), (x + w, y + h), (0, 0, 255), 2)
            cv2.putText(image, "bottle", (x, y - 10), cv2.FONT_HERSHEY_COMPLEX, 0.55, (0, 0, 255), 2)

            if ((x + x + w) / 2) > 160:
                left_boxes += 1
            else:
                right_boxes += 1

        cv2.putText(image, "R: {}".format(right_boxes), (25, 15), cv2.FONT_HERSHEY_COMPLEX, 0.55, (0, 0, 255), 2)
        cv2.putText(image, "L: {}".format(left_boxes), (25, 35), cv2.FONT_HERSHEY_COMPLEX, 0.55, (0, 0, 255), 2)
        cv2.imshow("Frame", image)
    else:
        cv2.putText(image, "Bottle not found", (50, 50), cv2.FONT_HERSHEY_COMPLEX, 0.55, (0, 0, 255), 2)
        cv2.imshow("Frame", image)

    # Send message to Arduino based on position

