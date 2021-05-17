#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    explicit Login(QString server, QWidget *parent = nullptr);
    ~Login();

    QString getPublicKeyFileName();
    QString getPrivateKeyFileName();

private slots:
    void on_pushButtonChoosePublicKey_clicked();
    void on_pushButtonChoosePrivateKey_clicked();

private:
    Ui::Login *ui;
    QFileDialog *fileDialog;
    QString publicKeyFileName;
    QString privateKeyFileName;

    QString chooseFileName();
};

#endif // LOGIN_H
