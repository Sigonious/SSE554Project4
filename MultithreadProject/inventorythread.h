#ifndef INVENTORYTHREAD_H
#define INVENTORYTHREAD_H

#include <QThread>
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

class InventoryThread : public QThread
{
    Q_OBJECT
    void run() override;
signals:
    void done(QStringList labels);
};

#endif // INVENTORYTHREAD_H
