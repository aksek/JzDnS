#include "obslugakomunikatow.h"

ObslugaKomunikatow::ObslugaKomunikatow(QObject *parent) : QObject(parent)
{
    //utworzyc kolejki
    ObslugaKomunikatowWejsciowych* ptr = new ObslugaKomunikatowWejsciowych();
    ptr->moveToThread(&odbiorKomunikatow);
    odbiorKomunikatow.start();
}

ObslugaKomunikatow::~ObslugaKomunikatow()
{
    odbiorKomunikatow.quit();
    odbiorKomunikatow.wait();
    //zniszczyc kolejki
}

void ObslugaKomunikatow::zaloguj(int serwer, std::string nazwa)
{
    //utworzyc modul wysylajacy w oddzielnym watku
    //utworzyc zaszyfrowany komunikat Login i wysłać (wstawić do kolejki komunikatów)
    //zmienic stan gry
    emit run();
}

void ObslugaKomunikatow::zarejestruj(int serwer, std::string nazwa, std::string nazwaPlikuKlucza)
{
    //utworzyc modul wysylajacy w oddzielnym watku
    //utworzyc zaszyfrowany komunikat Register i wysłać (wstawić do kolejki komunikatów)
    //zmienic stan gry
    emit run();
}

void ObslugaKomunikatow::wyloguj()
{
    //wysłać komunikat i zmienić stan gry, albo tylko zmienić stan gry
    //albo poczekać na potwierdzenie, albo od razu usunąć moduł wysyłający
    emit finish(); //jeżeli potwierdzenie, to nie!
}

void ObslugaKomunikatow::retransmitSlot()
{
    //wysłać ostatni komunikat
}

void ObslugaKomunikatow::odpowiedz(std::string)
{
    //wysłać komunikat, zmienić stan gry
}

void ObslugaKomunikatow::makeConnections(const ObslugaKomunikatowWejsciowych * const _odbiorKomunikatow) const
{
    connect(&odbiorKomunikatow, &QThread::finished, _odbiorKomunikatow, &QObject::deleteLater, Qt::ConnectionType::DirectConnection);
    connect(this, &ObslugaKomunikatow::run, _odbiorKomunikatow, &ObslugaKomunikatowWejsciowych::run, Qt::ConnectionType::QueuedConnection);
    connect(this, &ObslugaKomunikatow::finish, _odbiorKomunikatow, &ObslugaKomunikatowWejsciowych::finish, Qt::ConnectionType::QueuedConnection);
    connect(_odbiorKomunikatow, &ObslugaKomunikatowWejsciowych::retransmitSignal, this, &ObslugaKomunikatow::retransmitSlot, Qt::ConnectionType::QueuedConnection);

}
