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
            cout << info.warningMessage << endl;
            return VitalStatus::Warning;
        }
        else {
            cout << info.criticalMessage << endl;
            return VitalStatus::Critical;
        }
    }
    return VitalStatus::Ok;
}

bool vitalsOk(float temperature, float pulseRate, float spo2) {
    bool isTemperatureOk = checkVital(VitalType::Temperature, temperature) == VitalStatus::Ok;
    bool isPulseRateOk = checkVital(VitalType::PulseRate, pulseRate) == VitalStatus::Ok;
    bool isSpo2Ok = checkVital(VitalType::Spo2, spo2) == VitalStatus::Ok;

    return isTemperatureOk && isPulseRateOk && isSpo2Ok;
}

int main() {
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

    cout << "All tests passed." << endl;
}
