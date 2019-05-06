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

#ifdef __arm__
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}


class OPSystem {
private:
    VoidFunction _setup;
    VoidFunction _didSetup;
    VoidFunction _loop;
public:  
    LinkedList<OPComponent *> components;
    // OPTaskScheduler * scheduler;

    OPSystem(VoidFunction _setup, VoidFunction _didSetup, VoidFunction _loop) {
        // scheduler = new OPTaskScheduler();

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

        // scheduler->setup();
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

        // scheduler->loop();
    }
};

extern OPSystem app;

OPComponent * get(String name) {
    return app.get(name);
} 

void addComponent(OPComponent * component) {
    app.addComponent(component);
}

// void run(const OPTask & task) {
//     app.scheduler->append(task);
// }

// void runForever(OPTask & task) {
//     task.repeatTaskFor(-1);
//     app.scheduler->append(task);
// }

// void setTimeout(long ms, VoidFunction callback) {
//     OPTask task;
//     task.wait(ms, callback);
//     run(task);
// }

void setup() {
    app.setup();
}

void loop() {
    app.loop();
}



