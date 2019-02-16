//
//  LED.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "OPComponent.hpp"


// LED blink demo
class LEDComponent : public OPComponent {
public:
    // LEDComponent(String name, int pin = Unavailable)
    // : OPComponent(name, pin) {
        
    // }

    using OPComponent::OPComponent;
    
    void setup() override {
        Serial.println("LED Configured");
        pinMode(pin, OUTPUT);
    }
    

    // TODO: Async LED demo
    void loop() override {
        delay(300);
        digitalWrite(pin, HIGH);
        delay(100);
        digitalWrite(pin, LOW);
    }
};
