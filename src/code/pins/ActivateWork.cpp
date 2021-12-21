//
// Created by Daniil on 21.12.2021.
//

#include "ActivateWork.h"
#include "PinControl.h"
#include <code/consts.h>
#include <code/pins/AnalogReadPin.h>


extern std::map<String, PinControl> dig_pins;
extern std::map<String, sens_val_strucr> sensors_val;

ActivateWork::ActivateWork() {}

ActivateWork::ActivateWork(String key_n, String key_senser_n, int low_level_n, int high_level_n) {
    key = key_n;
    key_senser = key_senser_n;
    low_level = low_level_n;
    high_level = high_level_n;
}

ActivateWork::ActivateWork(String key_n, String key_senser_n, int low_level_n, int high_level_n, int8_t how_func_n) {
    key = key_n;
    key_senser = key_senser_n;
    low_level = low_level_n;
    high_level = high_level_n;
    how_func = how_func_n;
}

void ActivateWork::activate_change() {
    activate_change(how_func, key);
}

void ActivateWork::activate_change(int8_t how_func_n) {
    activate_change(how_func_n, key);
}

void ActivateWork::activate_change_key(String key_n) {
    activate_change(how_func, key_n);
}

void ActivateWork::activate_change(int8_t how_func_n, String key_n) {
    if (how_func_n == 1) {
        standart_activation_func(key_n);
    } else if (how_func_n == 2) {
        revers_standart_activation_func(key_n);
    } else if (how_func_n == 3) {
        poliv_activation_func(key_n);
    } else if (how_func_n == 100) {
        test_activ_func();
    }
}

void ActivateWork::standart_activation_func() {
    standart_activation_func(key);
}


void ActivateWork::standart_activation_func(String key_n){ // 1
    if (sensors_val.find(key_senser) !=  sensors_val.end()){
        if (sensors_val.find(key_senser)->second.value <= low_level){
            PinControl::static_edit_status_pin(key_n, true, 0, true);
        }
        else if (sensors_val.find(key_senser)->second.value >= high_level){
            PinControl::static_edit_status_pin(key_n, false, 0, true);
        }
    }
}

void ActivateWork::revers_standart_activation_func(String key_n){ // 2
    if (sensors_val.find(key_senser) !=  sensors_val.end()){
        if (sensors_val.find(key_senser)->second.value >= high_level){
            PinControl::static_edit_status_pin(key_n, true, 0, true);
        }
        else if (sensors_val.find(key_senser)->second.value < low_level){
            PinControl::static_edit_status_pin(key_n, false, 0, true);
        }
    }
}

void ActivateWork::poliv_activation_func(String key_n){ //3
    if (sensors_val.find(key_senser) !=  sensors_val.end()){
        if (sensors_val.find(key_senser)->second.value <= low_level){
            PinControl::static_turn_on_for_time(key_n, POLIV_DELAY, 0, false);
        }
    }
}

void ActivateWork::test_activ_func(){ // 100
    String key = "simple";
    Serial.println("-------------------------start 13");
    PinControl::static_edit_status_pin("test", true, 0, false);
    delay(3000);
    PinControl::static_edit_status_pin("test", false, 0, false);
    Serial.println("----------------------------end 13");
}