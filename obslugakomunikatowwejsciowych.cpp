#include "obslugakomunikatowwejsciowych.h"

ObslugaKomunikatowWejsciowych::ObslugaKomunikatowWejsciowych(QObject *parent) : QObject(parent)
{
    //otworz kolejke
}

ObslugaKomunikatowWejsciowych::~ObslugaKomunikatowWejsciowych()
{
    //zamknij kolejke
}

void ObslugaKomunikatowWejsciowych::run()
{
    //oczekuj na komunikat w kolejce odszyfruj i wyślij odpowiedni signal
}

void ObslugaKomunikatowWejsciowych::finish()
{
    //wykasuj otrzymane komunikaty
}
