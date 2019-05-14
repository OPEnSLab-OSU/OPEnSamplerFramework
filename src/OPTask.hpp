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

using VoidFunction = void (*)();
using BoolFunction = bool (*)();

struct OPAction {
    unsigned long start = 0;
    unsigned long delay;

    int repeat = 1;
    bool readyToRun = false;

    VoidFunction callback = nullptr;
    BoolFunction condition = nullptr;
};

class OPTask : public LinkedList<OPAction> {
private:
    int repeat = 1;
    int currentActionIndex = 0;
    int actionRepeatCounter = 1;

    friend class OPTaskScheduler;
    friend class OPSystem;

    void run() {
        Serial.print(F("Running Task..."));
    }

public:
    OPTask() : LinkedList() {}

    OPTask(const OPTask &obj) : LinkedList(obj) {
        repeat = obj.repeat;
        currentActionIndex = obj.currentActionIndex;
    }

    /**
     * Wait for specified time in ms before callback is executed
     **/
    OPTask & wait(unsigned long ms, VoidFunction callback) {
        OPAction action;
        action.delay = ms;
        action.callback = callback;
        append(action);
        return * this;
    }

    /**
     * Execute the callback immediately in the pipeline
     **/
    OPTask & now(VoidFunction callback) {
        OPAction action;
        action.delay = 0;
        action.callback = callback;
        append(action);
        return * this;
    }

    OPTask & completion(VoidFunction callback) {
        return now(callback);
    }

    OPTask & condition(BoolFunction condition) {
        OPAction action;
        action.condition = condition;
        append(action);
        return * this;
    }

    /**
     * How many times the repeat the current action for
     **/
    OPTask & repeatFor(int times) {
        OPAction & current = tail->data;
        current.repeat = times;
        return * this;
    }

    void repeatTaskFor(int times) {
        repeat = times;
    }


    void cycleAction() {
        if (currentActionIndex >= size - 1) {
            // Serial.println(F("Cycled"));
            repeat = max(-1, repeat - 1);
            currentActionIndex = 0;
        } else {
            currentActionIndex++;
        }

        actionRepeatCounter = 1;
    }

    void loop() {
        if (repeat == 0 || isEmpty()) {
            Serial.println(F("Empty or Repeat == 0"));
            return;
        }

        if (currentActionIndex >= size) {
            Serial.println(F("Action Index >= size"));
            return;
        }

        auto current = search(currentActionIndex);
        OPAction & action = current->data;

        Serial.print(F("Current Action Index: "));
        Serial.println(currentActionIndex);

        if (action.condition != nullptr) {
            Serial.println(F("Condition Not Empty"));
            if (action.condition()) {
                Serial.println(F("Condition Passed"));
                cycleAction();
            }

            return;
        }

        if (action.readyToRun == false) {
            action.readyToRun = true;
            action.start = millis();
        }

        if (millis() + action.start >= action.delay) {
            action.readyToRun = false;
            action.callback();
        }

        if (actionRepeatCounter >= action.repeat) {
            cycleAction();
        } else {
            actionRepeatCounter++;
        }
    }
};