//
// Created by Daniil on 21.12.2021.
//

#include "RaspberryPiControl.h"
#include <ArduinoJson.h>

#include <code/pins/PinControl.h>
#include <code/pins/AnalogReadPin.h>

extern std::map<String, PinControl> dig_pins;
extern std::map<String, sens_val_strucr> sensors_val;
extern int POLIV_DELAY;

//RaspberryPiControl::RaspberryPiControl(SoftwareSerial &ras) {
//    raspb = &ras;
//    raspb->begin(9600);
//    write_raspb("I started (uno)");
//}

RaspberryPiControl::RaspberryPiControl(HardwareSerial &ras) {
    raspb = &ras;
//    raspb->begin(9600);
    write_raspb("I started (uno)");
}

void RaspberryPiControl::raspb_update() {
    over_read_raspb();
    regular_send();
}

//=======! чтение с распберри !=======
void RaspberryPiControl::over_read_raspb() { // считывается строка, полученная с разбери
    while (raspb->available() > 0) {
        String raspb_data = read_raspb();
        if (raspb_data != "") {
            raspb_data.trim();
            data_processing(raspb_data);
        }
    }
}

String RaspberryPiControl::read_raspb() {
//    Serial.print("-");

//    if (!raspb->isListening()) {
//        bool useless = raspb->listen();
//    }

    if (raspb->available()) {
        Serial.println("разбери что-то прислал...");

//        raspb->setTimeout(10);
        String data = raspb->readStringUntil('\n');
//        String data;
//        while (raspb->available() > 0){
//          data += (String)raspb->peek();
//          if (raspb->read() == '\n'){break;}
//        }
        //delay(50);
//        Serial.println("raspb_get: " + data);
        return data;
    }
    return "";
}

//=======! Обработка полученных данных !=======
void RaspberryPiControl::data_processing(String str) { // обработка строки
    write_raspb("I get: " + str + "%%*");
//    Serial.println("Получил от разбери: " + str);
    str_raspb = str;
    if (str == "WhiteLedHIGH") {
        if (dig_pins.find("whiteLed") != dig_pins.end()) {
            dig_pins.find("whiteLed")->second.edit_status_pin(true, 11, true);
        }
    } else if (str == "WhiteLedLOW") {
        if (dig_pins.find("whiteLed") != dig_pins.end()) {
            dig_pins.find("whiteLed")->second.edit_status_pin(false, 11, true);
        }
    } else if (str == "fitoLedHIGH") {
        if (dig_pins.find("fitoLed") != dig_pins.end()) {
            dig_pins.find("fitoLed")->second.edit_status_pin(true, 11, true);
        }
    } else if (str == "fitoLedLOW") {
        if (dig_pins.find("fitoLed") != dig_pins.end()) {
            dig_pins.find("fitoLed")->second.edit_status_pin(false, 11, true);
        }
    } else if (str == "polivHIGH") {
        if (dig_pins.find("poliv") != dig_pins.end()) {
            dig_pins.find("poliv")->second.turn_on_for_time(POLIV_DELAY, 11);
        }
    } else if (str == "get_parametrs") { write_raspb(get_val_sens()); }
    else if (str == "Give_start_day") { write_raspb("start_day " + start_day); }

    if (str_raspb != str) {
    }


    DynamicJsonDocument doc(2048);
    deserializeJson(doc, str);
    if (!doc.isNull()) {
        if (!doc["set"].isNull()) {
            if (!doc["set"]["led"].isNull()) {
                int8_t led_ = doc["set"]["led"];
                if (dig_pins.find("whiteLed") != dig_pins.end()) {
                    dig_pins.find("whiteLed")->second.PWM_mode((int)(led_ * 255 / 100));
//                    dig_pins.find("whiteLed")->second.
                }

            }
            if (!doc["set"]["reb_blue_led"].isNull()) {
                int8_t led_ = doc["set"]["reb_blue_led"];
                if (dig_pins.find("fitoLed") != dig_pins.end()) {
                    dig_pins.find("fitoLed")->second.PWM_mode((int)(led_ * 255 / 100));
//                    dig_pins.find("fitoLed")->second.edit_status_pin(led_ >= 50, 11, true);
                }
            }
            if (!doc["set"]["humidity"].isNull()) {
//                serializeJson(doc["set"]["humidity"], Serial);;
//                Serial.print("\n");
            }
            if (!doc["set"]["root_humidity"].isNull()) {
//                serializeJson(doc["set"]["root_humidity"], Serial);;
//                Serial.print("\n");
            }
            if (!doc["set"]["temperature"].isNull()) {
//                serializeJson(doc["set"]["temperature"], Serial);;
//                Serial.print("\n");
            }
            if (!doc["set"]["fan"].isNull()) {
                bool fan_aur = doc["set"]["fan"];
                Serial.println("--" +  (String)fan_aur);
                serializeJson(doc["set"]["fan"],Serial);
                Serial.println();
                if (dig_pins.find("fan_air") != dig_pins.end()) {
                    dig_pins.find("fan_air")->second.set_priority(10);
                    dig_pins.find("fan_air")->second.edit_status_pin(fan_aur, 11, false);
                }
            }
            if (!doc["set"]["root_fan"].isNull()) {
                bool fan_root = doc["set"]["root_fan"];
                if (dig_pins.find("fan_root") != dig_pins.end()) {
                    dig_pins.find("fan_air")->second.set_priority(10);
                    dig_pins.find("fan_root")->second.edit_status_pin(fan_root, 11, false);
                }
            }
        }
        if (!doc["get"].isNull()) {

        }
        if (!doc["action"].isNull()) {
            if (!doc["action"]["water"].isNull()) {
                bool water = doc["action"]["water"];
                Serial.println(water);
                if (dig_pins.find("poliv") != dig_pins.end() && water) {
                    Serial.println("полив должен работать");
                    dig_pins.find("poliv")->second.turn_on_for_time(POLIV_DELAY, 11);
                }
            }
        }
//        Serial.println("-----");
//        serializeJson(doc, Serial);
//        Serial.println("\ninto if");
    }


}

String RaspberryPiControl::get_val_sens() {
    String ret = "/";
    for (auto it = sensors_val.begin(); it != sensors_val.end(); ++it) {
        ret += (String) it->first;
        ret += ":";
        ret += (String) it->second.value;
        ret += "/";
    }
    return ret;
}

//=======! отправление данеых !=======
void RaspberryPiControl::regular_send() {
    if (millis() - time_turn_on > 2000) {
        time_turn_on = millis();
        write_raspb(
                String("") +
                "{\"metrics\": {" +
                    "\"temperature\":" + (String) sensors_val["temp"].value + ", " +
                    "\"humidity\": " + (String) sensors_val["hum"].value + ", " +
                    "\"root_humidity\": " + (String) sensors_val["root_hum"].value + ", " +
                    "\"water_level\": " + (String) sensors_val["level_poliv"].value + ", " +
                    "\"water_humidity_level\": " + (String) sensors_val["level_vapor"].value +
                "}}");
    }
}

void RaspberryPiControl::write_raspb(String text) {
    raspb->println(text);
    time_turn_on = millis();
}