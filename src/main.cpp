
#include <Arduino.h>

#include <SoftwareSerial.h>
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include "DHT.h"



void setup() {
    // put your setup code here, to run once:
    pinMode(13, OUTPUT);
    Serial.begin(115200);
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
}

void loop() {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    Serial.println("--");
}
