#ifndef ADAPTER_H
#define ADAPTER_H

#include <QObject>
#include "mainwindow.h"


class Problem;

class Adapter
{
public:
    Adapter();
    static bool isAdmin(QString publicKeyPath);
    static QList<QString> getServers();
    static bool keyIsValid(QString publicKeyPath, QString PrivateKeyPath);
    static QList<Problem *> getAllProblems();
    static Problem *getCurrentProblem();
    static void saveProblem(Problem *problem);
    static bool checkAnswer(QString answer);

    static std::string getServer(MainWindow window);
    static std::string getPublicKeyPath(MainWindow window);
    static std::string getPrivateKeyPath(MainWindow window);
    static std::string getNick(MainWindow window);
};

#endif // ADAPTER_H
