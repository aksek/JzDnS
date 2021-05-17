#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    fileDialog = new QFileDialog(this);
    fileDialog->setModal(true);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
}

Login::Login(QString server, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    fileDialog = new QFileDialog(this);
    fileDialog->setModal(true);
    fileDialog->setFileMode(QFileDialog::ExistingFile);

    ui->labelServer->setText("Server: " + server);
}

Login::~Login()
{
    delete ui;
    delete fileDialog;
}

QString Login::chooseFileName()
{
    QStringList keyFileNames;
    if (fileDialog->exec()) {
        keyFileNames = fileDialog->selectedFiles();
        return keyFileNames[0];
    }
    else return QString();
}

void Login::on_pushButtonChoosePublicKey_clicked()
{
    publicKeyFileName = chooseFileName();
    ui->lineEditPublicKeyPath->setText(publicKeyFileName);
}

void Login::on_pushButtonChoosePrivateKey_clicked()
{
    privateKeyFileName = chooseFileName();
    ui->lineEditPrivateKeyPath->setText(privateKeyFileName);
}

QString Login::getPublicKeyFileName()
{
    return publicKeyFileName;
}

QString Login::getPrivateKeyFileName()
{
    return privateKeyFileName;
}
