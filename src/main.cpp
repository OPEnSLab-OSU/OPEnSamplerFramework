//
//  main.cpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#include "OPSystem.hpp"
#include "LED.hpp"
#include "Shifty.h"

Shifty tpic;

void app_setup() {
    Serial.begin(9600);

    // LED* led = new LED("LED", LED_BUILTIN);
    // app.addComponent(led);

    // LED* led2 = new LED("LED 2", 12);
    // app.addComponent(led2);

    tpic.setBitCount(8);
    tpic.setPins(11, 13, 10);
    tpic.writeBit(5, HIGH);

    Serial.println("SETUP");
}

void app_loop() {
    
}

OPSystem app(app_setup, app_loop);
