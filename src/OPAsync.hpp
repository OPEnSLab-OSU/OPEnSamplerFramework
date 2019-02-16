//
//  OPAsync.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "Arduino.h"
#include "LinkedList.hpp"
#include "OPComponent.hpp"


String UnnamedOperation = "unnamed";
using VoidFunction = void (*)();

class OPAsync : public OPComponent {
public:

    struct Operation {
        unsigned long delay;
        unsigned long start;
        VoidFunction callback;
        VoidFunction completion;
        String name;
    };
    
    LinkedList<Operation> operations;
    
    
    OPAsync()
    : OPComponent("timer") {
    }
    
    // Return a reference to the object here because if we return by value we
    // are basically returning the state of that object at a point in time.
    OPAsync& run(int delay, VoidFunction callback, String name = UnnamedOperation) {
        Operation op;
        op.start = millis();
        op.delay = delay;
        op.callback = callback;
        op.name = name;
        operations.append(op);
        return *this;
    }
    
//    // TODO: Implementation
//    OPAsync& run(int delay, VoidFunction callback, VoidFunction completion, std::string name = UnnamedOperation) {
//        return *this;
//    }
    
//    // TODO: Implementation
//    OPAsync& then(VoidFunction callback) { return *this; }
    
    
    bool contains(String name) {
        for (int i = operations.size - 1; i >= 0; i--) {
            Operation op = operations.get(i);
            if (op.name == name) {
                return true;
            }
        }
        
        return false;
    }
    
    void loop() override {
        for (int i = operations.size - 1; i >= 0; i--) {
            Operation op = operations.get(i);
            if (millis() - op.start >= op.delay) {
                op.callback();
                operations.remove(i);
            }
        }
    }
};
