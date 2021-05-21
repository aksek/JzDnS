#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QLabel>

class UserWindow;
class AdminWindow;
class WelcomeWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getServer();
    QString getPublicKeyPath();
    QString getPrivateKeyPath();
    QString getNick();

private slots:
    void on_action_Add_triggered();

    void on_action_Delete_triggered();

    void on_actionChoose_server_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *serverStatus;

    UserWindow *userWindow;
    AdminWindow *adminWindow;
    WelcomeWindow *welcomeWindow;

    QString server, publicKeyPath, privateKeyPath, nick;

    void enableEditControls(bool enable);
};

#endif // MAINWINDOW_H
