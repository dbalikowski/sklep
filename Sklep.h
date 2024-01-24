#ifndef Sklep_H
#define Sklep_H
#include <map>
#include <set>

#include "Produkt.h"
#include <vector>
#include <string>
#include <iterator>

class Sklep {
public:
    Sklep();
    void dodajKlienta(const std::string& nazwa);
    void wyswietlProdukty();
    void wystawFakture(const std::string& nazwaKlienta, const std::string& nazwaProduktu, int ilosc);
    void ZamowProdukty();
    bool czyKlientIstnieje(const std::string& nazwaKlienta);
    void zarezerwujProdukt(const std::string& nazwaKlienta, const std::string& nazwaProduktu);
    void powiadomKlientow();
    void wczytajRezerwacje();
    void zapiszRezerwacje();
    

private:
    std::vector<Produkt> produkty; // baza produktów 
    typedef std::vector<Produkt>::iterator iterator; // iterator do produktów
    std::map<std::string, std::set<std::string>> rezerwacje; // baza rezerwacji
    void wczytajProdukty(); // wczytanie z pliku
    void zapiszProdukty(); // zapis do pliku
};

#endif // Sklep_H
