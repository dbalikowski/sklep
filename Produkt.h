#ifndef PRODUKT_H
#define PRODUKT_H

#include <string>
#include <iostream>

class Produkt {
public:
    std::string nazwa;
    double cena;
    int ilosc;

    Produkt(std::string nazwa, double cena, int ilosc);
    void wyswietl();
};

#endif // PRODUKT_H
