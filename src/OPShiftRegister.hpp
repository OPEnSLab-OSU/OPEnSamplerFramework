//
//  OPShiftRegister.cpp
//  OpenSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "OPComponent.hpp"
#include "OPSerialBuffer.hpp"

class OPShiftRegister: public OPComponent {
public:
    byte * outputs;
    byte size;
    byte registersCount;

    int dataPin;
    int clockPin;
    int latchPin;
public:
    OPShiftRegister(String name, byte size) 
    : OPComponent(name), size(size) {
        registersCount = size / 8;
        outputs = new byte[registersCount];
    }

    OPShiftRegister(String name, byte size, int data, int clock, int latch) 
    : OPShiftRegister(name, size) {
        setPins(data, clock, latch);
    }

    void setPins(int data, int clock, int latch) {
        dataPin = data;
        clockPin = clock;
        latchPin = latch;

        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(latchPin, OUTPUT);
        registerReset();
    }

    void setOutputZero() {
        for (int i = 0; i < registersCount; i++){
            outputs[i] = 0;
        }
    }

    void setOutputOne() {
        for (int i = 0; i < registersCount; i++){
            outputs[i] = 255;
        }
    }

    /*
        Set output array to zero and flush
    */
    void registerReset() {
        setOutputZero();
        registerFlush();
    }

    /*
        Write data from the output array to shift registers
     */ 
    void registerFlush() {
        digitalWrite(latchPin, LOW);
        for (int i = registersCount - 1; i >= 0; i--){
            shiftOut(dataPin, clockPin, MSBFIRST, outputs[i]);
        }
        digitalWrite(latchPin, HIGH);
    }

    /*
        Set data in the output array according to the bit and call registerFlush()
    */
    bool registerWrite(byte bitToSet, bool signal) {
        if (bitToSet / 8 >= registersCount) {
            return false;
        }
        
        setOutputZero();

        byte output = 0b00000000;
        bitWrite(output, bitToSet % 8, signal);

        byte index = bitToSet / 8;
        outputs[index] = output;
        registerFlush();

        Serial.print(F("Register Index: "));
        Serial.println(index);
        Serial.print(F("Register Write: "));
        Serial.println(outputs[index]);
        return true;
    }
};