#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QLabel>
#include <QRandomGenerator>
#include <QSqlDatabase>

#include "inventorythread.h"
#include "orderthread.h"
#include "clientserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void orderPlaced(QStringList orderDetails);

signals:
    void DataRetrieved(QStringList labels);

private:
    Ui::MainWindow *ui;

    void FillInventory(QStringList labels);
    void FillOrders(QStringList labels);
    QStringList currentInventory;
    ClientServer *cs;
};

#endif // MAINWINDOW_H
