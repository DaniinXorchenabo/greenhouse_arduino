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

#ifndef GREENHOUSE_ARDUINO_RASPBERRYPICONTROL_H
#define GREENHOUSE_ARDUINO_RASPBERRYPICONTROL_H



class RaspberryPiControl{
public:
//    SoftwareSerial *raspb;
    HardwareSerial *raspb;
    String start_day = "None";

    RaspberryPiControl(SoftwareSerial &ras);
    RaspberryPiControl(HardwareSerial &ras);

    void raspb_update();


private:
    String new_str_raspb = "";
    String str_raspb = "";
    unsigned long time_turn_on;



    //=======! чтение с распберри !=======
    void over_read_raspb();

    String read_raspb();

    //=======! Обработка полученных данных !=======
    void data_processing(String str);

    String get_val_sens();

    //=======! отправление данеых !=======
    void regular_send();

    void write_raspb(String text);

};


#endif //GREENHOUSE_ARDUINO_RASPBERRYPICONTROL_H
