//
//  OPSystem.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "Arduino.h"
#include "OPActionSequenceScheduler.hpp"
#include "OPComponent.hpp"

class OPSystem {
public:
    VoidFunction setup;
    VoidFunction loop;
    
    LinkedList<OPComponent*> components;
    OPActionSequenceScheduler scheduler;

    OPSystem(VoidFunction _setup, VoidFunction _loop)
    :
    scheduler("scheduler") {
        // Serial.println("Serial Begin");
        // delay(100);
        addComponent(&scheduler);
        setup = _setup;
        loop = _loop;
    }
    
    void addComponent(OPComponent * component) {
        components.append(component);
    }

    OPComponent * getComponentByName(String name) {
        for (int i = 0; i < components.size; i++) {
            OPComponent * component = components.get(i);
            if (component->name == name) {
                return component;
            }
        }

        return nullptr;
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
    Serial.begin(9600);
    app.setup();
    for(int i = 0; i < app.components.size; i++) {
        OPComponent * component = app.components.get(i);
        component->setup();
        Serial.print("[Info] Component setup invoked: ");
        Serial.println(component->name);
    }
}
