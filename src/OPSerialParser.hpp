#pragma once
#include "OPComponent.hpp"

class OPSerialParser : public OPComponent {
private:
    String input;
    String lastCommand;

public:
    OPSerialParser() : OPComponent("serial") {}

    String getLastCommand() { 
        return lastCommand;
    }

    void clearLastCommand() { 
        lastCommand = "";
    }

    void setup() override {
        Serial.begin(9600);
        delay(100);
    }

    void loop() override {
        if (Serial.available() <= 0) {
            return;
        }

        char inputChar = Serial.read();
        if (inputChar == '\n') {
            lastCommand = input;
            input = "";
            return;
        }

        if (inputChar < 32) {
            return;
        }

        input += inputChar;
        Serial.print(inputChar);
    }
};