//
//  OPSystem.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "Arduino.h"
#include "OPAsync.hpp"
#include "OPComponent.hpp"

class OPSystem {
private:
public:
    int pin;
    VoidFunction setup;
    VoidFunction loop;
    
    LinkedList<OPComponent *> components;
    OPAsync timer;
    
    OPSystem(VoidFunction _setup, VoidFunction _loop) 
    : timer("timer") {
        addComponent(&timer);
        
        setup = _setup;
        loop = _loop;
    }
    
    void addComponent(OPComponent * component) {
        components.append(component);
    }
};

extern OPSystem app;

void loop() {
    app.loop();
    for(int i = 0; i < app.components.size; i++) {
        app.components.get(i)->loop();
    }
}

void setup() {
    app.setup();
    for(int i = 0; i < app.components.size; i++) {
        OPComponent * component = app.components.get(i);
        Serial.print("[Info] setting up component named: ");
        Serial.println(component->name);
        component->setup();
    }
}
