#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";

WiFiServer server(80);

String header;

String output27State = "off";

const int output27 = 27;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

QueueHandle_t xQueue;

void setup() {
    Serial.begin(115200);
    pinMode(output27, OUTPUT);
    digitalWrite(output27, LOW);

    Serial.print("Connecting to "); 
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();

    // Create the FreeRTOS queue
    xQueue = xQueueCreate(10, sizeof(int));

    // Create the FreeRTOS tasks
    xTaskCreate(webServerTask, "WebServerTask", 10000, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(ledControlTask, "LedControlTask", 10000, NULL, 2, NULL);
}

void loop() {
    // Do nothing here. All work is done in FreeRTOS tasks.
}

void webServerTask(void * parameter) {
    for(;;) {
        WiFiClient client = server.available();
        if (client) {
            currentTime = millis();
            previousTime = currentTime;
            Serial.println("New Client.");
            String currentLine = "";
            while (client.connected() && currentTime - previousTime <= timeoutTime) {
            currentTime = millis();
                if (client.available()) {
                    char c = client.read();
                    Serial.write(c);
                    header += c;
                    if (c == '\n') {
                        if (currentLine.length() == 0) {
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();
                            
                            if (header.indexOf("GET /27/on") >= 0) {
                                Serial.println("GPIO 27 on");
                                int ledState = HIGH;
                                xQueueSend(xQueue, &ledState, portMAX_DELAY);
                            } 
                            else if (header.indexOf("GET /27/off") >= 0) {
                                Serial.println("GPIO 27 off");
                                int ledState = LOW;
                                xQueueSend(xQueue, &ledState, portMAX_DELAY);
                            }
                            client.println();
                            break;
                        } 
                        else {
                            currentLine = "";
                        }
                    } 
                    else if (c != '\r') {
                        currentLine += c;
                    }
                }
                vTaskDelay(1);
            }
            header = "";
            client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
  }
}

void ledControlTask(void * parameter) {
    for(;;) {
        int ledState;
        if (xQueueReceive(xQueue, &ledState, portMAX_DELAY)) {
            //Serial.print("Received message from queue.");
            //Serial.println(ledState);
            digitalWrite(output27, ledState);
        }
    }
}