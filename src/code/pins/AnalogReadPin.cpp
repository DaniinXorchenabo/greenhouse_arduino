//
// Created by Daniil on 21.12.2021.
//

#include "AnalogReadPin.h"


AnalogReadPin::AnalogReadPin() {}

AnalogReadPin::AnalogReadPin(int pin_now, bool def_n) {
    pin = pin_now;
    def = def_n;
    pinMode(pin, INPUT);
}

AnalogReadPin::AnalogReadPin(int pin_now) {
    pin = pin_now;
    pinMode(pin, INPUT);
}

AnalogReadPin::AnalogReadPin(DHT &dht_new, String param_dht) {
    Serial.println("===8=0=9==-0=");
    *dht = dht_new;
    if (param_dht == "t") {
        how_param = true;
    } else {
        how_param = false;
    }
    DHT_flag = true;
}

int AnalogReadPin::get_value() {
    if (DHT_flag) {
        if (how_param) {
            return (int) return_temperature();
        } else {
            return (int) return_humid();
        }
    } else {
        return (int) pin_analogRead();
    }
}

int AnalogReadPin::pin_analogRead() {
    if (def) {
        return (int) -1;
    }
    return (int) analogRead(pin);
}

int AnalogReadPin::return_temperature() {
    float data = dht->readTemperature();
    if (!isnan(data)) {
        return (int) data;
    }
    return (int) -1;
}

int AnalogReadPin::return_humid() {
    float data = dht->readHumidity();
    if (!isnan(data)) {
        return (int) data;
    }
    return (int) -1;
}