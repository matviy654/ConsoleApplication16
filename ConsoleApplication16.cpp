#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
using namespace std;

class Contact {
private:
    char* fullName;
    string homePhone;
    string workPhone;
    string mobilePhone;
    string additionalInfo;

public:
    
    Contact(const char* name, const string& home, const string& work, const string& mobile, const string& info) {
        fullName = new char[strlen(name) + 1];
        strcpy(fullName, name);
        homePhone = home;
        workPhone = work;
        mobilePhone = mobile;
        additionalInfo = info;
    }

    Contact(const Contact& other) {
        fullName = new char[strlen(other.fullName) + 1];
        strcpy(fullName, other.fullName);
        homePhone = other.homePhone;
        workPhone = other.workPhone;
        mobilePhone = other.mobilePhone;
        additionalInfo = other.additionalInfo;
    }

    Contact& operator=(const Contact& other) {
        if (this == &other)
            return *this;

        delete[] fullName;

        fullName = new char[strlen(other.fullName) + 1];
        strcpy(fullName, other.fullName);
        homePhone = other.homePhone;
        workPhone = other.workPhone;
        mobilePhone = other.mobilePhone;
        additionalInfo = other.additionalInfo;

        return *this;
    }

    ~Contact() {
        delete[] fullName;
    }

    const char* getFullName() const {
        return fullName;
    }

    void print() const {
        cout << "ПІБ: " << fullName << endl;
        cout << "Домашній телефон: " << homePhone << endl;
        cout << "Робочий телефон: " << workPhone << endl;
        cout << "Мобільний телефон: " << mobilePhone << endl;
        cout << "Додаткова інформація: " << additionalInfo << endl;
    }

    void saveToFile(ofstream& file) const {
        int len = strlen(fullName);
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(fullName, len);
        file << homePhone << endl;
        file << workPhone << endl;
        file << mobilePhone << endl;
        file << additionalInfo << endl;
    }

   
    static Contact loadFromFile(ifstream& file) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* name = new char[len + 1];
        file.read(name, len);
        name[len] = '\0';

        string home, work, mobile, info;
        getline(file, home);
        getline(file, work);
        getline(file, mobile);
        getline(file, info);

        Contact contact(name, home, work, mobile, info);
        delete[] name; 
        return contact;
    }
};

class PhoneBook {
private:
    vector<Contact> contacts;

public:

    void addContact(const Contact& contact) {
        contacts.push_back(contact);
    }


    void removeContact(const char* name) {
        for (auto it = contacts.begin(); it != contacts.end(); ++it) {
            if (strcmp(it->getFullName(), name) == 0) {
                contacts.erase(it);
                cout << "Абонента видалено." << endl;
                return;
            }
        }
        cout << "Абонента не знайдено." << endl;
    }


    void searchContact(const char* name) const {
        for (const auto& contact : contacts) {
            if (strcmp(contact.getFullName(), name) == 0) {
                contact.print();
                return;
            }
        }
        cout << "Абонента не знайдено." << endl;
    }

    void displayAll() const {
        if (contacts.empty()) {
            cout << "Телефонна книга порожня." << endl;
            return;
        }
        for (const auto& contact : contacts) {
            contact.print();
            cout << "-----------------" << endl;
        }
    }


    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::binary);
        if (!file) {
            cout << "Помилка відкриття файлу!" << endl;
            return;
        }
        size_t size = contacts.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        for (const auto& contact : contacts) {
            contact.saveToFile(file);
        }
        cout << "Дані збережено в файл." << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cout << "Помилка відкриття файлу!" << endl;
            return;
        }
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        contacts.clear();
        for (size_t i = 0; i < size; ++i) {
            contacts.push_back(Contact::loadFromFile(file));
        }
        cout << "Дані завантажено з файлу." << endl;
    }
};

int main() {
    PhoneBook phoneBook;
    int choice;
    string filename = "contacts.dat";

    do {
        cout << "1. Додати абонента" << endl;
        cout << "2. Видалити абонента" << endl;
        cout << "3. Шукати абонента" << endl;
        cout << "4. Показати всіх абонентів" << endl;
        cout << "5. Зберегти в файл" << endl;
        cout << "6. Завантажити з файлу" << endl;
        cout << "0. Вийти" << endl;
        cout << "Ваш вибір: ";
        cin >> choice;

        if (choice == 1) {
            char name[100];
            string home, work, mobile, info;

            cout << "Введіть ПІБ: ";
            cin.ignore();
            cin.getline(name, 100);
            cout << "Введіть домашній телефон: ";
            getline(cin, home);
            cout << "Введіть робочий телефон: ";
            getline(cin, work);
            cout << "Введіть мобільний телефон: ";
            getline(cin, mobile);
            cout << "Введіть додаткову інформацію: ";
            getline(cin, info);

            phoneBook.addContact(Contact(name, home, work, mobile, info));
        }
        else if (choice == 2) {
            char name[100];
            cout << "Введіть ПІБ для видалення: ";
            cin.ignore();
            cin.getline(name, 100);
            phoneBook.removeContact(name);
        }
        else if (choice == 3) {
            char name[100];
            cout << "Введіть ПІБ для пошуку: ";
            cin.ignore();
            cin.getline(name, 100);
            phoneBook.searchContact(name);
        }
        else if (choice == 4) {
            phoneBook.displayAll();
        }
        else if (choice == 5) {
            phoneBook.saveToFile(filename);
        }
        else if (choice == 6) {
            phoneBook.loadFromFile(filename);
        }

    } while (choice != 0);

    return 0;
}
