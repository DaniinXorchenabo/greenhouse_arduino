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

#ifndef GREENHOUSE_ARDUINO_BLUETOOTH_H
#define GREENHOUSE_ARDUINO_BLUETOOTH_H


class Bluetooth{
public:
    SoftwareSerial *blut;

    Bluetooth(SoftwareSerial &blut_obj);

    void update_blut();

    void test(String data);

private:
    bool is_last_mode_avto = true; //чтобы каждый раз не перезаписывать приоритет пинов

    void tolking_with_android();

    //=======! Чтение !=======
    void over_blut_read();

    String blut_read();

    //=======! Обработка полученных данных !=======
    void data_processing(String blut_data);

    void reset_priority();

    String get_segment_for_ans(String data, String divider, int number);

    void parser_data(String data, String divider);

    void processing_data_by_one(int number_data, String status_pin);


    //=======! Отправка данных обратоно !=======
    void blut_write(String messenge);

    String generate_answer();

    String formating_status_pin(String key);

    bool get_status_pin(String key);

    //=======! Изменение статуса пина (включение или выключение) !=======
    void set_status_pin_for_str(String key, String _st);

    void set_status_pin(String key, bool working);

    void set_status_pin(String key, bool working, bool save_priority);

};

#endif //GREENHOUSE_ARDUINO_BLUETOOTH_H
