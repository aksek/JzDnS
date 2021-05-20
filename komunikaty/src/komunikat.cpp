#include "komunikat.h"

#include <cstddef>


//Komunikat
Komunikat::Komunikat(RodzajKomunikatu rodzajKomunikatu): _rodzajKomunikatu(rodzajKomunikatu), _rozmiarTresci(0), _sumaKontrolna(0){
	
}

std::string Komunikat::serialize(){
	std::string trescEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + trescEnum.capacity() + 1;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = trescEnum;
	doc["header"]["size"] = _rozmiarTresci;
	doc["header"]["control"] = _sumaKontrolna;
	
	if(_rodzajKomunikatu==RodzajKomunikatu::Retransmit){
		doc["text"] = "";
	}else throw "niewłaściwy rodzaj Komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

std::string Komunikat::serializeEnum(){
	if(_rodzajKomunikatu==RodzajKomunikatu::Problem)
		return "Problem";
	if(_rodzajKomunikatu==RodzajKomunikatu::Correct)
		return "Correct";
	if(_rodzajKomunikatu==RodzajKomunikatu::Round_over)
		return "Round_over";
	if(_rodzajKomunikatu==RodzajKomunikatu::Retransmit)
		return "Retransmit";
	if(_rodzajKomunikatu==RodzajKomunikatu::Login)
		return "Login";
	if(_rodzajKomunikatu==RodzajKomunikatu::Solution)
		return "Solution";
	if(_rodzajKomunikatu==RodzajKomunikatu::Problems)
		return "Problems";
	if(_rodzajKomunikatu==RodzajKomunikatu::New_problem)
		return "New_problem";
	if(_rodzajKomunikatu==RodzajKomunikatu::Delete_problem)
		return "Delete_problem";
	if(_rodzajKomunikatu==RodzajKomunikatu::Edit_problem)
		return "Edit_problem";
	throw "nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju";
}

void Komunikat::setRozmiarTresci(int rozmiarTresci){
	_rozmiarTresci = rozmiarTresci;
}

void Komunikat::setSumaKontrolna(int sumaKontrolna){
		_sumaKontrolna = sumaKontrolna;
}

int Komunikat::getRozmiarTresci(){
	return _rozmiarTresci;
}

int Komunikat::getSumaKontrolna(){
	return _sumaKontrolna;
}

//KomunikatString: public Komunikat
KomunikatString::KomunikatString(RodzajKomunikatu rodzajKomunikatu): Komunikat(rodzajKomunikatu), _tresc(""){
	if(rodzajKomunikatu!=RodzajKomunikatu::Problem && rodzajKomunikatu!=RodzajKomunikatu::Login && rodzajKomunikatu!=RodzajKomunikatu::Solution)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string KomunikatString::serialize(){
	std::string trescEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + _tresc.capacity() + trescEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = trescEnum;
	doc["header"]["size"] = _rozmiarTresci;
	doc["header"]["control"] = _sumaKontrolna;
	
	if(_rodzajKomunikatu==RodzajKomunikatu::Problem){
		doc["text"]["problem"] = _tresc;
	}else if(_rodzajKomunikatu==RodzajKomunikatu::Login){
		doc["text"]["nick"] = _tresc;
	}else if(_rodzajKomunikatu==RodzajKomunikatu::Solution){
		doc["text"]["solution"] = _tresc;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}
	
void KomunikatString::setTresc(std::string tresc){
	_tresc = tresc;
}

std::string KomunikatString::getTresc(){
	return _tresc;
}

//KomunikatInt: public Komunikat
KomunikatInt::KomunikatInt(RodzajKomunikatu rodzajKomunikatu): Komunikat(rodzajKomunikatu), _tresc(0){
	if(rodzajKomunikatu!=RodzajKomunikatu::Delete_problem)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string KomunikatInt::serialize(){
	std::string trescEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + trescEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = trescEnum;
	doc["header"]["size"] = _rozmiarTresci;
	doc["header"]["control"] = _sumaKontrolna;
	
	if(_rodzajKomunikatu==RodzajKomunikatu::Delete_problem){
		doc["text"]["id"] = _tresc;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void KomunikatInt::setTresc(int tresc){
	_tresc = tresc;
}

int KomunikatInt::getTresc(){
	return _tresc;
}


//KomunikatBool: public Komunikat
KomunikatBool::KomunikatBool(RodzajKomunikatu rodzajKomunikatu): Komunikat(rodzajKomunikatu), _tresc(false){
	if(rodzajKomunikatu!=RodzajKomunikatu::Correct)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string KomunikatBool::serialize(){
	std::string trescEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + trescEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = trescEnum;
	doc["header"]["size"] = _rozmiarTresci;
	doc["header"]["control"] = _sumaKontrolna;
	
	if(_rodzajKomunikatu==RodzajKomunikatu::Correct){
		doc["text"]["correct"] = _tresc;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void KomunikatBool::setTresc(bool tresc){
	_tresc = tresc;
}

bool KomunikatBool::getTresc(){
	return _tresc;
}

//KomunikatPairStringString: public Komunikat
KomunikatPairStringString::KomunikatPairStringString(RodzajKomunikatu rodzajKomunikatu): Komunikat(rodzajKomunikatu){
	if(rodzajKomunikatu!=RodzajKomunikatu::Round_over && rodzajKomunikatu!=RodzajKomunikatu::New_problem)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string KomunikatPairStringString::serialize(){
	std::string trescEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _tresc.first.capacity() + _tresc.second.capacity() + trescEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = trescEnum;
	doc["header"]["size"] = _rozmiarTresci;
	doc["header"]["control"] = _sumaKontrolna;
	
	if(_rodzajKomunikatu==RodzajKomunikatu::Round_over){
		doc["text"]["winner_nick"] = _tresc.first;
		doc["text"]["solution"] = _tresc.second;
	}else if(_rodzajKomunikatu==RodzajKomunikatu::New_problem){
		doc["text"]["problem_content"] = _tresc.first;
		doc["text"]["problem_answer"] = _tresc.second;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void KomunikatPairStringString::setTresc(std::pair<std::string, std::string> tresc){
	_tresc = tresc;
}

std::pair<std::string, std::string> KomunikatPairStringString::getTresc(){
	return _tresc;
}

//KomunikatPairIntString: public Komunikat
KomunikatPairIntString::KomunikatPairIntString(RodzajKomunikatu rodzajKomunikatu): Komunikat(rodzajKomunikatu){
	if(rodzajKomunikatu!=RodzajKomunikatu::Edit_problem)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string KomunikatPairIntString::serialize(){
	std::string trescEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _tresc.second.capacity() + trescEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = trescEnum;
	doc["header"]["size"] = _rozmiarTresci;
	doc["header"]["control"] = _sumaKontrolna;
	
	if(_rodzajKomunikatu==RodzajKomunikatu::Edit_problem){
		doc["text"]["id"] = _tresc.first;
		doc["text"]["changed_content"] = _tresc.second;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void KomunikatPairIntString::setTresc(std::pair<int, std::string> tresc){
	_tresc = tresc;
}

std::pair<int, std::string> KomunikatPairIntString::getTresc(){
	return _tresc;
}

//KomunikatVector: public Komunikat
KomunikatVector::KomunikatVector(RodzajKomunikatu rodzajKomunikatu): Komunikat(rodzajKomunikatu){
	if(rodzajKomunikatu!=RodzajKomunikatu::Problems)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string KomunikatVector::serialize(){
	std::string trescEnum = serializeEnum();
	size_t rozmiarString = 0;
	for(int i=0; i<_tresc.size(); ++i){
		rozmiarString += _tresc[i].first.capacity() + _tresc[i].second.capacity();
	}
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(_tresc.size()) + _tresc.size()*JSON_OBJECT_SIZE(2) + rozmiarString + trescEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = trescEnum;
	doc["header"]["size"] = _rozmiarTresci;
	doc["header"]["control"] = _sumaKontrolna;
	
	if(_rodzajKomunikatu==RodzajKomunikatu::Problems){
		if(_tresc.size()==0){
			JsonArray text = doc.createNestedArray("text");
		}else {
			for(int i=0; i<_tresc.size(); ++i){
				doc["text"][i]["text"] = _tresc[i].first;
				doc["text"][i]["answer"] = _tresc[i].second;
			}
		}
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void KomunikatVector::setTresc(std::vector< std::pair<std::string, std::string> > tresc){
	_tresc = tresc;
}

std::vector< std::pair<std::string, std::string> > KomunikatVector::getTresc(){
	return _tresc;
}

//Komunikaty
Komunikaty::Komunikaty(){
	
}

Komunikat* Komunikaty::createKomunikat(RodzajKomunikatu rodzajKomunikatu){
	if(rodzajKomunikatu==RodzajKomunikatu::Problem)
		return new KomunikatString(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Correct)
		return new KomunikatBool(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Round_over)
		return new KomunikatPairStringString(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Retransmit)
		return new Komunikat(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Login)
		return new KomunikatString(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Solution)
		return new KomunikatString(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Problems)
		return new KomunikatVector(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::New_problem)
		return new KomunikatPairStringString(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Delete_problem)
		return new KomunikatInt(rodzajKomunikatu);
	if(rodzajKomunikatu==RodzajKomunikatu::Edit_problem)
		return new KomunikatPairIntString(rodzajKomunikatu);
	throw "nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju";
}

Komunikat* Komunikaty::deserialize(std::string){
	return nullptr;
}

std::string Komunikaty::serialize(Komunikat* komunikat){
	return komunikat->serialize();
}