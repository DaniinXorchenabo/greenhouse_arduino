//
// Created by Daniil on 21.12.2021.
//

#include "EspControl.h"
#include <code/pins/PinControl.h>
#include <code/consts.h>
#include <code/pins/AnalogReadPin.h>


extern std::map<String, PinControl> dig_pins;
extern std::map<String, sens_val_strucr> sensors_val;


EspControl::EspControl(){
    Serial3.begin(9600); //115200
    Serial3.flush();
}

void EspControl::update_esp(){
    regular_send(); //отправка данных на esp
    over_read(); //обработка данных, полученных от ESP
}

//=======! отправка данных на esp !=======
void EspControl::regular_send(){
    if (millis() - time_turn_on > 2000){
        time_turn_on = millis();
        esp_write(generate_answer());
    }
}

String EspControl::generate_answer(){
    String answer;
    if (dig_pins.find("fitoLed") != dig_pins.end()){
        answer = "/" + (String)dig_pins.find("fitoLed")->second.get_filling_pin();
        answer = answer + answer + "/";
    } else {
        answer = "/no_date/no_date/";
    }
    answer = "/" + (String)(int)return_sensor_val("temp") + "/" + (String)(int)return_sensor_val("hum") + answer;
    return answer + "3/";
}

int EspControl::return_sensor_val(String key){
    if (sensors_val.find(key) != sensors_val.end()){
        return (int)sensors_val.find(key)->second.value;
    }
    return 0;
}

void EspControl::esp_write(String send_data){
    Serial.println("send to esp: " + send_data);
    Serial3.println(send_data);
}

//=======! прием данных с ESP !=======

void EspControl::over_read(){
    String new_str = esp_read();
    Serial.println("-=-=-="+new_str);
    if (new_str != ""){
        new_str += "---";
        new_str  = new_str.substring(0, 1);
        Serial.println("esp get^ " + new_str);
        data_processing(new_str);
    }
}

void EspControl::data_processing(String priem_c_ESP_str){

    if (priem_c_ESP_str == "3") {//свет включить
        if (dig_pins.find("fitoLed") != dig_pins.end()){
            dig_pins.find("fitoLed")->second.edit_status_pin(true, 31, true);
        }
    }
    if (priem_c_ESP_str == "2") {//свет выключить
        if (dig_pins.find("fitoLed") != dig_pins.end()){
            dig_pins.find("fitoLed")->second.edit_status_pin(false, 31, true);
        }
    }
    if (priem_c_ESP_str == "7") {/*digitalWrite(40,HIGH);   reset_esp = 1;*/} //resrt
    if (priem_c_ESP_str == "4") {  //полив
        if (dig_pins.find("poliv") != dig_pins.end()){
            dig_pins.find("poliv")->second.turn_on_for_time(POLIV_DELAY, 31);
        }
    }
    if (priem_c_ESP_str == "5") {} //включить авто
    if ((priem_c_ESP_str == "6")) {reset_priority();} //выключить авто
}

void EspControl::reset_priority(){
    for(auto it = dig_pins.begin(); it != dig_pins.end(); ++it){
        it->second.set_priority(30);
    }
}

String EspControl::esp_read(){
    if (Serial3.available() > 0){
        Serial3.setTimeout(50);
        String data = (String)Serial3.readString();
        delay(5);
        Serial3.flush();
        return data;
    }
    return "";
}