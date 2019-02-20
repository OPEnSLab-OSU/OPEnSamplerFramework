//
//  main.cpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#include "OPSystem.hpp"
#include "OPSerialParser.hpp"
#include "LEDComponent.hpp"
#include "ShiftRegisterComponent.hpp"

OPSystem app([] () {
    // Init
    auto serial = new OPSerialParser();
    app.addComponent(serial);

    auto led = new LEDComponent("led", LED_BUILTIN);
    app.addComponent(led);

    auto tpic = new ShiftRegisterComponent("tpic", 32);
    tpic->setPins(11, 13, 10);
    app.addComponent(tpic);
}, [] () {
    // Update
});
