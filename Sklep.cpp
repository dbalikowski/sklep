#define _CRT_SECURE_NO_WARNINGS
#include "Sklep.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>


// printowanie Tabelki (przy fakturze)
void napiszTabelke(const std::string& product, int quantity, double price, double value) {
    std::cout << "+--------------+-------+-------+-----------+\n";
    std::cout << "| Produkt      | Ilosc | Cena  | Wartosc   |\n";
    std::cout << "+--------------+-------+-------+-----------+\n";
    std::cout << std::setfill(' ');
    std::cout << "| " << std::setw(12) << product;
    std::cout << " | " << std::setw(5) << quantity;
    std::cout << " | " << std::setw(5) << price;
    std::cout << " | " << std::setw(9) << value << " |\n";

    std::cout << "+--------------+-------+-------+-----------+\n";
}

// printowanie aktualnej daty
void napiszDate() {
    std::cout << "Data: ";
    std::time_t aktualnyCzas = std::time(nullptr);
    std::tm* czasLokalny = std::localtime(&aktualnyCzas);
    std::cout << std::setfill('0') << std::setw(2) << czasLokalny->tm_mday << '-'
        << std::setw(2) << czasLokalny->tm_mon + 1 << '-'
        << std::setw(4) << czasLokalny->tm_year + 1900 << std::endl;
}

// zapis wiadomości do logów
void dodajDoLogow(std::string wiadomosc) {
    std::fstream plik("logi.txt", std::ios::app);
    if (plik.is_open()) {
        std::time_t aktualnyCzas = std::time(nullptr);
        std::tm* czasLokalny = std::localtime(&aktualnyCzas);
        plik << std::setfill('0') << std::setw(2) << czasLokalny->tm_mday << '-'
            << std::setw(2) << czasLokalny->tm_mon + 1 << '-' 
            << std::setw(4) << czasLokalny->tm_year + 1900 << ' ' 
            << std::setw(2) << czasLokalny->tm_hour << ':'
            << std::setw(2) << czasLokalny->tm_min << " ";
        plik << wiadomosc << std::endl;
    }
    else {
        std::cerr << "Nie mozna otworzyc pliku z logami" << std::endl;
    }
}

Sklep::Sklep() {
    wczytajProdukty();
    wczytajRezerwacje();
}


void Sklep::wczytajRezerwacje() {
    std::ifstream plik("rezerwacje.txt");
    std::string linia;
    while (std::getline(plik, linia)) {
        std::istringstream iss(linia);
        std::string nazwaProduktu, nazwaKlienta;
        std::getline(iss, nazwaProduktu, ',');
        while (std::getline(iss, nazwaKlienta, ',')) {
            rezerwacje[nazwaProduktu].insert(nazwaKlienta);
        }
    }
    plik.close();
}

void Sklep::zapiszRezerwacje() {
    std::ofstream plik("rezerwacje.txt");
    for (const auto& para : rezerwacje) {
        const auto& produkt = para.first;
        const auto& klienci = para.second;
        if (!klienci.empty()) {
            plik << produkt;
            for (const auto& klient : klienci) {
                plik << "," << klient;
            }
            plik << std::endl;
            
        }
    }
    plik.close();
}



void Sklep::dodajKlienta(const std::string& nazwa) {
    if (czyKlientIstnieje(nazwa)) {
        std::cout << "Klient '" << nazwa << "' juz istnieje w bazie." << std::endl;
        return;
    }

    std::ofstream plik("klienci.txt", std::ios::app);
    if (plik.is_open()) {
        plik << nazwa << "\n";
        plik.close();
        dodajDoLogow("Dodano uzytkownika: " + nazwa);
    } else {
        std::cerr << "Nie mozna otworzyc pliku klienci.txt" << std::endl;
    }
}

void Sklep::wyswietlProdukty() {
    for (iterator it = produkty.begin(); it != produkty.end(); ++it) {
        it->wyswietl();
    }
}

void Sklep::wystawFakture(const std::string& nazwaKlienta, const std::string& nazwaProduktu, int ilosc) {
    if (!czyKlientIstnieje(nazwaKlienta)) {
        std::cout << "Nie mozna wystawic faktury: klient '" << nazwaKlienta << "' nie istnieje w bazie." << std::endl;
        return;
    }

    for (iterator it = produkty.begin(); it != produkty.end(); ++it) {
        if (it->nazwa == nazwaProduktu && it->ilosc >= ilosc) {
            it->ilosc -= ilosc;
            system("cls");
            std::cout << "[=== FAKTURA ===]\n";
            std::cout << "Odbiorca: " << nazwaKlienta << std::endl;
            napiszDate();
            napiszTabelke(it->nazwa, ilosc, it->cena, it->cena * ilosc);

            dodajDoLogow("Wystawiono fakture dla " + nazwaKlienta + " [" + it->nazwa + "*" + std::to_string(ilosc) + "]");
            
            // jeżeli wszystkie sztuki produktu zostaną wyprzedane zostaje złożone zamówienie 
            if (it->ilosc == 0) {
                ZamowProdukty();
                dodajDoLogow("Brak produktu " + it->nazwa + " zamowiono 100 sztuk");
            }
            zapiszProdukty();
            return;
        }
    }
    std::cout << "Produkt niedostepny lub niewystarczajaca ilosc" << std::endl;
}



void Sklep::wczytajProdukty() {
    std::ifstream plik("produkty.txt");
    if (plik.is_open()) {
        std::string linia;
        while (std::getline(plik, linia)) {
            std::istringstream iss(linia);
            std::string nazwa;
            std::string cenatxt;
            std::string ilosctxt;
            std::getline(iss, nazwa, ',');
            std::getline(iss, cenatxt, ',');
            std::getline(iss, ilosctxt);

            try {
                double cena = std::stod(cenatxt);
                int ilosc = std::stoi(ilosctxt);
                produkty.emplace_back(nazwa, cena, ilosc);
            }
            catch (const std::exception& e) {
                std::cerr << "Blad podczas konwersji danych: " << e.what() << std::endl;
            }
        }
        plik.close();
    }
    else {
        std::cerr << "Nie mozna otworzyc pliku produkty.txt" << std::endl;
    }
}


void Sklep::zapiszProdukty() {
    std::ofstream plik("produkty.txt");
    if (plik.is_open()) {
        for (iterator it = produkty.begin(); it != produkty.end(); ++it) {
            plik << it->nazwa << "," << it->cena << "," << it->ilosc << std::endl;
        }
        plik.close();
    } else {
        std::cerr << "Nie mozna zapisac pliku produkty.txt" << std::endl;
    }
}


void Sklep::ZamowProdukty() {
    bool zmiany = false;
 
    for (iterator it = produkty.begin(); it != produkty.end(); ++it) {
        if (it->ilosc == 0) {
            it->ilosc += 100;
            zmiany = true;
            std::cout << "Zamowiono do sklepu 100 sztuk produktu: " << it->nazwa << std::endl;
        }
    }

    if (zmiany) {
        zapiszProdukty();
        powiadomKlientow();
        zapiszRezerwacje();
    }
}

bool Sklep::czyKlientIstnieje(const std::string& nazwaKlienta) {
    std::ifstream plik("klienci.txt");
    std::string linia;
    while (std::getline(plik, linia)) {
        if (linia == nazwaKlienta) {
            return true;
        }
    }
    return false;
}

// rezerwuje dany produkt dla danego klienta
void Sklep::zarezerwujProdukt(const std::string& nazwaKlienta, const std::string& nazwaProduktu) {
    for (iterator it = produkty.begin(); it != produkty.end(); ++it) {
        if (it->nazwa == nazwaProduktu) {
            rezerwacje[nazwaProduktu].insert(nazwaKlienta);
            return;
        }
    }
    std::cout << "Produkt " << nazwaProduktu << " nie istnieje." << std::endl;
}

// powiadamia klientow, ze produkt ktory zarezerwowali zostal zamówiony do sklepu
void Sklep::powiadomKlientow() {
    for (iterator it = produkty.begin(); it != produkty.end(); ++it) {
        if (rezerwacje.find(it->nazwa) != rezerwacje.end() && it->ilosc > 0) {
            for (const auto& klient : rezerwacje[it->nazwa]) {
                // Pusta funkcja powiadomień
                std::cout << "Powiadomienie dla klienta " << klient << ": produkt " << it->nazwa << " jest dostepny." << std::endl;
                dodajDoLogow("Wyslano powiadomienie dla klienta: " + klient + "[Dostepnosc: " + it->nazwa + "]");
            }
            rezerwacje[it->nazwa].clear(); // Usunięcie rezerwacji po powiadomieniu
        }
    }
}
