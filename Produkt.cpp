#include "Produkt.h"

Produkt::Produkt(std::string nazwa, double cena, int ilosc) : nazwa(nazwa), cena(cena), ilosc(ilosc) {}

void Produkt::wyswietl() {
	std::cout << "Nazwa: " << nazwa << ", Cena: " << cena << ", Ilosc: " << ilosc << std::endl;
}