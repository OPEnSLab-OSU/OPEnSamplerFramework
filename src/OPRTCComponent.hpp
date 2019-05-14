#pragma once
#include "OPComponent.hpp"
#include "LowPower.h"
#include "RTClibExtended.h"
#include "Wire.h"

#define RTC_INTERRUPT_PIN 3

// Routine for RTC interrupt calls
void wakeup() {
    if (millis() - rtcInterruptStart < 500) {
        return;
    }

    detachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN));
    alarmTriggered = true;
    rtcInterruptStart = millis();
}

class OPRTCComponent: public OPComponent {
public:
    RTC_DS3231 rtc;

    using OPComponent::OPComponent;

    void setup() override {
        // Prepare I2C communication
        rtc.begin();

        // Check if RTC is connected
        while (true) {
            Wire.beginTransmission(DS3231_ADDRESS);
            Wire.write(1);
            Wire.endTransmission();
            Wire.requestFrom(DS3231_ADDRESS, 1);

			// This means that all bits are high. I2C for DS3231 is active low.
            if (Wire.read() != 0xFF) {
                Serial.println(F("\n-= RTC Connected =-"));
                break;
            } else {
                Serial.println(F("\n-= RTC Not Connected =-"));
                delay(1000);
            }
        }

        // Update the time on the RTC to the complied time
        DateTime computerTime = DateTime(F(__DATE__), F(__TIME__));
        rtc.adjust(computerTime);
        rtc.writeSqwPinMode(DS3231_OFF);

        // Wait for I2C to finish configuring the RTC DS3231
        delay(2000); 

        pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
        disarmAlarms();
        displayCurrentTime();

        attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), wakeup, FALLING);
    }

    //----------------------------------------------------------------------------------
    // Clear internal alarm-trigger flags of the library and disable any prev alarms    
    //----------------------------------------------------------------------------------
    void disarmAlarms() {
        rtc.armAlarm(1, false);
        rtc.clearAlarm(1);
        rtc.alarmInterrupt(1, false);
        rtc.armAlarm(2, false);
        rtc.clearAlarm(2);
        rtc.alarmInterrupt(2, false);
    }


    //------------------------------------------------------------------------------------
    // Clear internal alarm-trigger flags of the library to re-enable alarm interrupts 
    //------------------------------------------------------------------------------------
    void resetAlarms() {
        rtc.clearAlarm(1);
        rtc.clearAlarm(2);
    }

    //------------------------------------------------------------------------------------
    // Checks if the alarm has been scheduled 
    //------------------------------------------------------------------------------------
    bool isAlarmScheduled() {
        return rtc.isArmed(1);
    }

    //------------------------------------------------------------------------------------
    // Serial print the current time
    //------------------------------------------------------------------------------------
    void displayCurrentTime() {
        Serial.print(F("Current Time: "));
        printTime(rtc.now());
    }

    //------------------------------------------------------------------------------------
    // Serial print the current time
    //------------------------------------------------------------------------------------
    void printTime(DateTime const &time) {
        Serial.print(time.hour());
        Serial.print(F("H "));
        Serial.print(time.minute());
        Serial.print(F("M "));
        Serial.print(time.second());
        Serial.println(F("S"));
    }

    //------------------------------------------------------------------------------------
    // Bring the chip to the low power mode. User interrupt to return to the regular 
    // operation
    //------------------------------------------------------------------------------------
    void sleepForever() {
        Serial.println();
        Serial.println(F("Going to sleep..."));
        for (int i = 3; i > 0; i--){
            Serial.print(F("-> "));
            Serial.println(i);
            delay(333);
        }

        attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), wakeup, FALLING);
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 


        Serial.println();
        Serial.println(F("Just woke up!"));
        displayCurrentTime();
    }

    void sleepFor(int seconds) {
        TimeSpan span(seconds);
        DateTime future(rtc.now() + span);
        Serial.println(F("Sleep Interval"));

        detachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN));
        disarmAlarms();
        rtc.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0);
        rtc.alarmInterrupt(1, true);
        sleepForever();
    }

    void scheduleNextAlarmForCurrentConfig() {
        scheduleNextAlarmForConfig(Config::current());
    }

    void scheduleNextAlarmForConfig(Config & c) {
        if (c.isAlarmInteval) {
            TimeSpan span(c.sampleAlarmDay, c.sampleAlarmHr, c.sampleAlarmMin, 0);
            DateTime future(rtc.now() + span);

            disarmAlarms();
            rtc.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0);
            rtc.alarmInterrupt(1, true);
            attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), wakeup, FALLING);
            
            Serial.println();
            Serial.print(F("New Alarm Scheduled to: "));
            printTime(future);
        } 
    }
};