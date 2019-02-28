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
    //Arduino: 11, 13, 10
    //Feather: 23, 24, 11
    auto shiftRegister = new OPShiftRegisterComponent("ShiftRegister", 32);
    shiftRegister->setPins(23, 24, 11);
    // shiftRegister->setPins(11, 13, 10);
    app.addComponent(shiftRegister);

    OPActionSequence sh("Shift");
    sh.delay(3000, []() {
        auto component = (OPShiftRegisterComponent &) app.get("ShiftRegister");
        for (int i = 0; i < component.bitsize; i++){
            component.registerWrite(i, HIGH);
        }
    }).delay(3000, []() {
        auto component = (OPShiftRegisterComponent &) app.get("ShiftRegister");
        for (int i = component.bitsize - 1; i >= 0; i--){
            component.registerWrite(i, HIGH);
        }
    });
    sh.repeat = -1;

    run(sh);
}

void update() {

}

OPSystem app(initialize, update);