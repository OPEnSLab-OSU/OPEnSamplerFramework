#pragma once
#include "OPComponent.hpp"
#include "EEPROM.h"

#define CHECKSUM 0xAA

// Flash memory conversion which can also be used to save storage space
// struct StorageConfig {
//     byte hasWritten = false;
//     byte isAlarmInterval = false;

//     int sampleAlarmMin = 1;
//     int sampleAlarmHr = 0;
//     int sampleAlarmDay = 0;

//     int sampleDuration = 15; // In second
//     int flushDuration = 15;  // In second
//     int valve = 8;
// };

struct Config {
    byte hasWritten = false;
    bool isAlarmInteval = true;

    int sampleAlarmMin = 1;
    int sampleAlarmHr = 0;
    int sampleAlarmDay = 0;

    int sampleDuration = 5; // In second
    int flushDuration = 5;  // In second
    int valve = 8;

    Config() {}
    Config(Config const&) = delete;
    // void operator =(Config const&) = delete;

    static Config & current() {
        static Config instance;
        return instance;
    }

    static void restoreDefaults() {
        current() = Config();
    }

    static void printCurrentSettings() {
        Config & c = current();

        Serial.println();
        Serial.println(F("-= Current Config =-"));

        Serial.print(F("Loaded From EEPROM: "));
        if (c.hasWritten) {
            Serial.println(F("True"));
        } else {
            Serial.println(F("False"));
        }

        Serial.print(F("Sample Alarm Min: "));
        Serial.println(c.sampleAlarmMin);

        Serial.print(F("Sample Alarm Hour: "));
        Serial.println(c.sampleAlarmHr);

        Serial.print(F("Sample Alarm Day: "));
        Serial.println(c.sampleAlarmDay);

        Serial.print(F("Sample Alarm Type: "));
        if (c.isAlarmInteval) {
            Serial.println(F("Interval"));
        } else {
            Serial.println(F("Schedule"));
        }

        Serial.print(F("Sample Duration: "));
        Serial.print(c.sampleDuration);
        Serial.println(F(" Seconds"));

        Serial.print(F("Flush Duration: "));
        Serial.print(c.flushDuration);
        Serial.println(F(" Seconds"));

        Serial.print(F("Current Valve: "));
        Serial.println(c.valve);
    }

    // void replaceWithStorageConfig(const StorageConfig & sc) {
    //     hasWritten = sc.hasWritten == CHECKSUM;
    //     isAlarmInteval = sc.isAlarmInterval == CHECKSUM;

    //     sampleAlarmMin = int(sc.sampleAlarmMin);
    //     sampleAlarmHr = int(sc.sampleAlarmHr);
    //     sampleAlarmDay = int(sc.sampleAlarmDay);

    //     sampleDuration = int(sc.sampleDuration); // In second
    //     flushDuration = int(sc.flushDuration);  // In second
    //     valve = int(sc.valve);
    // }

    // StorageConfig convertToStorageConfig() {
    //     StorageConfig sc;
    //     sc.hasWritten = hasWritten ? CHECKSUM : false;
    //     sc.isAlarmInterval = hasWritten ? CHECKSUM : false;
    //     // sc.sampleAlarmMin
    //     // On hold
    //     return sc;
    // }

    friend bool operator==(const Config & lhs, const Config & rhs) {
        return 
        lhs.hasWritten == rhs.hasWritten			&&
        lhs.isAlarmInteval == rhs.isAlarmInteval	&&
        lhs.sampleAlarmMin == rhs.sampleAlarmMin	&&
        lhs.sampleAlarmHr == rhs.sampleAlarmHr		&&
        lhs.sampleAlarmDay == rhs.sampleAlarmDay	&& 
        lhs.sampleDuration == rhs.sampleDuration 	&&
		lhs.flushDuration == rhs.flushDuration		&&
		lhs.valve == rhs.valve;
    }

    friend bool operator!=(const Config & lhs, const Config & rhs) {
        return !(lhs == rhs);
    }
};

class OPEEPROM : public OPComponent {
public:
    using OPComponent::OPComponent;

    void setup() override {
        //Configuration file has been written before. Read from it.
        byte hasWritten = EEPROM.read(0);
        if (hasWritten == CHECKSUM) {
            EEPROM.get(0, Config::current());
        } else {
            saveCurrentConfig();
        }

        Config::current().printCurrentSettings();
    }

    void saveCurrentConfig() {
        Config::current().hasWritten = CHECKSUM;
        EEPROM.put(0, Config::current());
        Serial.println(F("Config Saved to EEPROM"));
    }

    void restoreFactorySettings() {
        Config::restoreDefaults();
        Config::current().hasWritten = CHECKSUM;
        EEPROM.put(0, Config::current());
        Serial.println(F("Config Restored & Saved to EEPROM"));
    }
};