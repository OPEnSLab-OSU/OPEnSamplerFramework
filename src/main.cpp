//
//  main.cpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#include "OPSystem.hpp"
#include "OPShiftRegisterComponent.hpp"

void initialize() {
    // Arduino: 11, 13, 10
    // Feather: 23, 24, 11

    auto shift = new OPShiftRegisterComponent("ShiftRegister", 32);
    shift->setPins(11,13,10);
    app.addComponent(shift);
    
    OPTask blinkAll;
    blinkAll.delay(1000, []() {
        auto shift = (OPShiftRegisterComponent &) app.get("ShiftRegister");
        shift.setOutputOne();
        shift.registerFlush();
    }).delay(1000, []() {
        auto shift = (OPShiftRegisterComponent &) app.get("ShiftRegister");
        shift.setOutputZero();
        shift.registerFlush();
    });

    OPTask blink15;
    blink15.delay(100, []() {
        auto shift = (OPShiftRegisterComponent &) app.get("ShiftRegister");
        shift.registerWrite(15, HIGH);
    }).delay(100, []() {
        auto shift = (OPShiftRegisterComponent &) app.get("ShiftRegister");
        shift.registerWrite(15, LOW);
    });

    OPTaskSequence seq;
    seq.then(blinkAll).repeat(3).then(blink15).repeat(5);
    run(seq);    
}

void update() {

}

OPSystem app(initialize, update);