#include "databasehandler.h"

QStringList DatabaseHandler::GetItems()
{
    QSystemSemaphore qss("dbsemaphore", 1);
    qss.acquire();

    QStringList results;
    QSqlDatabase sqlDb = QSqlDatabase::database();
    if(!sqlDb.isValid())
        sqlDb = QSqlDatabase::addDatabase("QODBC3");

    sqlDb.setDatabaseName("Driver={SQL Server};Server=ADAM-PC;Database=TestDatabase;Uid=TestUser;Pwd=P@ssw0rd;");
    qDebug() << "Opening DB connection.";
    bool connected = sqlDb.open();
    if(connected)
    {
        qDebug() << "Running query.";
        QSqlQuery flim(sqlDb);
        flim.prepare("SELECT * FROM Item");
        flim.exec();
        while(flim.next())
        {
            try
            {
                results.push_back(flim.value(0).toString() + "," + flim.value(1).toString() + "," + flim.value(2).toString() + "," + flim.value(3).toString());
            } catch (std::exception)
            {
                qDebug() << "Error with sql query.";
            }
        }
    }
    else
    {
        qDebug() << sqlDb.lastError();
    }
    sqlDb.close();
    QSqlDatabase::removeDatabase("QODBC3");
    qss.release(1);
    return results;
}

QStringList DatabaseHandler::GetOrders()
{
    QSystemSemaphore qss("dbsemaphore", 1);
    qss.acquire();

    QStringList results;
    QSqlDatabase sqlDb = QSqlDatabase::database();
    if(!sqlDb.isValid())
        sqlDb = QSqlDatabase::addDatabase("QODBC3");

    sqlDb.setDatabaseName("Driver={SQL Server};Server=ADAM-PC;Database=TestDatabase;Uid=TestUser;Pwd=P@ssw0rd;");
    bool connected = sqlDb.open();
    if(connected)
    {
        QSqlQuery flim(sqlDb);
        flim.prepare("SELECT * FROM OrderDetail");
        flim.exec();
        while(flim.next())
        {
            try
            {
                results.push_back(flim.value(0).toString() + "," + flim.value(1).toString() + "," + flim.value(2).toString());
            } catch (std::exception)
            {
                qDebug() << "Error with sql query.";
            }
        }
    }
    else
    {
        qDebug() << sqlDb.lastError();
    }
    sqlDb.close();
    QSqlDatabase::removeDatabase("QODBC3");
    qss.release(1);
    return results;
}

void DatabaseHandler::UpdateItem(int ID, int quantity)
{
    QSystemSemaphore qss("dbsemaphore", 1);
    qss.acquire();

    QStringList results;

    QSqlDatabase sqlDb = QSqlDatabase::database();
    if(!sqlDb.isValid())
        sqlDb = QSqlDatabase::addDatabase("QODBC3");

    sqlDb.setDatabaseName("Driver={SQL Server};Server=ADAM-PC;Database=TestDatabase;Uid=TestUser;Pwd=P@ssw0rd;");
    bool connected = sqlDb.open();
    if(connected)
    {
        QSqlQuery flim(sqlDb);
        flim.prepare("SELECT * FROM Orders");
        flim.exec();
        int numberOfOrders = 0;
        while(flim.next())
        {
            numberOfOrders++;
        }
        numberOfOrders++;

        flim.prepare("SELECT AvailableQuantity FROM Item WHERE ItemID=:iid");
        flim.bindValue(":iid", ID);
        flim.exec();
        flim.next();
        int currentQuantity = flim.value(0).toInt();

        flim.prepare("UPDATE Item SET AvailableQuantity=:quant WHERE ItemID=:iid");
        flim.bindValue(":quant", (currentQuantity - quantity));
        flim.bindValue(":iid", ID);
        flim.exec();

        flim.prepare("INSERT INTO Orders VALUES (:order,:date)");
        flim.bindValue(":order", numberOfOrders);
        QDate d = QDate::currentDate();
        QString day = QString::number(d.day()).rightJustified(2, '0');
        QString month = QString::number(d.month()).rightJustified(2, '0');
        QString year = QString::number(d.year()).rightJustified(4,'0');

        flim.bindValue(":date", QString(year+"-"+month+"-"+day));
        flim.exec();

        flim.prepare("INSERT INTO OrderDetail(ItemID, OrderID, OrderQuantity) VALUES (:iid, :oid, :oq)");
        flim.bindValue(":iid", ID);
        flim.bindValue(":oid", numberOfOrders);
        flim.bindValue(":oq", quantity);
        flim.exec();
    }
    else
    {
        qDebug() << sqlDb.lastError();
    }
    sqlDb.close();
    QSqlDatabase::removeDatabase("QODBC3");
    qss.release();
}
