#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    Watek* watek = new Watek;
    watek->moveToThread(&watekUchwyt);
    makeConnections(watek);
    watekUchwyt.start();
}

Dialog::~Dialog()
{
    watekUchwyt.quit();
    watekUchwyt.wait();
    delete ui;
}

void Dialog::finish()
{
    this->close();
}

void Dialog::makeConnections(const Watek * const _watek) const
{
    connect(&watekUchwyt, &QThread::finished, _watek, &QObject::deleteLater, Qt::ConnectionType::DirectConnection);
    connect(this, &Dialog::run, _watek, &Watek::run, Qt::ConnectionType::QueuedConnection);
    connect(_watek, &Watek::finish, this, &Dialog::finish, Qt::ConnectionType::QueuedConnection);
}

void Dialog::on_pushButton_clicked()
{
    emit run();
}

