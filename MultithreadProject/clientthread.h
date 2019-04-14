#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>

class ClientThread : public QThread
{
    Q_OBJECT
public:
    ClientThread(qintptr socketInfo, QObject *parent = 0);
    void run() override;
    void UpdateInventory(QStringList items);

signals:
    void error(QTcpSocket::SocketError socketError);
    void orderFinished(QStringList orderDetail);

public slots:
    void receiveData();

private:
    QString data;
    QTcpSocket *socket;
    qintptr socketInfo;
    QStringList inventory;
};

#endif // CLIENTTHREAD_H
