//
//  OPShiftRegister.cpp
//  OpenSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "OPComponent.hpp"
#include "OPSerialParser.hpp"

class OPShiftRegisterComponent: public OPComponent {
private:
    byte * outputs;
    byte bitsize;
    byte count;

    int dataPin;
    int clockPin;
    int latchPin;
public:
    OPShiftRegisterComponent(String name, byte bitsize) 
    : OPComponent(name), bitsize(bitsize) {
        count = bitsize / 8;
        outputs = new byte[count];
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
        for (int i = 0; i < count; i++){
            outputs[i] = 0;
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
        for (int i = count - 1; i >= 0; i--){
            shiftOut(dataPin, clockPin, MSBFIRST, outputs[i]);
        }
        digitalWrite(latchPin, HIGH);
    }

    /*
        Set data in the output array according to the bit and call registerFlush()
    */
    bool registerWrite(byte bitToSet, bool signal) {
        if (bitToSet / 8 >= count) {
            return false;
        }
        
        setOutputZero();

        byte output = 0b00000000;
        bitWrite(output, bitToSet % 8, signal);

        byte index = bitToSet / 8;
        outputs[index] = output;
        registerFlush();

        Serial.print("Index: ");
        Serial.println(index);
        Serial.print("Register Write: ");
        Serial.println(outputs[index]);
        return true;
    }

    void setup() override {
        
    }

    void loop() override {
        auto serialParser = (OPSerialParser *) app.getComponentByName("serial");
        if (serialParser->currentLine() == "n") {
            for (int i = 0; i < bitsize; i++) {
                registerWrite(i, HIGH);
            }

            registerReset();
            serialParser->clearLine();
        }
    }
};