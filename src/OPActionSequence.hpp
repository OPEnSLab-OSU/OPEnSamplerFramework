//
//  OPAsync.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "OPComponent.hpp"
#include "LinkedList.hpp"

String UnnamedOperation = "unnamed";
using VoidFunction = void (*)();

struct OPAction {
    unsigned long start = 0, delay;
    bool expired = false;
    VoidFunction callback;
    
    void activate() {
        start = millis();
    }
    
    void update() {
        
    }
};

class OPActionSequence : public LinkedList<OPAction>, OPComponent {
public:
    bool activated = false;
    String name = "";
private:
    friend class OPActionSequenceScheduler;
    void run() {
        Serial.println("RUN");
        activated = true;
    }
public:
    OPActionSequence(String name = UnnamedOperation) 
    : 
    LinkedList(),OPComponent(), name(name) {
        
    }
    
    OPActionSequence(const OPActionSequence &obj) : LinkedList(obj) {
        name = obj.name;
        activated = obj.activated;
    }
    
    OPActionSequence & delay(unsigned long delay, VoidFunction callback) {
        OPAction action;
        action.delay = delay;
        action.callback = callback;
        
        append(action);
        return * this;
    }
    
    OPActionSequence & completion(VoidFunction callback) {
        return delay(0, callback);
    }
    
    OPActionSequence then(OPActionSequence const & other) {
        OPActionSequence copy(*this);
        copy.extend(other);
        return copy;
    }

    void setup() override {

    }

    void loop() override {
        if (!activated || isEmpty()) {
            return;
        }

        OPAction & action = head->data;

        if (action.start == 0){
            action.start = millis();
        }

        if (millis() - action.start < action.delay) {
            return;
        }
        
        action.callback();
        remove(0);
    }
};

