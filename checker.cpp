#include <iostream>
#include <cassert>
#include <unistd.h>
#include <cmath>
using namespace std;

void simulateBlinkingEffect()
{
    for (int i = 0; i < 6; i++)
    {
        cout << "\r* " << flush;
        sleep(1);
        cout << "\r *" << flush;
        sleep(1);
    }
}

bool isOutOfRange(float value, float lower, float upper)
{
    return value < lower || value > upper;
}

bool isApproachingWarning(float value, float lower, float upper, float tolerance)
{
    float lowerThreshold = lower + (upper - lower) * (1 - tolerance / 100.0);
    float upperThreshold = upper - (upper - lower) * (1 - tolerance / 100.0);
    return value >= lowerThreshold && value <= upperThreshold;
}

int vitalsOk(float temperature, float pulseRate, float spo2)
{
    float temperatureUpperLimit = 102;
    float pulseRateUpperLimit = 100;
    float spo2UpperLimit = 100;
    float tolerance = 1.5; // 1.5% tolerance

    // Check temperature
    if (isOutOfRange(temperature, 95, temperatureUpperLimit))
    {
        if (isApproachingWarning(temperature, 95, temperatureUpperLimit, tolerance))
        {
            cout << "Warning: Approaching hypothermia!\n";
        }
        else if (isApproachingWarning(temperature, temperatureUpperLimit - 1.53, temperatureUpperLimit, tolerance))
        {
            cout << "Warning: Approaching hyperthermia!\n";
        }
        else
        {
            cout << "Temperature critical!\n";
        }
        simulateBlinkingEffect();
        return 0;
    }

    // Check pulse rate
    if (isOutOfRange(pulseRate, 60, pulseRateUpperLimit))
    {
        if (isApproachingWarning(pulseRate, 60, pulseRateUpperLimit, tolerance))
        {
            cout << "Warning: Approaching abnormal pulse rate!\n";
        }
        else
        {
            cout << "Pulse Rate is out of range!\n";
        }
        simulateBlinkingEffect();
        return 0;
    }

    // Check spo2
    if (isOutOfRange(spo2, 90, spo2UpperLimit))
    {
        if (isApproachingWarning(spo2, 90, spo2UpperLimit, tolerance))
        {
            cout << "Warning: Approaching low oxygen saturation!\n";
        }
        else
        {
            cout << "Oxygen Saturation out of range!\n";
        }
        simulateBlinkingEffect();
        return 0;
    }

    return 1;
}

int main()
{
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

    cout << "All tests passed.\n";
}
