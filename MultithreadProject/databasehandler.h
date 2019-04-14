#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QDate>
#include <QSystemSemaphore>

class DatabaseHandler
{
public:
    static QStringList GetItems();
    static QStringList GetOrders();
    static void UpdateItem(int ID, int quantity);
};

#endif // DATABASEHANDLER_H
