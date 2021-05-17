#include "serversmodel.h"
#include "chooseserver.h"
#include "ui_chooseserver.h"
#include "login.h"

ChooseServer::ChooseServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseServer)
{
    ui->setupUi(this);
    serversModel = new ServersModel(this);
    ui->comboBoxChooseServer->setModel(serversModel);
}

ChooseServer::~ChooseServer()
{
    delete ui;
    delete serversModel;
}

QString ChooseServer::getChosenServer()
{
    return ui->comboBoxChooseServer->currentText();
}
