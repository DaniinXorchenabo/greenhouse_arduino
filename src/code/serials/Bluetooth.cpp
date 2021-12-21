//
// Created by Daniil on 21.12.2021.
//

#include <code/pins/PinControl.h>
#include <code/consts.h>
#include "Bluetooth.h"

extern std::map<String, PinControl> dig_pins;

Bluetooth::Bluetooth(SoftwareSerial &blut_obj) {
    blut = &blut_obj;
    blut->begin(9600);
}

void Bluetooth::update_blut() {
    tolking_with_android();
}

void Bluetooth::test(String data) {
    parser_data(data, "/");
}

void Bluetooth::tolking_with_android() {
    over_blut_read();
}

//=======! Чтение !=======
void Bluetooth::over_blut_read() {
    String blut_data = blut_read();
    if (blut_data != "") {
        data_processing(blut_data);
    }
}

String Bluetooth::blut_read() {
    //blut->listen();
    if (blut->available()) {
        String date = (String) blut->readString();
        return date;
    }
    return "";
}

//=======! Обработка полученных данных !=======
void Bluetooth::data_processing(String blut_data) {
    if (blut_data.substring(0, 8) == "set_day:") {
        String start_day = blut_data.substring(9);
    } else if (blut_data.substring(0, 1) == "/") {
        if (get_segment_for_ans(blut_data, "/", 1) == "1") {
            // работаем в ручном реживе (-1 в автоматическом)
            is_last_mode_avto = false;
            parser_data(blut_data, "/");
        } else if (get_segment_for_ans(blut_data, "/", 1) == "-1") {
            //работаем в автоматическом режиме
            if (!is_last_mode_avto) {
                reset_priority();
                is_last_mode_avto = true;
            }
        }
    }
}

void Bluetooth::reset_priority() {
    for (auto it = dig_pins.begin(); it != dig_pins.end(); ++it) {
        it->second.set_priority(20);
    }
}

String Bluetooth::get_segment_for_ans(String data, String divider, int number) {
    int first_ind = -1;
    int second_ind = data.indexOf(divider);
    data.trim();
    data += ">>>";
    int len_data = data.length();
    int count = 0;
    while (second_ind != -1) {
        if (second_ind == -1 || len_data < second_ind || len_data <= first_ind) {
            break;
        }
        ++count;
        if (count == number) {
            return data.substring(first_ind + 1, second_ind);
        }
        first_ind = second_ind;
        second_ind = data.indexOf(divider, first_ind + 1);
    }
    return (String) "";
}

void Bluetooth::parser_data(String data, String divider) {
    //ручной режим работы
    int first_ind = -1;
    int second_ind = data.indexOf(divider);
    int counter = 0;
    data.trim();
    data += ">>>";
    int len_data = data.length();
    while (second_ind != -1) {
        if (second_ind == -1 || len_data < second_ind || len_data <= first_ind) {
            break;
        }
        String sub_str = data.substring(first_ind + 1, second_ind);
        processing_data_by_one(counter, sub_str);
        first_ind = second_ind;
        second_ind = data.indexOf(divider, first_ind + 1);
        ++counter;
    }
}

void Bluetooth::processing_data_by_one(int number_data, String status_pin) {
    if (number_data == 0) {
        //это статус авто или нет
    } else if (number_data == 1) {
        //нашреватель
    } else if (number_data == 2) {
        //охлодитель
    } else if (number_data == 3) {
        //вентилятор на проветривание
    } else if (number_data == 4) {
        //вентилятор корней
    } else if (number_data == 5) {
        //полив
    } else if (number_data == 6) {
        //испаритель
    }

}


//=======! Отправка данных обратоно !=======
void Bluetooth::blut_write(String messenge) {
    blut->println(messenge);
}

String Bluetooth::generate_answer() {
    //answer = "нагреватель охлодитель вентилятор_на_проветривание вентилятор_корней полив испаритель"
    String answer = "-1 -1 " + formating_status_pin("fan_air") + " ";
    answer += formating_status_pin("fan_root") + " -1 ";
    return answer + formating_status_pin("vapor");
}

String Bluetooth::formating_status_pin(String key) {
    bool _status = get_status_pin(key);
    if (_status == true) {
        return String("1");
    }
    return String("-1");
}

bool Bluetooth::get_status_pin(String key) {
    if (dig_pins.find(key) != dig_pins.end()) {
        return (bool) dig_pins[key].get_working_pin();
    }
    return false;
}

//=======! Изменение статуса пина (включение или выключение) !=======
void Bluetooth::set_status_pin_for_str(String key, String _st) {
    if (dig_pins.find(key) != dig_pins.end()) {
        if (_st == "1") {
            //включить
            set_status_pin(key, true, true);
        } else if (_st == "-1") {
            //выключить
            set_status_pin(key, false, true);
        }
        if (_st != "3" && _st != "0") {}
    }
}

void Bluetooth::set_status_pin(String key, bool working) {
    set_status_pin(key, working, true);
}

void Bluetooth::set_status_pin(String key, bool working, bool save_priority) {
    if (dig_pins.find(key) != dig_pins.end()) {
        if (key != "poliv") {
            dig_pins.find(key)->second.edit_status_pin(working, 21, save_priority);
        } else {
            dig_pins.find(key)->second.turn_on_for_time(POLIV_DELAY, 21);
        }
    }
}