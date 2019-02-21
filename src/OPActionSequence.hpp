//
//  OPAsync.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "LinkedList.hpp"
#include "OPComponent.hpp"

String UnnamedOperation = "unnamed";
using VoidFunction = void (*)();

struct OPActionNode {
    unsigned long start, delay;
    bool expired = false;
    String name;
    VoidFunction callback;
    OPActionNode * next = nullptr;
};

class OPActionSequence : public OPComponent {
private:
    OPActionNode * head = nullptr;
    bool activate = false;

    friend class OPActionSequenceTimer;
public:
    int size = 0;
    
    // Copy constructor
    OPActionSequence(const OPActionSequence &obj) {
        auto source = obj.head;
        if (source) {
            head = new OPActionNode();
            head->start = source->start;
            head->delay = source->delay;
            head->expired = source->expired;
            head->callback = source->callback;
        }

        auto current = head;
        while(source->next) {
            source = source->next;
            
            auto copy = new OPActionNode();
            copy->start = source->start;
            copy->delay = source->delay;
            copy->expired = source->expired;
            copy->callback = source->callback;
            current->next = copy;
            current = current->next;
        }
        size = obj.size;
        name = obj.name;
        activate = obj.activate;
    }
    
    ~OPActionSequence() {
        OPActionNode * currentPtr = head;
        while (currentPtr) {
            OPActionNode * next = currentPtr;
            currentPtr = next->next;
            delete next;
        }
        head = nullptr;
    }
    
    // Using new keyword to allocate memory in the heap.
    // Without using new keyword, the object will be destroyed when it goes out
    // of scope.
    OPActionSequence & delay(unsigned long delay, VoidFunction callback) {
        auto action = new OPActionNode();
        action->delay = delay;
        action->callback = callback;
        append(action);
        return * this;
    }
    
    OPActionSequence & completion(VoidFunction callback) {
        auto action = new OPActionNode();
        action->callback = callback;
        append(action);
        return * this;
    }
    
    void append(OPActionNode * action) {
        if (head) {
            OPActionNode * currentPtr = head;
            while (currentPtr->next) {
                currentPtr = currentPtr->next;
            }
            
            currentPtr->next = action;
        } else {
            head = action;
        }
        
        size = size + 1;
    }
    
    bool remove(int index) {
        if (index < 0 || index > size - 1) {
            return false;
        }
        
        int i = 0;
        OPActionNode * prevPtr = nullptr;
        OPActionNode * currentPtr = head;
        while (i != index) {
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
            i++;
        }
        
        if (prevPtr) {
            prevPtr->next = currentPtr->next;
        } else {
            head = head->next;
        }
        
        size--;
        delete currentPtr;
        return true;
    }

    bool isCompleted() {
        return head == nullptr;
    }
    
    void run() {
        activate = true;
    }

    void loop() override {
        if (!activate || isCompleted) {
            return;
        }

        if (head->start == 0){
            head->start = millis();
        }

        if (millis() - head->start < head->delay) {
            return;
        }
        
        head->callback();
        remove(0);
    }
};

