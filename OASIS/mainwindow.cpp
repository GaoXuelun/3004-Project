#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init
    connection = new Connection("ok");
    battery = new Battery(100);
    therapy = new Therapy(0, "Delta", 1);
    db = new DBManager();
    timer = new QTimer(this);

    powerStatus = false;
    Id = 0;
    timecount = 0;
    currentTimerCount = -1;

    ui->batteryLevelBar->setVisible(false);

    // Connection status test
    connectionTest();

    // Power On, Off, Rechare
    connect(ui->powerBtn,&QPushButton::clicked,this,&MainWindow::turnOn);
    connect(ui->powerCloseBtn,&QPushButton::clicked,this,&MainWindow::turnOff);
    connect(ui->chargeBtn,&QPushButton::clicked,this,&MainWindow::batteryCharge);

    // Battery level
    connect(ui->batteryLevelAdminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);

    // Therapy attributes
    connect(ui->checkBtn,&QPushButton::clicked,this,&MainWindow::checkSelect);
    connect(ui->pauseBtn,&QPushButton::clicked,this,&MainWindow::pauseTherapy);
    connect(ui->stopBtn,&QPushButton::clicked,this,&MainWindow::stopTherapy);

    // Therapy record and view
    connect(ui->Record,&QPushButton::clicked,this,&MainWindow::Record);
    connect(ui->View,&QPushButton::clicked,this,&MainWindow::View);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectionTest()
{
    connect(ui->connection_No,&QPushButton::clicked,this,[=](){
        if (powerStatus == false)
        {
            qDebug()<<"connect no";
            connection->setConnectionStatus("no");
            ui->connection_No->setStyleSheet("QRadioButton::indicator:checked {background-color:rgb(240,128,128);}");
        }
    });
    connect(ui->connection_Okay,&QPushButton::clicked,this,[=](){
        if (powerStatus == false)
        {
            qDebug()<<"connect ok";
            connection->setConnectionStatus("ok");
            ui->connection_Okay->setStyleSheet("QRadioButton::indicator:checked {background-color:rgb(255,215,0);}");
        }
    });
    connect(ui->connection_Excellent,&QPushButton::clicked,this,[=](){
        if (powerStatus == false)
        {
            qDebug()<<"connect ex";
            connection->setConnectionStatus("ex");
            ui->connection_Excellent->setStyleSheet("QRadioButton::indicator:checked {background-color:rgb(144,238,144);}");
        }
    });
}

void MainWindow::selectionEnable(bool selectionStatus)
{
    ui->dura_25min->setEnabled(selectionStatus);
    ui->dura_45min->setEnabled(selectionStatus);
    ui->dura_design->setEnabled(selectionStatus);

    ui->session_Delta->setEnabled(selectionStatus);
    ui->session_Theta->setEnabled(selectionStatus);
    ui->session_Alpha->setEnabled(selectionStatus);
    ui->session_Beta->setEnabled(selectionStatus);

    ui->intensity_1->setEnabled(selectionStatus);
    ui->intensity_2->setEnabled(selectionStatus);
    ui->intensity_3->setEnabled(selectionStatus);
    ui->intensity_4->setEnabled(selectionStatus);
    ui->intensity_5->setEnabled(selectionStatus);
    ui->intensity_6->setEnabled(selectionStatus);
    ui->intensity_7->setEnabled(selectionStatus);
    ui->intensity_8->setEnabled(selectionStatus);
}

void MainWindow::drainBattery()
{
    double currentbatteryLevel = (battery->getBatteryLevel() == 0) ? battery->getBatteryLevel() - 0.05 : qMax(battery->getBatteryLevel() - battery->getBatteryLevel()/POWER_DRAIN, 0.0);
    changeBatteryLevel(currentbatteryLevel);
}

void MainWindow::initializeBatteryLevel(QTimer* time)
{
    connect(time, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);
    time->start(1000);
}

void MainWindow::updateTherapyTimer()
{
    drainBattery();
    if (therapy->getTimeDuration() > 0)
    {
        currentTimerCount -= 1;
    }


    if (currentTimerCount >= 0)
    {
        timeString = QString::number(currentTimerCount/60) + ((currentTimerCount%60 < 10) ? + ":0" + QString::number(currentTimerCount%60) : + ":" + QString::number(currentTimerCount%60));
    }
    else
    {
        selectionEnable(true);
    }
    ui->timeLeft->scene()->clear();
    ui->timeLeft->scene()->addText(timeString);
}

void MainWindow::initializeTimer(QTimer* time)
{
    connect(time, &QTimer::timeout, this, &MainWindow::updateTherapyTimer);
    time->start(1000);
}

void MainWindow::beginTherapy(int timeDuration, QString sessionName, int intensity)
{
    if (powerStatus == true)
    {
        therapy->setTimeDuration(timeDuration);
        therapy->setSessionName(sessionName);
        therapy->setIntensity(intensity);
        //Timer
        QGraphicsScene *scene = new QGraphicsScene(this);
        ui->timeLeft->setScene(scene);
        currentTimerCount = therapy->getTimeDuration();
        timeString = QString::number(currentTimerCount/60) + ":00";
        scene->addText(timeString);
        initializeTimer(therapy->getTimer());
    }
}


// Slots
void MainWindow::turnOn()
{
    timecount +=1;
    // turn on
    if (timecount > 10 && powerStatus == false && connection->getConnectionStatus() != "no") // && battery->getBatteryLevel() >= 10
    {
        powerStatus = true;
        qDebug()<<powerStatus;
        ui->batteryLevelBar->setVisible(true);
        //battery->setBatteryLevel(100);
        changeBatteryLevel(battery->getBatteryLevel());
        timecount = 0;
        selectionEnable(true);
        //initializeBatteryLevel(timer);
        ui->checkBtn->setEnabled(true);
        ui->pauseBtn->setEnabled(true);
        ui->stopBtn->setEnabled(true);
        ui->connection_No->setEnabled(false);
        ui->connection_Okay->setEnabled(false);
        ui->connection_Excellent->setEnabled(false);
        return;
    }
}

void MainWindow::turnOff()
{
    timecount +=1;
    // turn off
    if (timecount > 10 && powerStatus == true)
    {
        powerStatus = false;
        qDebug()<<powerStatus;
        ui->batteryLevelBar->setVisible(false);
        //ui->batteryLevelAdminSpinBox->setValue(0);
        timecount = 0;
        selectionEnable(false);
        therapy->setTimeDuration(0);
        therapy->getTimer()->stop();
        //ui->timeLeft->scene()->clear();
        therapyStatus = -1;
        ui->checkBtn->setEnabled(false);
        ui->pauseBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->connection_No->setEnabled(true);
        ui->connection_Okay->setEnabled(true);
        ui->connection_Excellent->setEnabled(true);
        ui->timeLeft->scene()->clear();
        return;
    }
}

void MainWindow::checkSelect()
{
    if (powerStatus == true)
    {
        if (ui->dura_25min->isChecked() == true)
        {
            timeDuration = 25;
        }
        if (ui->dura_45min->isChecked() == true)
        {
            timeDuration = 45;
        }
        if (ui->dura_design->isChecked() == true)
        {
            timeDuration = 65;
        }

        if (ui->session_Delta->isChecked() == true)
        {
            sessionName = "session_Delta";
        }
        if (ui->session_Theta->isChecked() == true)
        {
            sessionName = "session_Theta";
        }
        if (ui->session_Alpha->isChecked() == true)
        {
            sessionName = "session_Alpha";
        }
        if (ui->session_Beta->isChecked() == true)
        {
            sessionName = "session_Beta";
        }

        if (ui->intensity_1->isChecked() == true)
        {
            intensity = 1;
        }
        if (ui->intensity_2->isChecked() == true)
        {
            intensity = 2;
        }
        if (ui->intensity_3->isChecked() == true)
        {
            intensity = 3;
        }
        if (ui->intensity_4->isChecked() == true)
        {
            intensity = 4;
        }
        if (ui->intensity_5->isChecked() == true)
        {
            intensity = 5;
        }
        if (ui->intensity_6->isChecked() == true)
        {
            intensity = 6;
        }
        if (ui->intensity_7->isChecked() == true)
        {
            intensity = 7;
        }
        if (ui->intensity_8->isChecked() == true)
        {
            intensity = 8;
        }

        beginTherapy(timeDuration, sessionName, intensity);
        selectionEnable(false);
        therapyStatus = 1;
        /*
        qDebug()<< therapy->getTimeDuration();
        qDebug()<< therapy->getSessionName();
        qDebug()<< therapy->getIntensity();
        qDebug()<< therapyStatus;
        */
    }
}

void MainWindow::pauseTherapy()
{
    if (powerStatus == true && therapyStatus == 1)
    {
        qDebug()<< "pauseTherapy";
        therapyStatus = 0;
        ui->pauseBtn->setText("Keep");
        therapy->getTimer()->stop();
    }
    else if (powerStatus == true && therapyStatus == 0)
    {
        qDebug()<< "pauseTherapy go ahead";
        therapyStatus = 1;
        ui->pauseBtn->setText("Pause");
        //initializeTimer(therapy->getTimer());
        connect(therapy->getTimer(), &QTimer::timeout, this, &MainWindow::updateTherapyTimer);
        therapy->getTimer()->start(1000);
    }
}

void MainWindow::stopTherapy()
{
    if (powerStatus == true)
    {
        qDebug()<< "stopTherapy";
        therapyStatus = -1;
        selectionEnable(true);
        therapy->getTimer()->stop();
        ui->timeLeft->scene()->clear();
    }
}

void MainWindow::changeBatteryLevel(double newLevel)
{
    if (newLevel >= 0.0 && newLevel <= 100.0)
    {
        if (newLevel == 0.0 && powerStatus == true) {
            //powerChange();
            battery->setBatteryLevel(0);
            //qDebug()<<battery->getBatteryLevel();
        }else{
            battery->setBatteryLevel(newLevel);
            //qDebug()<<battery->getBatteryLevel();
        }
        ui->batteryLevelAdminSpinBox->setValue(newLevel);
        int newLevelInt = int(newLevel);
        ui->batteryLevelBar->setValue(newLevelInt);

        QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(144,238,144);}";
        QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(255,215,0); }";
        QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(240,128,128); }";

        if (powerStatus == true){
            if (newLevelInt >= 50) {
                ui->batteryLevelBar->setStyleSheet(highBatteryHealth);
            }
            else if (newLevelInt >= 20) {
                ui->batteryLevelBar->setStyleSheet(mediumBatteryHealth);
            }
            else {
                ui->batteryLevelBar->setStyleSheet(lowBatteryHealth);
            }
            if (newLevelInt < 10){
                powerStatus = false;
                ui->batteryLevelBar->setVisible(false);
                selectionEnable(false);
                therapy->setTimeDuration(0);
                therapyStatus = -1;
                ui->powerBtn->setEnabled(false);
                ui->powerCloseBtn->setEnabled(false);
                ui->checkBtn->setEnabled(false);
                ui->pauseBtn->setEnabled(false);
                ui->stopBtn->setEnabled(false);
                therapy->getTimer()->stop();
                ui->timeLeft->scene()->clear();
            }
        }
    }
}

void MainWindow::updateBatteryLevel()
{
    drainBattery();
}

void MainWindow::batteryCharge()
{
    if (battery->getBatteryLevel() >= 10)
    {
        ui->powerBtn->setEnabled(true);
        ui->powerCloseBtn->setEnabled(true);
        ui->connection_No->setEnabled(true);
        ui->connection_Okay->setEnabled(true);
        ui->connection_Excellent->setEnabled(true);
    }
}

void MainWindow::Record()
{
    if (powerStatus == true)
    {
        if (ui->dura_25min->isChecked() == true)
        {
            timeDuration = 25;
        }
        if (ui->dura_45min->isChecked() == true)
        {
            timeDuration = 45;
        }
        if (ui->dura_design->isChecked() == true)
        {
            timeDuration = 65;
        }

        if (ui->session_Delta->isChecked() == true)
        {
            sessionName = "session_Delta";
        }
        if (ui->session_Theta->isChecked() == true)
        {
            sessionName = "session_Theta";
        }
        if (ui->session_Alpha->isChecked() == true)
        {
            sessionName = "session_Alpha";
        }
        if (ui->session_Beta->isChecked() == true)
        {
            sessionName = "session_Beta";
        }

        if (ui->intensity_1->isChecked() == true)
        {
            intensity = 1;
        }
        if (ui->intensity_2->isChecked() == true)
        {
            intensity = 2;
        }
        if (ui->intensity_3->isChecked() == true)
        {
            intensity = 3;
        }
        if (ui->intensity_4->isChecked() == true)
        {
            intensity = 4;
        }
        if (ui->intensity_5->isChecked() == true)
        {
            intensity = 5;
        }
        if (ui->intensity_6->isChecked() == true)
        {
            intensity = 6;
        }
        if (ui->intensity_7->isChecked() == true)
        {
            intensity = 7;
        }
        if (ui->intensity_8->isChecked() == true)
        {
            intensity = 8;
        }
        if (ui->user1_->isChecked() == true)
        {
            user = "user1";
        }

        if (ui->user2_->isChecked() == true)
        {
             user = "user2";
        }
        if (ui->user3_->isChecked() == true)
        {
             user = "user3";
        }
    Id+=1;
    }
    QSqlQuery query;
    query.prepare("insert into person(id,user,sessiontype,duration,intensitylevel)"
                  "values(:id,:user,:sessiontype,:duration,:intensitylevel)");
    query.bindValue(":id",Id);
    query.bindValue(":user",user);
    query.bindValue(":sessiontype",sessionName);
    query.bindValue(":duration",timeDuration);
    query.bindValue(":intensitylevel",intensity);
    query.exec();
}

void MainWindow::View()
{
    QString viewRecord;
    QSqlQuery query;
      query.exec("select * from person");

      while(query.next())
         {
            qDebug() << query.value(0).toInt()
                                           << query.value(1).toString()
                                              << query.value(2).toString()
                                                 << query.value(3).toString()
                                                    << query.value(4).toString();

            viewRecord.append(query.value(1).toString() + " - \nTime Duration: " + query.value(3).toString() + ", \nSession Name: " + query.value(2).toString() + ", Intensity: " + query.value(4).toString() +"\n\n");
         }
        ui->reviewLabel->setText(viewRecord);
        ui->reviewLabel->adjustSize();
}









