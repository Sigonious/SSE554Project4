#ifndef ORDERTHREAD_H
#define ORDERTHREAD_H

#include <QThread>
#include <QTcpSocket>
class OrderThread : public QThread
{
    Q_OBJECT
public:
    OrderThread(int mode, int iid = 0, int iq = 0);
    void run() override;
signals:
    void done(QString labels);
private slots:
    void DataReceived();
private:
    QTcpSocket *tcp;
    int selectedMode;
    int itemID;
    int itemQuantity;

    void sendOrder();
};

#endif // ORDERTHREAD_H
