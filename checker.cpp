#include <iostream>
#include <cassert>
#include <unistd.h>
using namespace std;

void simulateBlinkingEffect() {
    for (int i = 0; i < 6; i++) {
        cout << "\r* " << flush;
        sleep(1);
        cout << "\r *" << flush;
        sleep(1);
    }
}

bool isOutOfRange(float value, float lower, float upper) {
    return value < lower || value > upper;
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    if (isOutOfRange(temperature, 95, 102)) {
        cout << "Temperature critical!\n";
        simulateBlinkingEffect();
        return 0;
    }
    if (isOutOfRange(pulseRate, 60, 100)) {
        cout << "Pulse Rate is out of range!\n";
        simulateBlinkingEffect();
        return 0;
    }
    if (isOutOfRange(spo2, 90, 100)) {
        cout << "Oxygen Saturation out of range!\n";
        simulateBlinkingEffect();
        return 0;
    }
    return 1;
}

int main() {
    // Test temperature
    assert(!vitalsOk(94, 70, 98));
    assert(!vitalsOk(103, 70, 98));

    // Test pulse rate
    assert(!vitalsOk(98.1, 59, 98));
    assert(!vitalsOk(98.1, 101, 98));

    // Test spo2
    assert(!vitalsOk(98.1, 70, 89));

    // Test combinations
    assert(!vitalsOk(94, 59, 89));
    assert(!vitalsOk(103, 101, 89));
    assert(!vitalsOk(94, 101, 89));
    assert(!vitalsOk(103, 59, 89));

    assert(vitalsOk(98.1, 70, 98));
    assert(vitalsOk(98.1, 70, 90));

    cout << "All tests passed.\n";
}