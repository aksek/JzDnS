#ifndef PROBLEM_H
#define PROBLEM_H

#include <QObject>

class Problem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString answer MEMBER m_answer READ answer WRITE setAnswer NOTIFY answerChanged)
    Q_PROPERTY(QString question MEMBER m_question READ question WRITE setQuestion NOTIFY questionChanged)

public:
    explicit Problem(QObject *parent = nullptr);
    explicit Problem(QString name, QString question, QString answer, QObject *parent = nullptr);

    QString name() const;
    QString answer() const;
    QString question() const;

    void setName(const QString &n);
    void setAnswer(const QString &n);
    void setQuestion(const QString &n);

signals:
    void nameChanged(QString);
    void answerChanged(QString);
    void questionChanged(QString);

private:
    QString m_question, m_answer, m_name;
};

#endif // PROBLEM_H
