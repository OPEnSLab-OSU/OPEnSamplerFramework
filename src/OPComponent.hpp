//
//  OPComponent.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "Arduino.h"

int Unavailable = -1;

class OPComponent {
public:
    String name = "unnamed";
    int pin = Unavailable;
    
    OPComponent() {}
    
    OPComponent(String name, int pin = Unavailable) :
    name(name), pin(pin) {
        
    }
    
    virtual void setup() {
        Serial.println("Component Setup");
    }
    
    virtual void loop() {
    }
};
