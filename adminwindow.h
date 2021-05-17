#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include <QDataWidgetMapper>
#include<QStandardItemModel>

class ProblemsModel;

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

    void addNewProblem();
    void deleteSelectedProblem();

private slots:
    void updateEditor(const QModelIndex &index);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_textEditAnswer_textChanged();

    void on_textEditProblem_textChanged();

private:
    Ui::AdminWindow *ui;
    ProblemsModel *model;
    QDataWidgetMapper *mapper;

    QModelIndex listViewSelectedIndex();
};

#endif // ADMINWINDOW_H
