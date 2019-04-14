#include "clientserver.h"
#include "orderthread.h"
#include "databasehandler.h"

ClientServer::ClientServer(QObject *parent) : QTcpServer(parent)
{

}

void ClientServer::incomingConnection(qintptr socketDescription)
{
    ClientThread *cs = new ClientThread(socketDescription);
    cs->UpdateInventory(items);
    connect(cs, SIGNAL(finished()), cs, SLOT(deleteLater()));
    connect(cs, &ClientThread::orderFinished, this, &ClientServer::orderFinished);
    cs->start();
}

void ClientServer::orderFinished(QStringList orderDetails)
{
    emit orderPlaced(orderDetails);
}

void ClientServer::UpdateInventory(QStringList inventory)
{
    items = inventory;
}
