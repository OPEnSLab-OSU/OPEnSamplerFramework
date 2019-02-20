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
class OPLEDComponent : public OPComponent {
public:
    using OPComponent::OPComponent;
    
    void setup() override {
        pinMode(pin, OUTPUT);
    }

    // TODO: Async LED demo
    void loop() override {
        delay(100);
        digitalWrite(pin, HIGH);
        delay(100);
        digitalWrite(pin, LOW);
    }
};
