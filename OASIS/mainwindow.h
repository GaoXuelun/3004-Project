#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QTimer>
#include <QString>

#include "connection.h"
#include "battery.h"
#include "therapy.h"
#include "dbmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define POWER_DRAIN 10000.0

/* Class Purpose: UI window of app, to control the app */

/* Data Members: */
/* UI MainWindow:
 * int Id: user id
 * QString user: user name
 * int timecount: time count for button hold
 * int currentTimerCount: use for time countdown (duration time of therapy)
 * QTimer* timer: timer
*/
/* Connection* connection: Connection object, us to test connection status, store connection status
 * int timeDuration
 * QString sessionName
 * int intensity
*/
/* Battery* battery: Battery object, store battery level
 * int timeDuration
 * QString sessionName
 * int intensity
*/
/* Therapy* therapy: Therapy object, store time duration, session name, and intensity
 * int timeDuration
 * QString sessionName
 * int intensity
 * int therapy status: -1(stop), 0(pause), 1(work)
*/

/* Class Functions:
 * void connectionTest(): test connection status
 * void selectionEnable(bool selectionStatus): set(change) enable for select buttons (time, session, intensity) in different situation
 * void drainBattery(): Drain the battery
 * void initializeBatteryLevel(QTimer*)
 * void initializeTimer(QTimer*): timer for therapy time duration
 * void beginTherapy(int timeDuration, QString sessionName, int intensity): Store the therapy selections
*/

/* Class Slots:
 * void turnOn(): Power on the app
 * void turnOff(): Power off the app
 * void checkSelect(): Start the therapy progress
 * void pauseTherapy(): Pause the therapy progress,can press again to continue if it is paused
 * void stopTherapy(): Stop the therapy progress
 * void changeBatteryLevel(double): Change the battery level
 * void updateBatteryLevel(): Update battery level on ui
 * void batteryCharge():Charge the app when reset battery more than 10 after drop
 * void updateTherapyTimer(): Time count
 * void Record(): Recording therapy
 * void View(): view therapy histroy
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    DBManager* db;
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int Id;
    int timecount;
    int currentTimerCount;

    int timeDuration;
    int intensity;
    int therapyStatus;

    bool powerStatus;

    QString user;
    QString timeString;
    QString sessionName;
    QString connectionStatus;

    QTimer* timer;

    Connection* connection;
    Battery* battery;
    Therapy* therapy;

    void connectionTest();
    void selectionEnable(bool selectionStatus);

    void drainBattery();

    void initializeTimer(QTimer*);
    void initializeBatteryLevel(QTimer*);
    void beginTherapy(int timeDuration, QString sessionName, int intensity);


private slots:
    void turnOn();
    void turnOff();

    void checkSelect();
    void pauseTherapy();
    void stopTherapy();

    void changeBatteryLevel(double);
    void updateBatteryLevel();
    void batteryCharge();

    void updateTherapyTimer();

    void Record();
    void View();

};
#endif // MAINWINDOW_H
