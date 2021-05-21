#include <QList>

#include "adapter.h"
#include "problem.h"
#include "mock.h"

Adapter::Adapter()
{

}

bool Adapter::isAdmin(QString publicKeyPath)
{
    return Mock::isAdmin(publicKeyPath.toStdString());
}

QList<QString> Adapter::getServers()
{
    std::vector<std::string> std_servers = Mock::getServers();
    QList<QString> servers;
    for (std::string server : std_servers) {
        servers.push_back(QString::fromStdString(server));
    }
    return servers;
}

bool Adapter::keyIsValid(QString publicKeyPath, QString PrivateKeyPath)
{
    return Mock::keyIsValid(publicKeyPath.toStdString(), PrivateKeyPath.toStdString());
}

QList<Problem *> Adapter::getAllProblems()
{
    QList<Problem *> problems;

    Problem *problem1 = new Problem;
    problem1->setName("Moria riddle");
    problem1->setQuestion("Say friend and come in");
    problem1->setAnswer("Mellon");
    problems.push_back(problem1);

    Problem *problem2 = new Problem;
    problem2->setName("Deep thought");
    problem2->setQuestion("What's the answer to the ultimate question about life, universe and everything?");
    problem2->setAnswer("42");
    problems.push_back(problem2);

    return problems;
}

Problem *Adapter::getCurrentProblem()
{
    return new Problem("Deep thought", "What's the answer to the ultimate question "
                        "about life, universe and everything?", "");
}

void Adapter::saveProblem(Problem *problem)
{

}

bool Adapter::checkAnswer(QString answer)
{
    return Mock::checkAnswer(answer.toStdString());
}

std::string Adapter::getServer(MainWindow window) {
    return window.getServer().toStdString();
}

std::string Adapter::getPublicKeyPath(MainWindow window) {
    return window.getPublicKeyPath().toStdString();
}

std::string Adapter::getPrivateKeyPath(MainWindow window) {
    return window.getPrivateKeyPath().toStdString();
}

std::string Adapter::getNick(MainWindow window) {
    return window.getNick().toStdString();
}
