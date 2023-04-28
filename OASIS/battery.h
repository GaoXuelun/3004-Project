#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>

/* Battery object, store battery level */

class Battery
{
    public:
        Battery(double batteryLevel);
        // getter
        double getBatteryLevel();
        //setter
        void setBatteryLevel(double newLevel);

    private:
        double batteryLevel;
};

#endif // BATTERY_H
