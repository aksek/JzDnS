#ifndef ADAPTER_H
#define ADAPTER_H

#include <QObject>


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
};

#endif // ADAPTER_H
