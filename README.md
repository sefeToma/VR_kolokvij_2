# Overview
The main.ino file is an Arduino sketch for a WiFi server that controls an LED using FreeRTOS tasks. The server is set up to connect to a specified WiFi network and then listen for incoming HTTP requests. The state of the LED is controlled by the ledControlTask FreeRTOS task, which receives its instructions from a queue.

## Prerequisites
- Arduino IDE
- ESP32 board
- WiFi network

## Setup
1. Open main.ino in the Arduino IDE.
2. Replace "SSID" and "PASSWORD" with your WiFi network's SSID and password.
3. Connect your ESP32 board to your computer.
4. Select the correct board and port in the Arduino IDE.
5. Upload the sketch to your ESP32 board.


## Usage
Once the sketch is running on the ESP32, it will connect to the WiFi network and print its IP address to the Serial Monitor. You can then send HTTP requests to this IP address to control the LED.

## Key Functions
- setup(): Initializes the serial communication, sets the LED pin as output, connects to the WiFi network, starts the WiFi server, and creates the FreeRTOS tasks and queue.
- loop(): An empty function. All work is done in the FreeRTOS tasks.
- webServerTask(void * parameter): A FreeRTOS task that listens for incoming HTTP requests and sends responses.
- ledControlTask(void * parameter): A FreeRTOS task that controls the state of the LED based on messages received from the queue.

## Variables
- ssid and password: Strings that hold the SSID and password of your WiFi network.
- server: An instance of WiFiServer that listens for incoming HTTP requests.
- header: A string that holds the HTTP request header.
- output27State: A string that holds the state of the LED ("on" or "off").
- output27: An integer that represents the GPIO pin number of the LED.
- currentTime and previousTime: Unsigned long integers used for non-blocking delay.
- timeoutTime: A long integer that specifies the timeout time in milliseconds.
- xQueue: A handle for the FreeRTOS queue.

## Note
This sketch uses FreeRTOS, which is a real-time operating system for microcontrollers. FreeRTOS allows you to write more complex, reliable, and responsive programs.
