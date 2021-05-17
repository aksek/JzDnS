#ifndef CHOOSESERVER_H
#define CHOOSESERVER_H

#include <QDialog>
#include <QDataWidgetMapper>

class ServersModel;

namespace Ui {
class ChooseServer;
}

class ChooseServer : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseServer(QWidget *parent = nullptr);
    ~ChooseServer();

    QString getChosenServer();

private slots:

private:
    Ui::ChooseServer *ui;
    ServersModel *serversModel;
};

#endif // CHOOSESERVER_H
