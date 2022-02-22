#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <ctime>
#include <cmath>
#include <cassert>

using namespace std;


class User{
public:
	string name;
	string email;
	string password;
	User* nast;

	User() :nast(nullptr) {
		string n, e, p;
		cout << "Prosze podac email: ";
		cin >> e;
		email = e;

		cout << "Prosze podac nazwe uzytkownika: ";
		cin >> n;
		name = n;

		cout << "Prosze podac haslo: ";
		cin >> p;
		password = p;
	}

	~User() {
		delete nast;
	}

	void login() {
		string li, pas;
		cout << "Prosze podac nazwe uzytkownika lub email: ";
		cin >> li;
		cout << "Prosze podac haslo: ";
		cin >> pas;
		if ((li != name && li != email)) {
			throw invalid_argument("Nie ma takiego uzytkownika.");
		}
		else {
			if (pas != password) {
				throw invalid_argument("Niepoprawne haslo.");
			}
			else {
				cout << "Poprawnie zalogowano." << endl;
			}
		}
	}

};

//Klasa w ktorej zapisana jest lista obiektow
class Stos {
public:
	User* head;

	Stos() : head(nullptr) {};

	~Stos() {
		delete head;
	}

	void check_data(User* u, User* wsk) {
		if (u->email == wsk->email) {
			throw invalid_argument("Adres email juz raz zostal wykorzystany");
		}
		else if (u->name == wsk->name) {
			throw invalid_argument("Nazwa uzytkownika jest juz zajeta");
		}
	}

	void add_user() {
		User* u = new User();
		if (head == nullptr) {
			head = u;
		}
		else {
			User* wsk = head;
			check_data(u, wsk);

			while (wsk->nast != nullptr) {
				check_data(u, wsk);
				wsk = wsk->nast;
			}
			wsk->nast = u;
		}

	}

	//Iterator sluzacy do przeszukiwania stosu
	class interator {
		Stos& s;
		User* index;
	public:
		interator(Stos& is) : s(is), index(is.head) {};
		interator(Stos& is, bool) : s(is) {
			User* wsk = s.head;
			while (wsk->nast != NULL) {
				wsk = wsk->nast;
			}
			index = wsk;
		}
		User* operator++() {
			index = index->nast;
			return index;
		}
		User* operator++(int) {
			User* w = index;
			index = index->nast;
			return w;
		}
		User* operator*() const {
			return index;
		}
		interator& operator=(const interator& rv) {
			s = rv.s;
			index = rv.index;
			return *this;
		}
		bool operator!=(const interator& rv) {
			if (index != rv.index)
				return true;
			return false;
		}
	};

	interator begin() { return interator(*this); }
	interator end() { return interator(*this, true); }
	friend class interator;
};


//Funkcja sprawdzajaca dane logowania
bool check_user(string id, User* pom) {
	if (id == pom->email || id == pom->name) {
		cout << "Znaleziono uzytkownika" << endl;
		for (int i = 3; i >= 0; --i) {
			cout << "Haslo: ";
			string pass;
			cin >> pass;
			if (pass == pom->password) {
				cout << "Poprawnie zalogowano." << endl;
				return true;
			}
			else if (i != 0) {
				cout << "Niepoprawne haslo. Pozostale proby:  " << i << endl;
			}
		}
		throw invalid_argument("Niepoprawne haslo.");
	}
	return false;
}

//Funkcja przy pomocy iteratora przeszukuje stos (Zakomentowane linijki kodu sluza do debugowania)
template<class T>
bool find_user(T itbeg, T itend, string id) {

	//Zmienne pomocnicze
	bool flaga = false;
	User* pom = *itbeg;

	while (itbeg != itend) {
		flaga = check_user(id, pom);
		
		itbeg++;
		pom = *itbeg;
	}

	pom = *itbeg;
	flaga = check_user(id, pom);

	if (!flaga) {
		throw invalid_argument("Nie znaleziono uzytkownika.");
	}

	return false;
}


int main() {
	Stos users_list;

	try {
		while (true)
		{
			cout << "Zarejestruj sie - wpisz 0" << endl;
			cout << "Zaloguj sie - wpisz 1" << endl;
			cout << "Zakoncz dzialanie programu - wypisz 2" << endl << endl;
			cout << "Wpisz: ";
			int i;
			cin >> i;

			//Sprawdzam poprawnosc wprowadzanych danych
			if (!cin.good()) {
				throw invalid_argument("Nie mozna wykonac polecenia.");
			}
			

			if (i == 0) {
				users_list.add_user();
			}
			else if (i == 1) {
				if (users_list.head == nullptr) {
					throw invalid_argument("Lista uzytkownikow jest pusta.");
				}
				cout << "Podaj nazwe uzytkownika lub adres email." << endl;
				cout << "Login: ";
				string login;
				cin >> login;
				find_user(users_list.begin(), users_list.end(), login);
			}
			else if (i == 2)
			{
				break;
			}
			else {
				throw invalid_argument("Wprowadzono niepoprawne dane.");
			}
			system("pause");
			system("cls");
		}

	}
	catch (invalid_argument& e) {
		cout << "Blad logowania: " << e.what() << endl;
	}

	cout << "Koniec programu";
}