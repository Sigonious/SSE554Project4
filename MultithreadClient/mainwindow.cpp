#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ItemStock->addItem("ID:\tName:\tQuantity:\tPrice:");

    OrderThread *ot = new OrderThread(0);
    connect(ot, &OrderThread::done, this, &MainWindow::orderPlaced);
    connect(ot, &OrderThread::finished, ot, &QObject::deleteLater);
    ot->run();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::orderPlaced(QString orderDetails)
{
    ui->ItemStock->addItem(orderDetails);
}

void MainWindow::on_OrderButton_clicked()
{
    if(ui->ItemIDInput->text().count() <= 0)
        return;

    if(ui->ItemQuantityInput->text().count() <= 0)
        return;

    bool itemIdOkay = false;
    bool itemQuantityOkay = false;
    int itemId = ui->ItemIDInput->text().toInt(&itemIdOkay);
    int itemQuant = ui->ItemIDInput->text().toInt(&itemQuantityOkay);

    if(!itemIdOkay)
    {
        ui->ItemIDInput->setText("Invalid ID");
        return;
    }
    else if(!itemQuantityOkay)
    {
        ui->ItemQuantityInput->setText("Invalid Quantity");
        return;
    }

    OrderThread *ot = new OrderThread(1, itemId, itemQuant);
    connect(ot, &OrderThread::done, this, &MainWindow::orderPlaced);
    connect(ot, &OrderThread::finished, ot, &QObject::deleteLater);
    ot->run();

    ui->ItemIDInput->setText("");
    ui->ItemQuantityInput->setText("");
}
