//
//  main.cpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

volatile bool programmingMode = false;
volatile bool interruptTriggered = true;
volatile bool alarmTriggered = false;
volatile unsigned long intetrruptStart = 0;
volatile unsigned long rtcInterruptStart = 0;

#define PROGRAMMING_ISR_PIN 2
#define FLUSH_VALVE 0

#include "OPSystem.hpp"
#include "OPSerialBuffer.hpp" 
#include "OPEEPROM.hpp"
#include "OPRTCComponent.hpp"
#include "OPShiftRegister.hpp"
#include "OPPump.hpp"

OPSerialBuffer serialBuffer("SerialBuffer");
OPShiftRegister shift("TPIC Registers of the Sampler", 32); // 32 bits, dataPin=11, clockPin=13, latchPin=10
OPRTCComponent rtc("RTC DS3221");
OPPump pump("Pump", 8, 9); 
OPEEPROM eeprom("EEPROM");

void userSerialInput(String &rawInput) {
    // Calculate the start of the actual content
    int start = 0;
    for (unsigned int i = 0; i < rawInput.length(); i++){
        if (rawInput[i] == ' ') {
            start++;
        } else {
            break;
        }
    }

    // Copy string into char array and convert it to uppercase
    char characters[rawInput.length() - start + 1];
    rawInput.toCharArray(characters, rawInput.length(), start);
    strupr(characters);

    // Parse the input into (Command) (Subcommand) (arguments) structure.
    char space_delimiter = ' ';
    char colon_delimiter = ':';
    char * p = characters;
    char ** ptr = &p;
    char * command = strsep(ptr, &space_delimiter);
    char * subcommand = strsep(ptr, &space_delimiter);
    char * rest = *ptr;
    
    int arguments[6] = {0};
    for (int i = 0; i < 6; i++) {
        arguments[i] = strtoul(strsep(ptr, &colon_delimiter), nullptr, 10);
    }

    // RESET to default settings
    if (strcmp(command, "RESET") == 0) {
        eeprom.restoreFactorySettings();
        delay(1000);
        rtc.scheduleNextAlarmForCurrentConfig();
    } 

    // SET Command for setting values
    else if (strcmp(command, "SET") == 0) {        
        // Set the rtc clock to a specified date and time | arguments = YYYY:MM:DD:hh:mm
        if (strcmp(subcommand, "CLOCK") == 0) {
            rtc.rtc.adjust(DateTime(arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]));
            rtc.scheduleNextAlarmForCurrentConfig();
        } 

        // Set the exact time of the day and time of the month to sample water | arguments = DD:hh:mm
        else if (strcmp(subcommand, "EXACT") == 0) {
            Config::current().sampleAlarmDay = arguments[0];
            Config::current().sampleAlarmHr = arguments[1];
            Config::current().sampleAlarmMin = arguments[2];
            Config::current().isAlarmInteval = false;
        }

        // Set the alarm type to interval to periodic sample the water | arguments = DD:hh:mm
        else if (strcmp(subcommand, "INTERVAL") == 0) {
            Config::current().sampleAlarmDay = arguments[0];
            Config::current().sampleAlarmHr = arguments[1];
            Config::current().sampleAlarmMin = arguments[2];
            Config::current().isAlarmInteval = true;
        }

        // Set the sample duration in seconds | arguments = mm:ss
        else if (strcmp(subcommand, "SAMPLE") == 0) {
            Config::current().sampleDuration = arguments[0] * 60 + arguments[1];
        }

        // Set the flush duration in seconds | arguments = mm:ss
        else if (strcmp(subcommand, "FLUSH") == 0) {
            Config::current().flushDuration = arguments[0] * 60 + arguments[1];
        }

        // Turn the specified valve to on or off | arguments = 1 or 0
        else if (subcommand[0] == 'V') {
            int valveNumber = strtol(subcommand + 1, nullptr, 10);
            int operation = arguments[0] == 1 ? true : false;

            if (operation < 0 || operation > 1 || valveNumber < 0 || valveNumber > 24) {
                Serial.println();
                Serial.println(F("Invalid Command"));
                return;
            }

            shift.registerWrite(valveNumber == 0 ? 0 : (valveNumber + 8), operation);
        }

        // Manually turn on the motor | arguments = 1 or 0
        else if (subcommand[0] == 'M') {
            int operation = arguments[0];
            if (operation < 0 || operation > 1) {
                Serial.println();
                Serial.println(F("Invalid Command"));
                return;
            }

            unsigned long v = *((unsigned long *) shift.outputs);
            if (v == 0) {
                Serial.println();
                Serial.println(F("Invalid Operation: No Valve is currently opened"));
                return;
            }

            if (operation) {
                pump.on();
            } else {
                pump.off();
            }
        }
    }


    // GET Command for retriving infomation
    else if (strcmp(command, "GET") == 0) {

        // Get the current status of sampler
        if (strcmp(subcommand, "STATUS") == 0) {
            Config::current().printCurrentSettings();
            Serial.print("Current TPIC Index: " );
            Serial.println(((long *) shift.outputs)[0], BIN);
            rtc.displayCurrentTime();
        }
        
        // Get only the current valve number
        else if (strcmp(subcommand, "VALVE") == 0) {
            Serial.println(Config::current().valve);
        }

        // Get the current time
        else if (strcmp(subcommand, "TIME") == 0) {
            rtc.displayCurrentTime();
        }
    }
}

// Switch the mode of operation to either programming or operating
void changeOperatingMode() {
    if (millis() - intetrruptStart < 500) {
        return;
    }

    detachInterrupt(digitalPinToInterrupt(PROGRAMMING_ISR_PIN));
    programmingMode = !programmingMode;
    interruptTriggered = true;
    intetrruptStart = millis();
} 


void initialize() {
    Serial.begin(9600);
    delay(1000);

    pinMode(PROGRAMMING_ISR_PIN, INPUT_PULLUP);
    programmingMode = digitalRead(PROGRAMMING_ISR_PIN);
    
    shift.setPins(11, 13, 10); // data, clock, latch
    serialBuffer.didReceiveSerialCommand(userSerialInput);

	app.addComponent(&serialBuffer);
    app.addComponent(&rtc);
    app.addComponent(&eeprom);
	app.addComponent(&shift);
    app.addComponent(&pump);
}

//----------------------------------------------------------------------------------------------------
// Provide injection before update is called for the first time. At this point, all components    
// have been initialized and their setup() have been called.                                       
//----------------------------------------------------------------------------------------------------
void didSetupComponents() {
    if (Config::current().isAlarmInteval) {
        rtc.scheduleNextAlarmForCurrentConfig();
    } else {

    }
}

void samplingRoutine() {
    Config & c = Config::current();
    if (c.valve >= shift.size) {
            Serial.println(F("Sleep Forever"));
            rtc.sleepForever();
            return;
    } else {
        c.printCurrentSettings();
    }

    // Flushing  
    shift.registerWrite(0, HIGH);
    pump.on();
    rtc.sleepFor(c.flushDuration);
    pump.off();
    shift.registerWrite(0, LOW);

    //Sampling
    shift.registerWrite(c.valve, HIGH);
    pump.on();
    rtc.sleepFor(c.sampleDuration);
    pump.off();
    shift.registerWrite(c.valve, LOW);
}

void update() {    
    if (interruptTriggered) {
        if (programmingMode) {
            Serial.println(F("-= Programming Mode =-"));
        } else {
            Serial.println(F("-= Operating Mode =-"));
        }

        Serial.println(freeMemory());
        interruptTriggered = false;
    }

    if (programmingMode) {
        attachInterrupt(digitalPinToInterrupt(PROGRAMMING_ISR_PIN), changeOperatingMode, FALLING);
    } else {
        attachInterrupt(digitalPinToInterrupt(PROGRAMMING_ISR_PIN), changeOperatingMode, RISING);
        if (alarmTriggered) {
            Serial.println(F("*** Alarm Missed ***"));
            rtc.displayCurrentTime();
            rtc.scheduleNextAlarmForCurrentConfig();
            alarmTriggered = false;
        }

        rtc.sleepForever();

        if (alarmTriggered) {
            detachInterrupt(digitalPinToInterrupt(PROGRAMMING_ISR_PIN));
            samplingRoutine();
            Config::current().valve++;
            eeprom.saveCurrentConfig();
            rtc.scheduleNextAlarmForCurrentConfig();
            alarmTriggered = false;
        }
    }
}

OPSystem app(initialize, didSetupComponents, update);