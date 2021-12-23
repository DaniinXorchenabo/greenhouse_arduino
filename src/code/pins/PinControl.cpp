//
// Created by Daniil on 21.12.2021.
//

#include "PinControl.h"
#include <code/pins/PinControl.h>
#include <code/consts.h>

extern std::map<String, PinControl> dig_pins;

PinControl::PinControl(int pin_now,
                       ActivateWork &activate_work_n) { //конструктор класса , bool useless1, bool useless2 , void (*func)(String)
    //activate_func_from_out = func;
    activate_work = &activate_work_n;
    pin_mode_l = LOW;
    starting(pin_now);
}

PinControl::PinControl(int pin_now, boolean pin_mode_now, ActivateWork &activate_work_n) { //конструктор класса
    //activate_func_from_out = func;
    activate_work = &activate_work_n;
    pin_mode_l = pin_mode_now;
    //Serial.println("************************ " + (String)activate_work->how_func);
    starting(pin_now);
}

PinControl::PinControl(int pin_now, boolean pin_mode_now, int over_pin_fill,
                       ActivateWork &activate_work_n) { //конструктор класса
    //activate_func_from_out = func;
    //over_pin_fill = over_pin_fill;
    activate_work = &activate_work_n;

    pin_mode_l = pin_mode_now;
    starting(pin_now);
}

PinControl::PinControl(int pin_now, boolean pin_mode_now, int over_pin_fill) { //конструктор класса
    //over_pin_fill = over_pin_fill;
    pin_mode_l = pin_mode_now;
    starting(pin_now);
}

PinControl::PinControl(int pin_now, boolean pin_mode_now) { //конструктор класса
    pin_mode_l = pin_mode_now;
    starting(pin_now);
}

PinControl::PinControl(int pin_now) { //конструктор класса
    pin_mode_l = LOW;
    starting(pin_now);
}

PinControl::PinControl() {}

void PinControl::run_activation_func(String key) { // МК1 управляет теплицей
    if (activate_work != NULL) {
        activate_work->activate_change_key(key);
    } else {
        //Serial.println("NULL activate func");
    }
}

void PinControl::static_edit_status_pin(String key, bool status_pin, int now_priority, bool save_priority) {
    if (dig_pins.find(key) != dig_pins.end()) {
        //Serial.print((String)key + ":\t");
        if (key.length() < 7) {
            //Serial.print("\t");
        }
        dig_pins.find(key)->second.edit_status_pin(status_pin, now_priority, save_priority);

    }
}

void PinControl::static_turn_on_for_time(String key, int delay_t_now, int now_priority, bool save_priority) {
    if (dig_pins.find(key) != dig_pins.end()) {
        Serial.print((String) key + ": (on time)\t");
        dig_pins.find(key)->second.turn_on_for_time(delay_t_now, now_priority, save_priority);
    }
}

void PinControl::edit_status_pin(bool status_pin) {//постановка статуса пина
    edit_status_pin(status_pin, 1, false);
}

void PinControl::edit_status_pin(bool status_pin, int now_priority) {
    edit_status_pin(status_pin, now_priority, false);
}

void PinControl::edit_status_pin(bool status_pin, int now_priority, bool save_priority) { //постановка статуса пина
    if (status_pin) {
        pin_write_priority(!pin_mode_l, now_priority, save_priority);
    } else {
        is_work_for_time = false;
        pin_write_priority(pin_mode_l, now_priority, save_priority);
    }
}

void PinControl::turn_on_for_time(int delay_t_now) {
    Serial.println("turn_on_for_time 1");
    turn_on_for_time(delay_t_now, 1, false);
}

void PinControl::turn_on_for_time(int delay_t_now, int now_priority) {
    turn_on_for_time(delay_t_now, now_priority, false);
}

void PinControl::turn_on_for_time(int delay_t_now, int now_priority, bool save_priority) {
    Serial.println("turn_on_for_time 3");

    if (pin_write_priority(delay_t_now, now_priority, save_priority, false)) {
        delay(10);
        //Serial.println("turn_on_for_time if 1");
        delay_t = delay_t_now;
        time_turn_on = millis();
        is_work_for_time = true;
        pin_write(!pin_mode_l); /**/
    }
}


void PinControl::update_pin(String key) { //метод, который должен постоянно крутиться в loop
    run_activation_func(key);
    if (is_work_for_time) {
        if (millis() - time_turn_on > delay_t) {
            Serial.println("end_time_work");
            is_work_for_time = false;
            pin_write(pin_mode_l);
        }
    }
}

void PinControl::PWM_mode(int filling) {
    analog_write_pin(filling);
}

void PinControl::add_priority(int t) {}

// =======! Гетторы и Сетторы !=======
void PinControl::set_priority(int now_priority) {
    if (now_priority < 40) {
        int kostil = 0;
        if (now_priority > 9) {
            kostil = priority % (int) my_pow(10, now_priority / 10 + 1) / (int) my_pow(10, now_priority / 10);
            kostil = (int) my_pow(10, now_priority / 10) * kostil;
            priority -= kostil;
        } else {
            priority -= priority % (int) my_pow(10, now_priority / 10 + 1);
        }
        //Serial.println("&&&&&&&%$$$$________" + (String)priority);
        kostil = (int) my_pow(10, (int) (now_priority / 10));
        kostil *= now_priority % 10;
        priority += kostil;
    }
}

int PinControl::get_priority() {
    return priority;
}

int PinControl::get_filling_pin() {
    return pin_filling;
}

bool PinControl::get_working_pin() { // возвращает 1 если пин в режиме работы, 0 - если пин не работает
    return (bool) (pin_mode_l != now_pin_mode);
}


// управление чем-либо с учётом приоритета
void PinControl::pin_write_priority(boolean pin_mod, int now_priority, bool save_priority) {
    pin_write_priority(pin_mod, now_priority, save_priority, true);
}

bool PinControl::pin_write_priority(boolean pin_mod, int now_priority, bool save_priority, bool isworking) {
    //now_priority = |номер ячейки в строке| значение этой ячейки|
    if (levle_priority(now_priority)) {
        if (save_priority){
            set_priority(now_priority);
        }
        //Serial.print("now_priority >= priority " + String(pin) + "\t");
        if (isworking) {
            pin_write(pin_mod);
        }
        return true;
    }
    return false;
}

// включение или выключение чего-либо
void PinControl::pin_write(boolean pin_mod) {
    //Serial.println("pin_write, if " +(String)now_pin_mode + " != " + (String)pin_mod);
    if (now_pin_mode != pin_mod) {
        //Serial.println("if pin_write " + (String)pin + " " + (String)pin_mod);
        digitalWrite(pin, pin_mod);
        now_pin_mode = pin_mod;
    }
}

// вкличение чего-нибудь в режиме ШИМ (Широтно Импульсная Модуляция)
void PinControl::analog_write_pin(int filling) {
    if (pin_filling != filling) {
        pin_filling = filling;
        analogWrite(pin, pin_filling);
    }
}

void PinControl::starting(int pin_now) {
    pin = pin_now;
    Serial.print("inicialization pin ");
    Serial.println(pin_now);
    pinMode(pin_now, OUTPUT);
    pin_write(pin_mode_l);
}

// true - если можно включить что-то
bool PinControl::levle_priority(int got_priority) {
    got_priority = (int) pow(10, got_priority / 10) * got_priority % 10;
    if (priority % 10000 <= got_priority) {
        return true;
    }
    return false;
}

// стандартное возмедение в степень работает криво, это его аналог
int PinControl::my_pow(int num, int _step) {
    int new_num = 1;
    for (int i = 0; i < _step; ++i) {
        new_num *= num;
    }
    return new_num;
}