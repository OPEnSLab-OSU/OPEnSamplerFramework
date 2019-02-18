#pragma once
#include "OPComponent.hpp"
#include "LinkedList.hpp"

class OPComponentArray : OPComponent {

    private:
    LinkedList<OPComponent *> components;
    public:
    void setup() override {
        
    }

    void loop() override {

    }
};