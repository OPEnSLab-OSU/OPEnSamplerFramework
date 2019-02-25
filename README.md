# OPEnSamplerFramework ![Code Size](https://img.shields.io/github/languages/code-size/OPEnSLab-OSU/OPEnSamplerFramework.svg?branch=master) ![Language](https://img.shields.io/badge/language-c%2B%2B-red.svg)


Lightweight modularized Arduino framework for the next generation OPEnSampler
## Composing Asynchronous Actions
```c++
#include "OPSystem.hpp"


// LED blinking demo
void initialize() {
    pinMode(LED_BUILTIN, OUTPUT);

     OPActionSequence bl("blink");
    bl.delay(100, []() {
        digitalWrite(LED_BUILTIN, HIGH);
    }).delay(500, []() {
        digitalWrite(LED_BUILTIN, LOW);
    });
    bl.repeat = -1; // Repeat forever

    run(bl);
}

void update() {

}

OPSystem app(initialize, update);
```
