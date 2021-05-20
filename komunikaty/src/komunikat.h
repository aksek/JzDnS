#pragma once

#include "ArduinoJson-v6.18.0.h"

#include <string>
#include <vector>
#include <variant>
#include <utility>

enum class RodzajKomunikatu{
	Problem, Correct, Round_over, Retransmit, Login, Solution, Problems, New_problem, Delete_problem, Edit_problem
};

class Komunikat{
	
public:
	Komunikat(RodzajKomunikatu rodzajKomunikatu);
	
	virtual std::string serialize();
	
	std::string serializeEnum();
	
	void setRozmiarTresci(int rozmiarTresci);
	void setSumaKontrolna(int sumaKontrolna);
	
	int getRozmiarTresci();
	int getSumaKontrolna();
	
protected:
	RodzajKomunikatu _rodzajKomunikatu;
	int _rozmiarTresci;
	int _sumaKontrolna;
};

class KomunikatString: public Komunikat{
	
public:
	KomunikatString(RodzajKomunikatu rodzajKomunikatu);
	virtual std::string serialize() override;
	
	void setTresc(std::string tresc);
	std::string getTresc();

private:
	std::string _tresc;
};

class KomunikatInt: public Komunikat{

public:
	KomunikatInt(RodzajKomunikatu rodzajKomunikatu);
	virtual std::string serialize() override;

	void setTresc(int tresc);
	int getTresc();

private:
	int _tresc;
};

class KomunikatBool: public Komunikat{

public:
	KomunikatBool(RodzajKomunikatu rodzajKomunikatu);
	virtual std::string serialize() override;

	void setTresc(bool tresc);
	bool getTresc();

private:
	bool _tresc;
};

class KomunikatPairStringString: public Komunikat{

public:
	KomunikatPairStringString(RodzajKomunikatu rodzajKomunikatu);
	virtual std::string serialize() override;

	void setTresc(std::pair<std::string, std::string> tresc);
	std::pair<std::string, std::string> getTresc();

private:
	std::pair<std::string, std::string> _tresc;
};

class KomunikatPairIntString: public Komunikat{

public:
	KomunikatPairIntString(RodzajKomunikatu rodzajKomunikatu);
	virtual std::string serialize() override;

	void setTresc(std::pair<int, std::string> tresc);
	std::pair<int, std::string> getTresc();

private:
	std::pair<int, std::string> _tresc;
};

class KomunikatVector: public Komunikat{

public:
	KomunikatVector(RodzajKomunikatu rodzajKomunikatu);
	virtual std::string serialize() override;

	void setTresc(std::vector< std::pair<std::string, std::string> > tresc);
	std::vector< std::pair<std::string, std::string> > getTresc();

private:
	std::vector< std::pair<std::string, std::string> > _tresc;
};

class Komunikaty{
	
public:
	Komunikaty();
	Komunikat* createKomunikat(RodzajKomunikatu rodzajKomunikatu);
	Komunikat* deserialize(std::string);
	std::string serialize(Komunikat* komunikat);
};