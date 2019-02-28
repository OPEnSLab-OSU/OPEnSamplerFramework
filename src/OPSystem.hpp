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
    
    OPActionSequenceScheduler scheduler;
public:  
    LinkedList<OPComponent*> components;

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

    void runForever(OPActionSequence const & action) {
        scheduler.scheduleForever(action);
    }
    
    void addComponent(OPComponent * component) {
        components.append(component);
    }

    OPComponent & get(String name) {
        auto current = components.head;
        while (current) {
            auto component = *current->data;
            if (component.name == name) {
                break;
            } 
            
            current = current->next; 
        }

        if (current == nullptr) {
            while(true) {
                Serial.println("Error: accessing null pointer!");
                delay(1000);
            }
        }

        return *current->data;
    }

    void setup() {
        Serial.begin(9600);
        _setup();
        auto current = components.head;
        while (current) {
            auto component = current->data;
            component->setup();
            current = current->next;
        }
    }

    void loop() {
        _loop();
        auto current = components.head;
        while (current) {
            auto component = current->data;
            component->loop();
            current = current->next;
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

void run(OPActionSequence const & seq) {
    app.run(seq);
}

// void run(OPActionSequence req) {

// }
