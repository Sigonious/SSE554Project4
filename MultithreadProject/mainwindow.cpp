#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    cs = new ClientServer();
    connect(cs, &ClientServer::orderPlaced, this, &MainWindow::orderPlaced);
    cs->listen(QHostAddress::LocalHost, 60800);

    InventoryThread *it = new InventoryThread();
    connect(it, &InventoryThread::done, this, &MainWindow::FillInventory);
    connect(it, &InventoryThread::finished, it, &QObject::deleteLater);

    OrderThread *ot = new OrderThread();
    connect(ot, &OrderThread::done, this, &MainWindow::FillOrders);
    connect(ot, &OrderThread::finished, ot, &QObject::deleteLater);

    it->start();
    ot->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::orderPlaced(QStringList orderDetails)
{
    FillOrders(orderDetails);
}

void MainWindow::FillInventory(QStringList labels)
{
    ui->InventoryList->clear();
    ui->InventoryList->addItem("ID:\tName:\tQuantity:\tPrice:");
    currentInventory = labels;
    cs->UpdateInventory(labels);
    //Retrieve data from SQL server
    for(int i = 0; i < labels.count(); i++)
    {
        ui->InventoryList->addItem(labels[i].replace(" ", "").replace(",", "\t"));
    }
}

void MainWindow::FillOrders(QStringList labels)
{
    ui->OrderList->clear();
    ui->OrderList->addItem("ID:\tItem:\tQuantity:\tPrice");
    //Retrieved data from client
    for(int i = 0; i < labels.count(); i++)
    {
        ui->OrderList->addItem(labels[i].replace(" ", "").replace(",", "\t"));
    }
}
