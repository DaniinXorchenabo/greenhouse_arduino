
#include <Arduino.h>

#include <SoftwareSerial.h>
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include "DHT.h"

unsigned long time_turn_on;

void setup() {
    // put your setup code here, to run once:
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
}

void _loop() {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    Serial.println("--");
}

void loop() {

//    Serial.print("-");

    if (Serial.available()) {
        String data = "";
        Serial.setTimeout(50);
        data = Serial.readString();
        Serial.println("rpi_get: " + data);
    }
    delay(100);

    if (millis() - time_turn_on > 2000) {
        time_turn_on = millis();
        String data = String("") +
               "{\"metrics\": {" +
               "\"temperature\":" + (String) rand() + ", " +
               "\"humidity\": " + (String) rand() + ", " +
               "\"root_humidity\": " + (String) rand() + ", " +
               "\"water_level\": " + (String) rand() + ", " +
               "\"water_humidity_level\": " + (String) rand() +
               "}";
        Serial.println(data);
    }
}
