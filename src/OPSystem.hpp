//
//  OPSystem.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//


#define Unnamed "Unnamed"
#define UnavaiablePin -1
#define IsWritten 0xFF

using VoidFunction = void (*)();
using BoolFunction = bool (*)();

#pragma once
#include "OPComponent.hpp"
#include "LinkedList.hpp"
// #include "OPTaskScheduler.hpp"

class OPSystem {
private:
    VoidFunction _setup;
    VoidFunction _didSetup;
    VoidFunction _loop;
public:  
    LinkedList<OPComponent *> components;

    OPSystem(VoidFunction _setup, VoidFunction _didSetup, VoidFunction _loop) {
        this->_setup = _setup;
        this->_didSetup = _didSetup;
        this->_loop = _loop;
    }

    void addComponent(OPComponent * component) {
        components.append(component);
    }

    OPComponent * get(String name) {
        auto current = components.head;
        while (current) {
            auto component = *current->data;
            if (component.name == name) {
                break;
            } 
            
            current = current->next; 
        }

        return current->data;
    }

    void setup() {
        _setup();
        auto current = components.head;
        while (current) {
            auto component = current->data;
            component->setup();
            current = current->next;
        }

        _didSetup();
    }

    void loop() {
        _loop();
        auto current = components.head;
        while (current) {
            auto component = current->data;
            if (component->enabled) {
                component->loop();
            } 
            current = current->next;
        }

    }
};

extern OPSystem app;

OPComponent * get(String name) {
    return app.get(name);
} 

void addComponent(OPComponent * component) {
    app.addComponent(component);
}

void setup() {
    app.setup();
}

void loop() {
    app.loop();
}



