//
// Created by Daniil on 21.12.2021.
//

#include "RaspberryPiControl.h"

#include <code/pins/PinControl.h>
#include <code/consts.h>
#include <code/pins/AnalogReadPin.h>

extern std::map<String, PinControl> dig_pins;
extern std::map<String, sens_val_strucr> sensors_val;

RaspberryPiControl::RaspberryPiControl(SoftwareSerial &ras){
raspb = &ras;
raspb->begin(9600);
write_raspb("I started (uno)");
}

void RaspberryPiControl::raspb_update(){
    over_read_raspb();
    regular_send();
}

//=======! чтение с распберри !=======
void RaspberryPiControl::over_read_raspb(){ // считывается строка, полученная с разбери
    while (raspb->available() > 0){
        String raspb_data = read_raspb();
        if (raspb_data != ""){
            raspb_data.trim();
            data_processing(raspb_data);
        }
    }
}

String RaspberryPiControl::read_raspb(){
    Serial.print("-");

    if (!raspb->isListening()){
        bool useless = raspb->listen();
    }

    if (raspb->available()){
        Serial.println("разбери что-то прислал...");
        String data = "";
        raspb->setTimeout(50);
        data = raspb->readString();
        /*
        while (raspb->available() > 0){
          data += (String)raspb->peek();
          if (raspb->read() == '\n'){break;}
        }*/
        //delay(50);
        Serial.println("raspb_get: " + data);
        return data;
    }
    return "";
}

//=======! Обработка полученных данных !=======
void RaspberryPiControl::data_processing(String str){ // обработка строки
    write_raspb("I get: " + str);
    Serial.println("Получил от разбери: " + str);
    str_raspb = str;
    if (str == "WhiteLedHIGH"){
        if (dig_pins.find("whiteLed") != dig_pins.end()){
            dig_pins.find("whiteLed")->second.edit_status_pin(true, 11, true);
        }
    }
    else if (str == "WhiteLedLOW"){
        if (dig_pins.find("whiteLed") != dig_pins.end()){
            dig_pins.find("whiteLed")->second.edit_status_pin(false, 11, true);
        }
    }
    else if (str == "fitoLedHIGH"){
        if (dig_pins.find("fitoLed") != dig_pins.end()){
            dig_pins.find("fitoLed")->second.edit_status_pin(true, 11, true);
        }
    }
    else if (str == "fitoLedLOW"){
        if (dig_pins.find("fitoLed") != dig_pins.end()){
            dig_pins.find("fitoLed")->second.edit_status_pin(false, 11, true);
        }
    }
    else if (str == "polivHIGH"){
        if (dig_pins.find("poliv") != dig_pins.end()){
            dig_pins.find("poliv")->second.turn_on_for_time(POLIV_DELAY, 11);
        }
    }
    else if (str == "get_parametrs"){write_raspb(get_val_sens());}
    else if (str == "Give_start_day"){write_raspb("start_day " + start_day);}

    if (str_raspb != str){
    }
}

String RaspberryPiControl::get_val_sens(){
    String ret = "/";
    for(auto it = sensors_val.begin(); it != sensors_val.end(); ++it){
        ret += (String)it->first;
        ret += ":";
        ret += (String)it->second.value;
        ret += "/";
    }
    return ret;
}

//=======! отправление данеых !=======
void RaspberryPiControl::regular_send(){
    if (millis() - time_turn_on > 2000){
        time_turn_on = millis();
        write_raspb("something");
    }
}

void RaspberryPiControl::write_raspb(String text){
    raspb->println(text);
    time_turn_on = millis();
}