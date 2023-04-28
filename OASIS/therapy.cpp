#include "therapy.h"

Therapy::Therapy(int time, QString name, int i)
{
    timeDuration = time;
    sessionName = name;
    intensity = i;
    timer = new QTimer(this);
}

Therapy::~Therapy()
{
    delete timer;
}

// getters
int Therapy::getTimeDuration() { return timeDuration; }
int Therapy::getIntensity() { return intensity; }
QString Therapy::getSessionName() { return sessionName; }
QTimer* Therapy:: getTimer() { return timer; }

// setters
void Therapy::setTimeDuration(int time) { timeDuration = time; }
void Therapy::setSessionName(QString name) { sessionName = name; }
void Therapy::setIntensity(int i) { intensity = i; }
