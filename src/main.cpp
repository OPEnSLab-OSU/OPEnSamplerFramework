//
//  main.cpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#include "OPSystem.hpp"
#include "LEDComponent.hpp"
#include "ShiftRegisterComponent.hpp"
// #include "Shifty.h"

void app_setup() {
    Serial.begin(9600);

    // auto led = new LEDComponent("LED", LED_BUILTIN);
    // app.addComponent(led);

    // LED* led2 = new LED("LED 2", 12);
    // app.addComponent(led2);

    auto regist = new ShiftRegisterComponent("TPIC", 16);
    regist->setPins(11, 13, 10);
    app.addComponent(regist);
    Serial.println("SETUP");
}

void app_loop() {
    
}

OPSystem app(app_setup, app_loop);
