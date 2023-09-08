#include <iostream>
#include <cassert>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

// Enumeration for vital types
enum class VitalType { Temperature, PulseRate, Spo2 };

// Enumeration for vital status
enum class VitalStatus { Ok, Critical, Warning };

// Enumeration for supported languages
enum class Language { English, German, /* Add more languages here */ };

// Global variable to store the current language
Language currentLanguage = Language::English;

// Struct to hold vital information
struct VitalInfo {
    float lowerLimit;
    float upperLimit;
    float tolerance;
    const char* criticalMessage;
    const char* warningMessage;
};

// Map to store vital information
map<VitalType, VitalInfo> vitalsInfo = {
    {VitalType::Temperature, {95, 102, 1.5, "Temperature critical!", "Warning: Approaching hypothermia or hyperthermia!"}},
    {VitalType::PulseRate, {60, 100, 1.5, "Pulse Rate is out of range!", "Warning: Approaching abnormal pulse rate!"}},
    {VitalType::Spo2, {90, 100, 1.5, "Oxygen Saturation out of range!", "Warning: Approaching low oxygen saturation!"}}
};

// Map to store translations for different languages
map<Language, map<string, string>> translations = {
    {Language::English, {
        {"Temperature critical!", "Temperature critical!"},
        {"Pulse Rate is out of range!", "Pulse Rate is out of range!"},
        {"Oxygen Saturation out of range!", "Oxygen Saturation out of range!"},
        {"Warning: Approaching hypothermia or hyperthermia!", "Warning: Approaching hypothermia or hyperthermia!"},
        {"Warning: Approaching abnormal pulse rate!", "Warning: Approaching abnormal pulse rate!"},
        {"Warning: Approaching low oxygen saturation!", "Warning: Approaching low oxygen saturation!"},
        {"All tests passed.", "All tests passed."}
    }},
    {Language::German, {
        {"Temperature critical!", "Temperatur kritisch!"},
        {"Pulse Rate is out of range!", "Pulsrate ist außerhalb des Bereichs!"},
        {"Oxygen Saturation out of range!", "Sauerstoffsättigung außerhalb des Bereichs!"},
        {"Warning: Approaching hypothermia or hyperthermia!", "Warnung: Annäherung an Unterkühlung oder Hyperthermie!"},
        {"Warning: Approaching abnormal pulse rate!", "Warnung: Annäherung an abnormale Pulsrate!"},
        {"Warning: Approaching low oxygen saturation!", "Warnung: Annäherung an niedrige Sauerstoffsättigung!"},
        {"All tests passed.", "Alle Tests bestanden."}
    }}
    /* Add more languages here */
};

void simulateBlinkingEffect() {
    for (int i = 0; i < 6; i++) {
        cout << "\r* " << flush;
        sleep(1);
        cout << "\r *" << flush;
        sleep(1);
    }
}

bool isOutOfRange(float value, const VitalInfo& info) {
    return value < info.lowerLimit || value > info.upperLimit;
}

bool isApproachingWarning(float value, const VitalInfo& info) {
    float lowerThreshold = info.lowerLimit + (info.upperLimit - info.lowerLimit) * (1 - info.tolerance / 100.0);
    float upperThreshold = info.upperLimit - (info.upperLimit - info.lowerLimit) * (1 - info.tolerance / 100.0);
    return value >= lowerThreshold && value <= upperThreshold;
}

VitalStatus checkVital(VitalType type, float value) {
    const VitalInfo& info = vitalsInfo[type];
    if (isOutOfRange(value, info)) {
        if (isApproachingWarning(value, info)) {
            cout << translations[currentLanguage][info.warningMessage] << endl;
            return VitalStatus::Warning;
        }
        else {
            cout << translations[currentLanguage][info.criticalMessage] << endl;
            return VitalStatus::Critical;
        }
    }
    return VitalStatus::Ok;
}

void setLanguage(Language language) {
    currentLanguage = language;
}

string translate(const string& message) {
    if (translations.find(currentLanguage) != translations.end() &&
        translations[currentLanguage].find(message) != translations[currentLanguage].end()) {
        return translations[currentLanguage][message];
    }
    // If translation not found, return the message in English as a fallback
    return message;
}

bool vitalsOk(float temperature, float pulseRate, float spo2) {
    bool isTemperatureOk = checkVital(VitalType::Temperature, temperature) == VitalStatus::Ok;
    bool isPulseRateOk = checkVital(VitalType::PulseRate, pulseRate) == VitalStatus::Ok;
    bool isSpo2Ok = checkVital(VitalType::Spo2, spo2) == VitalStatus::Ok;

    return isTemperatureOk && isPulseRateOk && isSpo2Ok;
}

int main() {
    // Set the language to German
    setLanguage(Language::German);

    // Test temperature
    assert(!vitalsOk(94, 70, 98));
    assert(!vitalsOk(103, 70, 98));
    assert(!vitalsOk(95.015, 70, 98)); // Approaching hypothermia
    assert(!vitalsOk(100.485, 70, 98)); // Approaching hyperthermia

    // Test pulse rate
    assert(!vitalsOk(98.1, 59, 98));
    assert(!vitalsOk(98.1, 101, 98));
    assert(!vitalsOk(98.1, 99.4, 98)); // Approaching abnormal pulse rate

    // Test spo2
    assert(!vitalsOk(98.1, 70, 89));
    assert(!vitalsOk(98.1, 70, 75)); // Approaching low oxygen saturation

    // Test combinations
    assert(!vitalsOk(94, 59, 89));
    assert(!vitalsOk(103, 101, 75)); // Approaching hypothermia, abnormal pulse rate, low oxygen saturation
    assert(!vitalsOk(94, 101, 75));  // Approaching hypothermia, abnormal pulse rate, low oxygen saturation
    assert(!vitalsOk(103, 59, 98.1)); // Approaching hypothermia, abnormal pulse rate

    assert(vitalsOk(98.1, 70, 98));
    assert(vitalsOk(98.1, 70, 90));

    cout << translate("All tests passed.") << endl;
}
