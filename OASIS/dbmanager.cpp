#include "dbmanager.h"

const QString DBManager::DATABASE_PATH = "/database/RMB.db";

DBManager::DBManager()
{
    RMBDB = QSqlDatabase::addDatabase("QSQLITE");
    RMBDB.setDatabaseName("RMB.db");
    if (!RMBDB.open()) {
        throw "Error: Database could not be opened";
        }
        if (!dbInit()) {
            throw "Error: Database could not be initialized";
        }
}
bool DBManager::dbInit()
{
    RMBDB.transaction();
    QSqlQuery query;
    query.exec("create table person (id int primary key,"
               "user varchar(20), "
                "sessiontype varchar(20),"
               "duration varchar(20),"
               "intensitylevel varchar(20))");
     return RMBDB.commit();
}
