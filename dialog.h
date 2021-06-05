#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QThread>

#include "watek.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

signals:
    void run();

public slots:
    void finish();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    QThread watekUchwyt;

    void makeConnections(const Watek* const _watek) const;
};

#endif // DIALOG_H
