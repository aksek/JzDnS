#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ObslugaKomunikatow* _komunikaty = new ObslugaKomunikatow();
    _komunikaty->moveToThread(&komunikaty);
    makeConnections(_komunikaty);
    komunikaty.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    komunikaty.quit();
    komunikaty.wait();
}

void MainWindow::makeConnections(const ObslugaKomunikatow * const _komunikaty) const
{
    connect(&komunikaty, &QThread::finished, _komunikaty, &QObject::deleteLater, Qt::ConnectionType::DirectConnection);

}

