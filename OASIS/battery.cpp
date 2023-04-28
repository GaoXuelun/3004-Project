#include "battery.h"

Battery::Battery(double batteryLevel)
{
    this->batteryLevel = batteryLevel;
}

// getters
double Battery::getBatteryLevel() { return batteryLevel; }

// setters
void Battery::setBatteryLevel(double newLevel) { batteryLevel = newLevel; }


