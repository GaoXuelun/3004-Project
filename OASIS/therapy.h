#ifndef THERAPY_H
#define THERAPY_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>

/* Class Purpose: Therapy object, store time duration, session name, and intensity */

class Therapy: public QObject
{
    Q_OBJECT

    public:
        Therapy(int timeDuration, QString sessionName, int intensity);
        ~Therapy();
        // getter
        int getTimeDuration();
        int getIntensity();
        QString getSessionName();
        QTimer* getTimer();
        //setter
        void setTimeDuration(int time);
        void setSessionName(QString name);
        void setIntensity(int i);

    private:
        int timeDuration;
        int intensity;
        QString sessionName;
        QTimer* timer;

};

#endif // THERAPY_H
