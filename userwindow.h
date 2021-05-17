#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QWidget>
#include "problemsmodel.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void on_pushButtonSubmit_clicked();

    void displayNewProblem();

private:
    Ui::UserWindow *ui;

    ProblemsModel *model;
};

#endif // USERWINDOW_H
