//
// Created by Daniil on 21.12.2021.
//
#include <SoftwareSerial.h>
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
//#include <vector>
#include <iterator>
#include <map>
#include "DHT.h"

#ifndef GREENHOUSE_ARDUINO_CONSTS_H
#define GREENHOUSE_ARDUINO_CONSTS_H

int POLIV_DELAY = 10000;
int POLIV_LOW_LEVEL = 30;
int8_t VAPOR_ACTIVATION_LEVLE = 60;
int8_t VAPOR_DISACTIVATION_LEVLE = VAPOR_ACTIVATION_LEVLE + 20;
int8_t GAS_ACTIVATION_LEVLE = 40;
int8_t GAS_DISACTIVATION_LEVLE = GAS_ACTIVATION_LEVLE - 15;
int8_t TEMP_FAN_ACTIVATION_LEVLE = 26;
int8_t TEMP_FAN_DISACTIVATION_LEVLE = TEMP_FAN_ACTIVATION_LEVLE - 2;

#endif //GREENHOUSE_ARDUINO_CONSTS_H
