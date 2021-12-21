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
#include "ActivateWork.h"

#ifndef GREENHOUSE_ARDUINO_PINCONTROL_H
#define GREENHOUSE_ARDUINO_PINCONTROL_H


class PinControl{
    /*
    коротко о проекте:
        теплица, работающая по принципу аэропоники (корни в воздухе)
        выращивает продукты с высоким содержанием витаминов (не будем в это углубляться)
        в состав теплицы входят:
            куча датчиков и всяких реле
            МК1 - микроконтроллер1 (Arduino Mega)
            МК2 - микрокомпьютер2 (Raspberry Pi B3)
            модуль Bluetooth
            модуль Wi-Fi (ESP8266-01)
            сервер (один на великое множество теплиц)
            Android-приложение
            Web-сайт (сервер != Web-сайт)

        теплица может работать в автоматическом и ручном режимах

        ручной режим имеет приоритет над автоматическим

        приоритетность управляющих сигналов (какое устройство управляет теплицей)
        - от более приоритетного к менее приоритетному
            Web-сайт
            Android-приложение
            МК2
            МК1 (если МК2 отсутствует в теплице, или связь с ним по каким-то причинам потеряна)

        тут представлен код (фрагмент кода) для МК1
        его роль, по сути сводится к получению и отправке на другие управляющие устройства показаний датчиков и
        к получению и выполнению команд включения или выключения различных реле (включить отопление или выключить свет)
        при всем этом МК1 может работать и самостоятельно.

      О работе самого класса:
          есть переменная int priority = 10000;
          каждый разряд (кроме первой 1) соответствует управляющему устройству
          0 - если управляющее устройство на данный момент не управляет теплицей
          1 - если управляет
          (по всей видимости надо будет дороботать. 1 - управляет, но устройство выключено,
          9 - управляет, устройство включено.
          Промежуточные значения - возможность для ШИМ модуляции (к примеру, для освещения))

        Подобный подхот, по сути, позволяет теплице одновременно работать во всех режимах одновременно,
        Но физически теплица будет работать только в режиме с наивысшим приоритетом

          update_pin - метод, который постоянно будет крутиться в цикле работы
          edit_status_pin - метод, позволяющий включить или выключить что-нибудь
          turn_on_for_time - метод, позволяющий включить или выключить что-нибудь на заданное время
          PWM_mode - метод, позволяющий использовать ШИМ для освещения
    */
public:
    int8_t pin = 43; //  к этому пину ничего не подключено
    boolean pin_mode_l = LOW; // дело в том, что на некоторых реле низкий уровень включает нагрузку
    boolean now_pin_mode = pin_mode_l; //изначально, при старте работы, нагрузка не включена
    //void (*activate_func_from_out)(String) = NULL;
    ActivateWork *activate_work = NULL; // это когда МК1 управляет работой теплицы,
    // в переданном объекте содержится функция управления нагрузкой

    // =======! Конструкторы !=======
    PinControl(int pin_now, ActivateWork &activate_work_n);
    PinControl(int pin_now, boolean pin_mode_now, ActivateWork &activate_work_n);
    PinControl(int pin_now, boolean pin_mode_now, int over_pin_fill, ActivateWork &activate_work_n);
    PinControl(int pin_now, boolean pin_mode_now, int over_pin_fill);
    PinControl(int pin_now, boolean pin_mode_now);
    PinControl(int pin_now);
    PinControl();

    void run_activation_func(String key);

    //используется при управлении теплицей МК1
    static void static_edit_status_pin(String key, bool status_pin, int now_priority, bool save_priority);

    //используется при управлении теплицей МК1
    static void static_turn_on_for_time(String key, int delay_t_now, int now_priority, bool save_priority);

    void edit_status_pin(bool status_pin);
    void edit_status_pin(bool status_pin, int now_priority);
    void edit_status_pin(bool status_pin, int now_priority, bool save_priority);

    void turn_on_for_time(int delay_t_now);
    void turn_on_for_time(int delay_t_now, int now_priority);
    void turn_on_for_time(int delay_t_now, int now_priority, bool save_priority);

    void update_pin(String key);

    void PWM_mode(int filling);

    void add_priority(int t);

    // =======! Гетторы и Сетторы !=======
    void set_priority(int now_priority);
    int get_priority();
    int get_filling_pin();
    bool get_working_pin();

private:
    unsigned long time_turn_on;
    bool is_work_for_time = false;
    unsigned int delay_t;
    int pin_filling = 0;
    //int over_pin_fill = 0;
    int priority = 10000; // любое число|с сайта|с мобильного приложения|с разбери|с датчиков или по времени|

    // управление чем-либо с учётом приоритета
    void pin_write_priority(boolean pin_mod, int now_priority, bool save_priority);
    bool pin_write_priority(boolean pin_mod, int now_priority, bool save_priority, bool isworking);

    // включение или выключение чего-либо
    void pin_write(boolean pin_mod);

    // вкличение чего-нибудь в режиме ШИМ (Широтно Импульсная Модуляция)
    void analog_write_pin(int filling);

    void starting(int pin_now);

    // true - если можно включить что-то
    bool levle_priority(int got_priority);

    // стандартное возмедение в степень работает криво, это его аналог
    int my_pow(int num, int _step);

};

#endif //GREENHOUSE_ARDUINO_PINCONTROL_H
