#ifndef ORDERTHREAD_H
#define ORDERTHREAD_H

#include <QThread>
class OrderThread : public QThread
{
    Q_OBJECT
    void run() override;
signals:
    void done(QStringList labels);
};

#endif // ORDERTHREAD_H
