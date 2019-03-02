//
//  OPAsync.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "LinkedList.hpp"


String UnnamedOperation = "unnamed";
using VoidFunction = void (*)();

struct OPAction {
    unsigned long start = 0;
    unsigned long delay;

    bool expired = false;

    VoidFunction callback;
    
    void activate() {
        start = millis();
    }
    
    void update() {
        
    }
};

class OPTask : public LinkedList <OPAction> {
public:
    bool activated = false;
    int repeat = 1;
    long delayTask = 0;
    int currentActionIndex = 0;
private:
    friend class OPTaskSequence;
    friend class OPSystem;
    void run() {
        Serial.println("RUN");
        activated = true;
    }
public:
    OPTask() : LinkedList() {
        
    }
    
    OPTask(const OPTask &obj) : LinkedList(obj) {
        repeat = obj.repeat;
        activated = obj.activated;
    }
    
    OPTask & delay(unsigned long delay, VoidFunction callback) {
     OPAction action;
        action.delay = delay;
        action.callback = callback;
        
        append(action);
        return * this;
    }

    OPTask & execute(VoidFunction callback) {
        return delay(0, callback);
    }
    
    OPTask & completion(VoidFunction callback) {
     OPAction action;
        action.callback = callback;
        OPTask next;
        next.append(action);
        return delay(0, callback);
    }

    OPTask & repeatFor(int times) {
        repeat = times;
        return *this;
    }

    void loop() {
        if (!activated || isEmpty()) {
            return;
        }

        if (repeat == 1) {
         OPAction & action = head->data;

            if (action.start == 0) {
                action.activate();
            }

            if (millis() - action.start >= action.delay) {
                action.callback();
                remove(0);
            }
            return;
        } 

        if (repeat == 0) {
            return;
        }

        auto current = search(currentActionIndex);
        OPAction & action = current->data;
        if (action.start == 0) {
            action.activate();
        }

        if (millis() - action.start >= action.delay) {
            action.callback();
            action.start = 0;

            if (current == tail) {
                currentActionIndex = 0;
                repeat = max(repeat - 1, -1);
            } else {
                currentActionIndex++;
            }

            Serial.println(currentActionIndex);
        }
    }
};

