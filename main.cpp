#include "Sklep.h"
#include <iostream>
#include <limits>

// tryb u¿ytkownika
enum class UserType {
    Admin,
    Client
};

// pobranie informacji w jakim trybie ma dzia³aæ program
UserType login() {
    std::string username, password;
    std::cout << "Podaj login (lub nacisnij Enter, aby kontynuowac jako klient): ";
    std::getline(std::cin, username);

    if (username.empty()) {
        return UserType::Client;
    }

    std::cout << "Podaj haslo: ";
    std::getline(std::cin, password);

    if (username == "admin" && password == "admin") {
        return UserType::Admin;
    }

    return UserType::Client;
}

// obs³uga menu
void menu(Sklep& baza, UserType userType) {
    while (true) {
        std::cout << "\n=================" << std::endl;
        std::cout << "1. Dodaj klienta\n2. Wyswietl produkty\n";
        if (userType == UserType::Admin) {
            std::cout << "3. Wystaw fakture\n4. Zarezerwuj\n";
        }
        else {
            std::cout << "3. Zarezerwuj\n";
        }
        std::cout << "0. Wyjdz\n";

        int wybor;
        std::cin >> wybor;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Czyœci bufor

        switch (wybor) {
        case 1: {
            std::string nazwaKlienta;
            std::cout << "Podaj nazwe klienta: ";
            std::getline(std::cin, nazwaKlienta);
            baza.dodajKlienta(nazwaKlienta);
            break;
        }
        case 2:
            baza.wyswietlProdukty();
            break;
        case 3:
            if (userType == UserType::Admin) {
                std::string nazwaKlienta, nazwaProduktu;
                int ilosc;
                std::cout << "Podaj nazwe klienta: ";
                std::getline(std::cin, nazwaKlienta);
                std::cout << "Podaj nazwe produktu: ";
                std::getline(std::cin, nazwaProduktu);
                std::cout << "Podaj ilosc: ";
                std::cin >> ilosc;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                baza.wystawFakture(nazwaKlienta, nazwaProduktu, ilosc);
            }
            else {
                std::string nazwaKlienta, nazwaProduktu;
                std::cout << "Podaj nazwe klienta: ";
                std::getline(std::cin, nazwaKlienta);
                std::cout << "Podaj nazwe produktu: ";
                std::getline(std::cin, nazwaProduktu);
                baza.zarezerwujProdukt(nazwaKlienta, nazwaProduktu);
                baza.zapiszRezerwacje();
            }
            break;
        case 4:
            if (userType == UserType::Admin) {
                std::string nazwaKlienta, nazwaProduktu;
                std::cout << "Podaj nazwe klienta: ";
                std::getline(std::cin, nazwaKlienta);
                std::cout << "Podaj nazwe produktu: ";
                std::getline(std::cin, nazwaProduktu);
                baza.zarezerwujProdukt(nazwaKlienta, nazwaProduktu);
                baza.zapiszRezerwacje();
            }
            break;
        case 0:
            return;
        default:
            std::cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
            break;
        }
    }
}

int main() {
    Sklep baza;
    UserType userType = login();
    menu(baza, userType);
    return 0;
}
