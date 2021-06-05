#ifndef OBSLUGAKOMUNIKATOWWEJSCIOWYCH_H
#define OBSLUGAKOMUNIKATOWWEJSCIOWYCH_H

#include <QObject>

class ObslugaKomunikatowWejsciowych : public QObject
{
    Q_OBJECT
public:
    explicit ObslugaKomunikatowWejsciowych(QObject *parent = nullptr);
    ~ObslugaKomunikatowWejsciowych();

public slots:
    //uruchom i zakończ petle
    void run();
    void finish();

signals:
    //czy logowanie było udane
    void logowanieGracz(bool udane);
    //pojawienie się komunikatu retransmit
    void retransmitSignal();

    //GRACZ
    //serwer dostarczył zagadkę
    void zagadka(std::string tresc);
    //serwer dostarczył informację o rozwiązaniu przez kogoś innego
    void rozwiazanie(std::string nickZwyciescy, std::string odpowiedz);
    //czy roziwazanie bylo poprawne
    void odpowiedzPoprawnosc(bool poprawnosc);

private:
    //uchwyty do kolejki komunikatów od serwera


};

#endif // OBSLUGAKOMUNIKATOWWEJSCIOWYCH_H
