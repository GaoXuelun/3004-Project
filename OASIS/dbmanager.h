#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>
#include <QMessageBox>

class DBManager
{
    public:
        static const QString DATABASE_PATH;
        DBManager();

    private:
        QSqlDatabase RMBDB;
        bool dbInit();
};

#endif // DBMANAGER_H
