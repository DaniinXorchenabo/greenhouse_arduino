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
#include "DHT.h"

#ifndef GREENHOUSE_ARDUINO_ANALOGREADPIN_H
#define GREENHOUSE_ARDUINO_ANALOGREADPIN_H


class AnalogReadPin {
public:

    AnalogReadPin();

    AnalogReadPin(int pin_now, bool def_n);

    AnalogReadPin(int pin_now);

    AnalogReadPin(DHT &dht_new, String param_dht);

    int get_value();

private:
    bool DHT_flag = false;
    bool how_param = false;
    bool def = false;
    int8_t pin;
    DHT *dht;

    int pin_analogRead();

    int return_temperature();

    int return_humid();
};

struct sens_val_strucr {
    float value;
    AnalogReadPin cls;
};

#endif //GREENHOUSE_ARDUINO_ANALOGREADPIN_H
