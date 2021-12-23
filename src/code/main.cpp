
//
// Created by Daniil on 21.12.2021.
//

#include <Arduino.h>

#include <SoftwareSerial.h>
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <code/pins/PinControl.h>
#include <code/pins/AnalogReadPin.h>
#include <code/serials/RaspberryPiControl.h>
#include <code/serials/EspControl.h>
#include <code/serials/Bluetooth.h>

#include "DHT.h"
#include "consts.h"


int POLIV_DELAY = 10000;
int POLIV_LOW_LEVEL = 30;
int8_t VAPOR_ACTIVATION_LEVLE = 60;
int8_t VAPOR_DISACTIVATION_LEVLE = VAPOR_ACTIVATION_LEVLE + 20;
int8_t GAS_ACTIVATION_LEVLE = 40;
int8_t GAS_DISACTIVATION_LEVLE = GAS_ACTIVATION_LEVLE - 15;
int8_t TEMP_FAN_ACTIVATION_LEVLE = 26;
int8_t TEMP_FAN_DISACTIVATION_LEVLE = TEMP_FAN_ACTIVATION_LEVLE - 2;


using namespace std;

DHT dht(7, DHT11);
//SoftwareSerial raspb(53, 51);
//SoftwareSerial blut(52, 50);
//RaspberryPiControl rasClass(raspb);



std::map<String, PinControl> dig_pins;
std::map<String, sens_val_strucr> sensors_val;


RaspberryPiControl rasClass(Serial);
//EspControl espControl;
//Bluetooth bluetooth(blut);

ActivateWork activate_vapor("vapor", "hum", VAPOR_ACTIVATION_LEVLE, VAPOR_DISACTIVATION_LEVLE,
                            1); //и для испарителя и для его вентилятора
ActivateWork activate_root_fan("fan_root", "gas", GAS_DISACTIVATION_LEVLE, GAS_ACTIVATION_LEVLE, 2);
//ActivateWork activate_temp_fan("fan_air", "temp", TEMP_FAN_DISACTIVATION_LEVLE, TEMP_FAN_ACTIVATION_LEVLE, 2);
//ActivateWork activate_poliv("poliv", "root_hum", POLIV_LOW_LEVEL, 0, 3);
ActivateWork activate_test("test", "test", 1, 2, 100);

extern int __bss_end;
extern void *__brkval;

// Функция, возвращающая количество свободного ОЗУ
int memoryFree() {
    // check it for more details: https://alexgyver.ru/lessons/code-optimisation/
    int freeValue;
    if ((int) __brkval == 0)
        freeValue = ((int) &freeValue) - ((int) &__bss_end);
    else
        freeValue = ((int) &freeValue) - ((int) __brkval);
    return freeValue;
}

void update_pin() {
    for (auto it = dig_pins.begin(); it != dig_pins.end(); ++it) {
        //Serial.println((String)it->first);
        //Serial.println("--------------------------------");
        it->second.update_pin(it->first);
    }
}

void update_sensors_value() {
    int data;
    for (auto it = sensors_val.begin(); it != sensors_val.end(); ++it) {
        data = (int) it->second.cls.get_value();
        if (data != -1) {
            it->second.value = (float) data;
        }
//        Serial.print(String(it->first) + " " + String(it->second.value) + "\t");
    }
//    Serial.println();
}


void setup() {
    //dht.begin();

    Serial.begin(9600);
    dht.begin();
    Serial.println("^trtr");
    Serial.println("free memory: " + (String) memoryFree());

    dig_pins["whiteLed"] = PinControl(8, LOW);
    dig_pins["fitoLed"] = PinControl(9, LOW);


    //dig_pins["air"] = PinControl(30);
//    dig_pins["fan_air"] = PinControl(33, activate_temp_fan);
    dig_pins["fan_air"] = PinControl(33);
    dig_pins["vapor"] = PinControl(30, activate_vapor);
    dig_pins["fan_root"] = PinControl(28, activate_root_fan);//4
    dig_pins["fan_vapor"] = PinControl(26, activate_vapor);
    dig_pins["poliv"] = PinControl(13, LOW /*настоящий: 24, activate_poliv*/);

//    dig_pins["test"] = PinControl(13, LOW); //, activate_test

    Serial.println("000");
    sensors_val["temp"] = (sens_val_strucr) {23, AnalogReadPin(dht, (String) "t")};
    Serial.println("000");
    sensors_val["hum"] = (sens_val_strucr) {70, AnalogReadPin(dht, (String) "h")};
    Serial.println("000");
    sensors_val["gas"] = (sens_val_strucr) {5, AnalogReadPin(0, true)};
    Serial.println("000");
    sensors_val["level_poliv"] = (sens_val_strucr) {500, AnalogReadPin(1)};
    sensors_val["level_vapor"] = (sens_val_strucr) {500, AnalogReadPin(2)};
    sensors_val["root_hum"] = (sens_val_strucr) {80, AnalogReadPin(3, true)};


    /*
    Serial.println("rtrtr ");
    //rasClass.test();
    Serial.println("rtrtr ");
    Serial.println(sensors_val["gas"].value);
    */
//    dig_pins.find("fan_air")->second.edit_status_pin(true);

/*
dig_pins.find("test")->second.set_priority(39);//19000
dig_pins.find("test")->second.set_priority(5);//11405
dig_pins.find("test")->second.set_priority(0);//11400

dig_pins.find("test")->second.set_priority(31);//11000

dig_pins.find("test")->second.set_priority(25);//11500
dig_pins.find("test")->second.set_priority(24);//11400
dig_pins.find("test")->second.set_priority(5);//11405
//dig_pins.find("test")->second.set_priority(0);//11400
dig_pins.find("test")->second.set_priority(35);//15405
*/

//
    dig_pins["fitoLed"].edit_status_pin(true);
    dig_pins["whiteLed"].edit_status_pin(false);
//dig_pins["poliv"].edit_status_pin(false, 39, true);
    Serial.println("free memory: " + (String) memoryFree());
}


void loop() {

    update_pin();
    rasClass.raspb_update();
    //bluetooth.update_blut();
//    espControl.update_esp();
    update_sensors_value();
//    delay(500);

}



