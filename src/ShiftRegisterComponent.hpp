//
//  OPShiftRegister.cpp
//  OpenSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "OPComponent.hpp"


class ShiftRegisterComponent: public OPComponent {
private:
    byte * registers;
    byte bitsize;
    byte count;

    int dataPin;
    int clockPin;
    int latchPin;
public:
    ShiftRegisterComponent(String name, byte bitsize) 
    : OPComponent(name), bitsize(bitsize) {
        count = bitsize / 8;
        registers = new byte[count];
        for(int i = 0; i < count; i++){
            registers[i] = 0;
        }
    }

    void setPins(int data, int clock, int latch) {
        dataPin = data;
        clockPin = clock;
        latchPin = latch;

        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(latchPin, OUTPUT);
    }

    bool registerWrite(byte bitToSet, bool signal) {
        if (bitToSet / 8 >= count) {
            return false;
        }

        // Clear the output array
        for (int i = 0; i < count; i++){
            registers[i] = 0;
        }

        // Turn the latch pin off to disable unwanted output
        digitalWrite(latchPin, LOW);

        // Set the output byte
        byte output = 0x00000000;
        bitWrite(output, bitToSet % 8, signal);

        // Set the output byte according to register index
        byte index = bitToSet / 8;
        registers[index] = output;

        // Shftout the output array
        for (int i = count - 1; i >= 0; i--){
            shiftOut(dataPin, clockPin, MSBFIRST, registers[i]);
        }

        // Turn the latch pin on to apply the output
        digitalWrite(latchPin, HIGH);

        Serial.print("Index: ");
        Serial.println(index);
        Serial.print("Register Write: ");
        Serial.println(registers[index]);
        return true;
    }

    String input = "";
    void loop() override {
        if (!Serial.available()) {
            return;
        }

        char inputChar = Serial.read();
        if (inputChar == 'n') {
            for(int i = 0; i < bitsize; i++){
                registerWrite(i, HIGH);
            }
        }

        if (input != "" && (inputChar == 10 || inputChar == 13)) {
            Serial.read();
            Serial.println();
            Serial.println(input);
            registerWrite(input.toInt(), HIGH);
            input = "";
        } else if (inputChar != 10 || inputChar != 13) {
            Serial.print(inputChar);
            input += inputChar;
        }
    }
};