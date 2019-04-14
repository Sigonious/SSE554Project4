#include "clientthread.h"
#include "databasehandler.h"

ClientThread::ClientThread(qintptr socketInfo, QObject *parent) : QThread(parent), socketInfo(socketInfo)
{
    qDebug() << "Client created";
}

void ClientThread::run()
{
    socket = new QTcpSocket;
    if(!socket->setSocketDescriptor(socketInfo))
    {
        emit error(socket->error());
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    socket->waitForReadyRead();
}

void ClientThread::receiveData()
{
    QByteArray receivedData = socket->readAll();
    if(receivedData.length() == 0)
        return;

    QStringList currentInventory = inventory;
    QByteArray sendData;
    int state = receivedData.mid(0, sizeof(char)).toInt();
    switch(state)
    {
        case 0: //Inventory request
        {
            sendData.push_back('0');
            int inventoryCount = currentInventory.length();
            sendData.push_back(QByteArray::number(inventoryCount).rightJustified(4));
            for(int i = 0; i < currentInventory.length(); i++)
            {
                QStringList splitList = currentInventory[i].split(",");
                int id = splitList[0].toInt();
                QString name = splitList[1];
                int quantity = splitList[2].toInt();
                float price = splitList[3].toFloat();
                sendData.push_back(QString::number(id).rightJustified(10).toUtf8());
                sendData.push_back(name.rightJustified(20).toUtf8());
                sendData.push_back(QString::number(quantity).rightJustified(10).toUtf8());
                sendData.push_back(QString::number(price).rightJustified(10).toUtf8());
            }
            socket->write(sendData);
            socket->waitForBytesWritten();
            break;
        }
        case 1: //Order placed
        {
            int numberOfItems = receivedData.mid(1, sizeof(int)).toInt();
            int offset = 1 + sizeof(int);
            for(int i = 0; i < numberOfItems; i++)
            {
                int itemID = receivedData.mid(offset, 10).toInt();
                offset += 10;
                int quantity = receivedData.mid(offset, 10).toInt();
                offset += 10;
                DatabaseHandler::UpdateItem(itemID, quantity);
            }
            QStringList qsl = DatabaseHandler::GetOrders();
            emit orderFinished(qsl);
            socket->write("1");
            socket->waitForBytesWritten();
            break;
        }
    }
}

void ClientThread::UpdateInventory(QStringList items)
{
    inventory = items;
}
