#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

/* Connection object, us to test connection status, store connection status */

class Connection
{
    public:
        Connection(QString connectionStatus);
        // getter
        QString getConnectionStatus();
        //setter
        void setConnectionStatus(QString connectionStatus);

    private:
        QString connectionStatus;
};

#endif // CONNECTION_H
