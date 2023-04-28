#include "connection.h"

Connection::Connection(QString status)
{
    connectionStatus = status;
}

// getters
QString Connection::getConnectionStatus() { return connectionStatus; }

// setters
void Connection::setConnectionStatus(QString status) { connectionStatus = status; }


