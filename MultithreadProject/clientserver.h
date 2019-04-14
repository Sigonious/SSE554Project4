#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QTcpServer>
#include <QStringList>

#include "clientthread.h"

class ClientServer : public QTcpServer
{
    Q_OBJECT

public:
    ClientServer(QObject *parent = 0);
    void UpdateInventory(QStringList inventory);

public slots:
    void orderFinished(QStringList orderDetails);

signals:
    void orderPlaced(QStringList orderDetails);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QStringList items;
};

#endif // CLIENTSERVER_H
