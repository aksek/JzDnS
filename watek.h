#ifndef WATEK_H
#define WATEK_H

#include <QObject>

class Watek : public QObject
{
    Q_OBJECT
public:
    explicit Watek(QObject *parent = nullptr);

public slots:
    void run(){
        for(int i = 0; i<100; ++i)
            ;
        emit finish();
    }

signals:
    void finish();

};

#endif // WATEK_H
