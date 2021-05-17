#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userwindow.h"
#include "adminwindow.h"
#include "welcomewindow.h"
#include "chooseserver.h"
#include "login.h"
#include "adapter.h"

void MainWindow::enableEditControls(bool enable)
{
    ui->menuData->setEnabled(enable);
    if (enable) {
        ui->toolBar->show();
    } else {
        ui->toolBar->hide();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    enableEditControls(false);
    serverStatus = new QLabel("");
    ui->statusbar->addWidget(serverStatus);

    welcomeWindow = new WelcomeWindow(this);
    ui->mdiArea->addSubWindow(welcomeWindow);
    welcomeWindow->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Add_triggered()
{
    adminWindow->addNewProblem();
}

void MainWindow::on_action_Delete_triggered()
{
    adminWindow->deleteSelectedProblem();
}

void MainWindow::on_actionChoose_server_triggered()
{
    ui->mdiArea->closeAllSubWindows();
    ui->statusbar->removeWidget(serverStatus);

    enableEditControls(false);

    QString server, publicKeyPath, privateKeyPath;

    ChooseServer chooseServerDialog;
    chooseServerDialog.setModal(true);
    if (chooseServerDialog.exec()) {
        server = chooseServerDialog.getChosenServer();
        serverStatus->setText(server);
        serverStatus->show();
        ui->statusbar->addWidget(serverStatus);
        Login loginDialog(server, this);
        loginDialog.setModal(true);
        if (loginDialog.exec()) {
            publicKeyPath = loginDialog.getPublicKeyFileName();
            privateKeyPath = loginDialog.getPrivateKeyFileName();

            if (Adapter::keyIsValid(publicKeyPath, privateKeyPath)) {
                if (Adapter::isAdmin(privateKeyPath)) {
                    adminWindow = new AdminWindow(this);
                    ui->mdiArea->addSubWindow(adminWindow);
                    adminWindow->showMaximized();
                    enableEditControls(true);
                } else {
                    userWindow = new UserWindow(this);
                    ui->mdiArea->addSubWindow(userWindow);
                    userWindow->showMaximized();
                }
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Invalid login");
                msgBox.setText("These keys are invalid");
                msgBox.exec();
            }
        }
    }
}
