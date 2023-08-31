#include <iostream>
#include <assert.h>
#include <unistd.h>
using namespace std;

int vitalsOk(float temperature, float pulseRate, float spo2) {
  if(temperature > 102 || temperature < 95) {
    cout << "Temperature critical!\n";
    for (int i = 0; i < 6; i++)
    {
      cout << "\r* " << flush;
      sleep(1);
      cout << "\r *" << flush;
      sleep(1);
    }

    return 0;
  } else if(pulseRate < 60 || pulseRate > 100) {
    cout << "Pulse Rate is out of range!\n";
    for (int i = 0; i < 6; i++)
    {
      cout << "\r* " << flush;
      sleep(1);
      cout << "\r *" << flush;
      sleep(1);
    }
    return 0;
  } else if(spo2 < 90) {
    cout << "Oxygen Saturation out of range!\n";
    for (int i = 0; i < 6; i++)
    {
      cout << "\r* " << flush;
      sleep(1);
      cout << "\r *" << flush;
      sleep(1);
    }
    return 0;
  }
  return 1;
}

int main() {
  assert(!vitalsOk(99, 102, 70));
  assert(vitalsOk(98.1, 70, 98));
  cout << "Done\n";
}
