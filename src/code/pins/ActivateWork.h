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

#ifndef GREENHOUSE_ARDUINO_ACTIVATEWORK_H
#define GREENHOUSE_ARDUINO_ACTIVATEWORK_H


class ActivateWork {
public:
    String key = "";
    String key_senser = "";
    int low_level = 50;
    int high_level = 50;
    int8_t how_func = 1;

    ActivateWork();

    ActivateWork(String key_n, String key_senser_n, int low_level_n, int high_level_n);

    ActivateWork(String key_n, String key_senser_n, int low_level_n, int high_level_n, int8_t how_func_n);

    void activate_change();

    void activate_change(int8_t how_func_n);

    void activate_change_key(String key_n);

    void activate_change(int8_t how_func_n, String key_n);

    void standart_activation_func();

    void standart_activation_func(String key_n);

    void revers_standart_activation_func(String key_n);

    void poliv_activation_func(String key_n);

    void test_activ_func();
};


#endif //GREENHOUSE_ARDUINO_ACTIVATEWORK_H
