#include "OPComponent.hpp"
#include "RTClib.h"

RTC_DS3231 rtc;

class OPRTCComponent: public OPComponent {

    public:
        using OPComponent::OPComponent;

    void setup() override {

        // Code from adafruit 2
        if (!rtc.begin()) {
            Serial.println("Couldn't find RTC");
            while (true);
        }

        if (rtc.lostPower()) {
            Serial.println("RTC lost power, lets set the time!");
            // following line sets the RTC to the date & time this sketch was compiled
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            // This line sets the RTC with an explicit date & time, for example to set
            // January 21, 2014 at 3am you would call:
            // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        }

    }

    void loop() override {

    }

}