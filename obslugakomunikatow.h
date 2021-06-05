#ifndef OBSLUGAKOMUNIKATOW_H
#define OBSLUGAKOMUNIKATOW_H

#include <QObject>
#include <QThread>

#include "obslugakomunikatowwejsciowych.h"

class ObslugaKomunikatow : public QObject
{
    Q_OBJECT
public:
    explicit ObslugaKomunikatow(QObject *parent = nullptr);
    ~ObslugaKomunikatow();

public slots:
    // logowanie na podany serwer -> utworzenie modułu i wysłanie komunikatu
    void zaloguj(int serwer, std::string nazwa);
    // rejestrowanie na podany serwer -> utworzenie modułu i wysłanie komunikatu
    void zarejestruj(int serwer, std::string nazwa, std::string nazwaPlikuKlucza);
    // wylogowanie -> wysłać komunikat i usunąć moduł wysyłający komunikaty (albo po otrzymaniu potwierdzenia)
    void wyloguj();
    // wyslij ostatni komunikat
    void retransmitSlot();

    //GRACZ
    // odpowiedz na zagdke
    void odpowiedz(std::string);


signals:
    //uruchomienie i zatrzymanie petli odbioru komunikatu (połączenie wątków)
    void run();
    void finish();
    //czy logowanie było udane (gdy logował się Gracz)
    void logowanieGracz(bool udane);

    //GRACZ
    //serwer dostarczył zagadkę
    void zagadka(std::string tresc);
    //serwer dostarczył informację o rozwiązaniu przez kogoś innego
    void rozwiazanie(std::string nickZwyciescy, std::string odpowiedz);
    //czy roziwazanie bylo poprawne
    void odpowiedzPoprawnosc(bool poprawnosc);

private:
    //uchwyty do kolejek komunikatów
    //ostatni komunikat
    //jeśli będzie trzeba to stan gry(enum - czy była próba logowania/rejestracji, czy była próba rozwiązania zagadki
    QThread odbiorKomunikatow;

    void makeConnections(const ObslugaKomunikatowWejsciowych* const _odbiorKomunikatow) const;

};

#endif // OBSLUGAKOMUNIKATOW_H
