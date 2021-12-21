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

#ifndef GREENHOUSE_ARDUINO_ESPCONTROL_H
#define GREENHOUSE_ARDUINO_ESPCONTROL_H


class EspControl{
public:

    EspControl();

    void update_esp();

private:
    String esp_string = "";
    unsigned long time_turn_on;
//=======! отправка данных на esp !=======
    void regular_send();

    String generate_answer();

    int return_sensor_val(String key);

    void esp_write(String send_data);

    //=======! прием данных с ESP !=======

    void over_read();

    void data_processing(String priem_c_ESP_str);

    void reset_priority();
    String esp_read();


};


#endif //GREENHOUSE_ARDUINO_ESPCONTROL_H
