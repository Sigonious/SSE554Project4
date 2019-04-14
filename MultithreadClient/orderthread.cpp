#include "orderthread.h"

OrderThread::OrderThread(int mode, int iid, int iq) : selectedMode(mode), itemID(iid), itemQuantity(iq)
{

}

void OrderThread::run()
{
    tcp = new QTcpSocket();
    connect(tcp, SIGNAL(disconnected()), tcp, SLOT(deleteLater()));
    connect(tcp, SIGNAL(readyRead()), this, SLOT(DataReceived()));
    tcp->connectToHost("127.0.0.1", 60800);
    tcp->waitForConnected();

    switch(selectedMode)
    {
        case 0: //retrieve item data
        {
            tcp->write("0");
            tcp->waitForReadyRead();
            break;
        }
        case 1: //send order
        {
            sendOrder();
            break;
        }
    }
}

void OrderThread::sendOrder()
{
    QByteArray orderBytes;
    orderBytes.push_back("1");
    orderBytes.push_back("0001");
    orderBytes.push_back(QString::number(itemID).rightJustified(10).toUtf8());
    orderBytes.push_back(QString::number(itemQuantity).rightJustified(10).toUtf8());

    tcp->write(orderBytes);
    tcp->waitForBytesWritten();
}

void OrderThread::DataReceived()
{
    QByteArray data = tcp->readAll();
    qDebug() << data;

    int state = data.mid(0, sizeof(char)).toInt();
    switch(state)
    {
        case 0:
        {
            int currentIndex = 5;
            for(int i = 0; i < data.mid(1, sizeof(int)).toInt(); i++)
            {
                int itemID = data.mid(currentIndex, 10).replace(" ", "").rightJustified(4).toInt();
                currentIndex += 10;
                QString itemName = data.mid(currentIndex, 20).replace(" ", "");
                currentIndex += 20;
                int itemQuantity = data.mid(currentIndex, 10).replace(" ", "").toInt();
                currentIndex += 10;
                float itemPrice = data.mid(currentIndex, 10).replace(" ", "").toFloat();
                currentIndex += 10;
                qDebug() << "Adding item: " + QString::number(itemID) + " : " + itemName + " : " + QString(itemQuantity) + " : " + QString::number(itemPrice);
                emit done(QString::number(itemID) + "\t" + itemName + "\t" +  QString::number(itemQuantity) + "\t" +  QString::number(itemPrice));
            }
            break;
        }
        case 1:
        {
            qDebug() << "Order was successfully placed.";
            break;
        }
    }
}
