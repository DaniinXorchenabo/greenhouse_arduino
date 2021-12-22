
#include <Arduino.h>

#include <SoftwareSerial.h>
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include "DHT.h"
//#include <json/value.h>
#include <ArduinoJson.h>

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

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, data);
        if (!doc.isNull()) {
            if (!doc["set"].isNull()) {
                if (!doc["set"]["led"].isNull()) {
                    serializeJson(doc["set"]["led"], Serial);
                    Serial.print("\n");
                }
                if (!doc["set"]["reb_blue_led"].isNull()) {
                    serializeJson(doc["set"]["reb_blue_led"], Serial);;
                    Serial.print("\n");
                }
                if (!doc["set"]["humidity"].isNull()) {
                    serializeJson(doc["set"]["humidity"], Serial);;
                    Serial.print("\n");
                }
                if (!doc["set"]["root_humidity"].isNull()) {
                    serializeJson(doc["set"]["root_humidity"], Serial);;
                    Serial.print("\n");
                }
                if (!doc["set"]["temperature"].isNull()) {
                    serializeJson(doc["set"]["temperature"], Serial);;
                    Serial.print("\n");
                }
                if (!doc["set"]["fan"].isNull()) {
                    serializeJson(doc["set"]["fan"], Serial);;
                    Serial.print("\n");
                }
                if (!doc["set"]["root_fan"].isNull()) {
                    serializeJson(doc["set"]["root_fan"], Serial);;
                    Serial.print("\n");
                }
            }
            if (doc["get"].isNull()) {

            }
            if (doc["action"].isNull()) {
                doc["action"]["water"];
            }
            Serial.println("-----");
            serializeJson(doc, Serial);
            Serial.println("\ninto if");
        }
        Serial.print("rpi_get: " + data + "||");
        Serial.println((!doc.isNull()));
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
                      "}"
                      "}";
        Serial.println(data);
    }
}
