#pragma once
#include "OPComponent.hpp"

class OPSerialBuffer : public OPComponent {
private:
    String input;
    String currentCommand;

public:
    OPSerialBuffer() : OPComponent("SerialBuffer") {}

    String currentLine() { 
        return currentCommand;
    }

    void clear() { 
        currentCommand = "";
    }

    void setup() override {
        Serial.begin(9600);
        delay(1000);
    }

    void loop() override {
        if (Serial.available() <= 0) {
            return;
        }

        char inputChar = Serial.read();
        if (inputChar == '\n') {
            currentCommand = input;
            input = "";
            return;
        }

        // System characters
        if (inputChar < 32) {
            return;
        }

        input += inputChar;
        Serial.print(inputChar);
    }
};