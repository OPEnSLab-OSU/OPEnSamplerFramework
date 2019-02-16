//
//  LED.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "Arduino.h"
#include "OPComponent.hpp"

class LED: public OPComponent {
public:
    LED(String name, int pin = Unavailable)
    : OPComponent(name, pin) {
        
    }
    
    void setup() override {
        Serial.println("LED Configured");
        pinMode(pin, OUTPUT);
    }
    
    void loop() override {
        delay(100);
        digitalWrite(pin, HIGH);
        delay(100);
        digitalWrite(pin, LOW);
    }
};
