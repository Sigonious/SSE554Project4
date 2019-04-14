#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include "orderthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OrderButton_clicked();
    void orderPlaced(QString orderDetails);

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcp;
    const quint16 port = 60800;
};

#endif // MAINWINDOW_H
