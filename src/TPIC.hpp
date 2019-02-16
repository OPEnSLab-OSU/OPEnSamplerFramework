//
//  TPIC.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once
#include "OPComponent.hpp"

class TPIC : public OPComponent {
   private:
    int size = 8;
    char outputs[];

   public:
    TPIC(int size) 
    : size(size), outputs[size / 8], OPComponent() {

    }
    bool set(char nth, bool flag) {
        if (nth > size - 1) {
            return false;
        }

        char mask = (1 << nth);
        if (flag) {
            outputs = (bits & ~mask) + mask;
        } else {
            outputs = bits & ~mask;
        }

        return true;
    }

    void flush() {
        for (int i = 0; i < size; i++) {
        }
    }

    void setup() override {}

    void loop() override {}
}