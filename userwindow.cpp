#include <QMessageBox>

#include "userwindow.h"
#include "ui_userwindow.h"
#include "adapter.h"

UserWindow::UserWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    model = new ProblemsModel(this);
    displayNewProblem();
}

UserWindow::~UserWindow()
{
    delete ui;
    delete model;
}

void UserWindow::on_pushButtonSubmit_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Result");
    if (Adapter::checkAnswer(ui->lineEditAnswer->text())) {
        msgBox.setText("Congratulations! The answer is correct");
        displayNewProblem();
    } else {
        msgBox.setText("Sorry! That's not right.");
    }
    msgBox.exec();
}

void UserWindow::displayNewProblem()
{
    model->loadCurrentProblem();
    ui->textBrowserCurrentProblem->setText(model->data(model->index(0, 1, QModelIndex()), Qt::DisplayRole).toString());
}
