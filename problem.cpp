#include "problem.h"

Problem::Problem(QObject *parent) : QObject(parent)
{

}

Problem::Problem(QString name, QString question, QString answer, QObject *parent)
{
    m_name = name;
    m_question = question;
    m_answer = answer;

}

QString Problem::name() const { return m_name; }

QString Problem::answer() const { return m_answer; }

QString Problem::question() const { return m_question; }

void Problem::setName(const QString &n)
{
    if (m_name != n) {
        m_name = n;
        emit nameChanged(n);
    }
}

void Problem::setAnswer(const QString &n)
{
    if (m_answer != n) {
        m_answer = n;
        emit answerChanged(n);
    }
}

void Problem::setQuestion(const QString &n)
{
    if (m_question != n) {
        m_question = n;
        emit questionChanged(n);
    }
}
