//
//  OPSystem.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "OPComponent.hpp"
#include "OPActionSequenceScheduler.hpp"

class OPSystem {
private:
    VoidFunction _setup;
    VoidFunction _loop;
public:  
    LinkedList<OPComponent*> components;
    OPActionSequenceScheduler scheduler;

    OPSystem(VoidFunction _setup, VoidFunction _loop)
    :
    scheduler("scheduler") {
        addComponent(&scheduler);
        this->_setup = _setup;
        this->_loop = _loop;
    }

    void run(OPActionSequence const & action) {
        scheduler.schedule(action);
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

    void setup() {
        Serial.begin(9600);
        _setup();
        for(int i = 0; i < components.size; i++) {
            OPComponent * component = components.get(i);
            component->setup();
            Serial.print("[Info] Component setup invoked: ");
            Serial.println(component->name);
        }
    }

    void loop() {
        _loop();
        for(int i = 0; i < components.size; i++) {
            components.get(i)->loop();
        }
    }
};

extern OPSystem app;

void setup() {
    app.setup();
}

void loop() {
    app.loop();
}

void run(OPActionSequence & seq) {
    app.run(seq);
}
