#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "problemsmodel.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    model = new ProblemsModel(this);

    model->loadAllProblems();
    ui->listViewProblems->setModel(model);

    connect(ui->listViewProblems->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &AdminWindow::updateEditor);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::addNewProblem()
{
    model->insertRows(listViewSelectedIndex().row() + 1, 1);
}

void AdminWindow::deleteSelectedProblem()
{
    model->removeRow(listViewSelectedIndex().row());
}

void AdminWindow::updateEditor(const QModelIndex &index)
{
    ui->textEditProblem->setPlainText(model->problem(index.row()));
    ui->textEditAnswer->setPlainText(model->answer(index.row()));
}

void AdminWindow::on_buttonBox_accepted()
{
    QModelIndex index = listViewSelectedIndex();

    model->setData(model->index(index.row(), 1, QModelIndex()), ui->textEditProblem->toPlainText(), Qt::EditRole);
    model->setData(model->index(index.row(), 2, QModelIndex()), ui->textEditAnswer->toPlainText(), Qt::EditRole);
    model->saveProblem(listViewSelectedIndex());
}

void AdminWindow::on_buttonBox_rejected()
{
    updateEditor(listViewSelectedIndex());
}

QModelIndex AdminWindow::listViewSelectedIndex()
{
    return ui->listViewProblems->selectionModel()->currentIndex();
}

void AdminWindow::on_textEditAnswer_textChanged()
{
    if (ui->textEditAnswer->toPlainText().isEmpty()) {
        ui->buttonBox->setDisabled(true);
    } else {
        ui->buttonBox->setEnabled(true);
    }
}

void AdminWindow::on_textEditProblem_textChanged()
{
    if (ui->textEditAnswer->toPlainText().isEmpty()) {
        ui->buttonBox->setDisabled(true);
    } else {
        ui->buttonBox->setEnabled(true);
    }
}
