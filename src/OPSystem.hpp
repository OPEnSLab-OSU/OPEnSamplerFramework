//
//  OPSystem.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "Arduino.h"
#include "OPActionSequence.hpp"
#include "OPComponent.hpp"

class OPSystem {
public:
    VoidFunction setup;
    VoidFunction loop;
    
    LinkedList<OPComponent *> components;
    // OPActionSequence timer;
    
    // OPSystem(VoidFunction _setup, VoidFunction _loop) 
    // : timer("timer") {
    //     addComponent(&timer);
    //     setup = _setup;
    //     loop = _loop;
    // }

    OPSystem(VoidFunction _setup, VoidFunction _loop) {
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
    app.setup();
    for(int i = 0; i < app.components.size; i++) {
        OPComponent * component = app.components.get(i);
        component->setup();
        Serial.print("[Info] Component has been initialized: ");
        Serial.println(component->name);
    }
}
