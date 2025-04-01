#include "ContactManager0.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>  
#include <sstream> // Äëÿ ðàáîòû ñ stringstream

using namespace std;

// Ôóíêöèÿ äëÿ ïðîâåðêè óíèêàëüíîñòè ïî÷òû
bool ContactManager::isEmailUnique(const std::string& email) const {
    return std::none_of(contacts.begin(), contacts.end(),
        [&](const Contact& c) { return c.email == email; });
}


// Ôóíêöèÿ äîáàâëåíèÿ êîíòàêòà
bool ContactManager::addContact(const Contact& contact) {
    if (contact.validate() && isEmailUnique(contact.email)) {
        contacts.push_back(contact);
        return true;
    }
    else {

        return false;
    }
}

// Ôóíêöèÿ óäàëåíèÿ êîíòàêòà ïî email

bool ContactManager::removeContact(const std::string& email) {
    auto it = std::find_if(contacts.begin(), contacts.end(),
        [&](const Contact& c) { return c.email == email; });

    if (it != contacts.end()) {
        contacts.erase(it);
        return true;
    }
    return false;
}

// Ôóíêöèÿ îòîáðàæåíèÿ âñåõ êîíòàêòîâ
void ContactManager::displayContacts() const {
    if (contacts.empty()) {
        cout << "Contact list is empty.\n";
        return;
    }

    for (const auto& contact : contacts) {
        cout << "First name: " << contact.firstName << "\n";
        cout << "Last name: " << contact.lastName << "\n";
        cout << "Middle name: " << contact.middleName << "\n";
        cout << "Address: " << contact.address << "\n";
        cout << "Birth date: " << contact.birthDate << "\n";
        cout << "Email: " << contact.email << "\n";
        cout << "Phone numbers: ";
        for (const auto& phone : contact.phoneNumbers) {
            cout << phone << " ";
        }
        cout << "\n--------------------------------\n";
    }
}

// Ôóíêöèÿ ñîðòèðîâêè êîíòàêòîâ ïî ôàìèëèè
void ContactManager::sortContacts() {
    sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
        return a.lastName < b.lastName;
        });
}

// Ôóíêöèÿ ïîèñêà êîíòàêòîâ ïî ñòðîêå
void ContactManager::searchContacts(const string& term) const {
    bool found = false;
    for (const auto& contact : contacts) {
        if (contact.firstName.find(term) != string::npos ||
            contact.lastName.find(term) != string::npos ||
            contact.email.find(term) != string::npos) {
            cout << contact.firstName << " " << contact.lastName << " - " << contact.email << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No contacts found.\n";
    }
}

// Ôóíêöèÿ ïîèñêà êîíòàêòà ïî email
Contact* ContactManager::findContact(const string& email) {
    for (auto& contact : contacts) {
        if (contact.email == email) {
            return &contact;
        }
    }
    return nullptr;
}

// Ôóíêöèÿ ðåäàêòèðîâàíèÿ êîíòàêòà
bool ContactManager::updateContact(const string& email) {
    Contact* contact = findContact(email);
    if (!contact) {
        cout << "Contact not found.\n";
        return false;
    }

    string input;

    cout << "Enter new first name (" << contact->firstName << "): ";
    getline(cin, input);
    if (!input.empty()) contact->firstName = input;

    cout << "Enter new last name (" << contact->lastName << "): ";
    getline(cin, input);
    if (!input.empty()) contact->lastName = input;

    cout << "Enter new middle name (" << contact->middleName << "): ";
    getline(cin, input);
    if (!input.empty()) contact->middleName = input;

    cout << "Enter new address (" << contact->address << "): ";
    getline(cin, input);
    if (!input.empty()) contact->address = input;

    cout << "Enter new birth date (" << contact->birthDate << ") [dd-mm-yyyy]: ";
    getline(cin, input);
    if (!input.empty()) contact->birthDate = input;

    cout << "Enter new email (" << contact->email << "): ";
    getline(cin, input);
    if (!input.empty()) contact->email = input;

    cout << "Enter new phone numbers (separate by spaces, finish input with '0'):\n";
    cout << "Current phones: ";
    for (const auto& phone : contact->phoneNumbers) {
        cout << phone << " ";
    }
    cout << "\nNew phones: ";

    contact->phoneNumbers.clear();
    string phone;
    while (cin >> phone) {
        if (phone == "0") break;
        contact->phoneNumbers.push_back(phone);
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Î÷èñòêà áóôåðà

    cout << "Contact updated successfully!\n";
    return true;
}


// Ôóíêöèÿ ñîõðàíåíèÿ êîíòàêòîâ â ôàéë
void ContactManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cout << "Error saving file!\n";
        return;
    }

    for (const auto& contact : contacts) {
        file << "\"" << contact.firstName << "\","
            << "\"" << contact.lastName << "\","
            << "\"" << contact.middleName << "\","
            << "\"" << contact.address << "\","
            << contact.birthDate << ","
            << contact.email << ",";

        for (size_t i = 0; i < contact.phoneNumbers.size(); ++i) {
            file << contact.phoneNumbers[i];
            if (i < contact.phoneNumbers.size() - 1) file << " ";
        }
        file << "\n";
    }
}

// Ôóíêöèÿ çàãðóçêè êîíòàêòîâ èç ôàéëà
void ContactManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "File not found.\n";
        return;
    }

    contacts.clear();
    std::string line;

    while (std::getline(file, line)) {
        Contact contact;
        std::stringstream ss(line);
        std::string field;

        // Ôóíêöèÿ äëÿ ÷òåíèÿ ñòðîê â êàâû÷êàõ
        auto readQuotedField = [](std::stringstream& ss) {
            std::string field;
            if (ss.peek() == '"') {
                ss.get(); // Ïðîïóñêàåì îòêðûâàþùóþ êàâû÷êó
                std::getline(ss, field, '"'); // ×èòàåì äî çàêðûâàþùåé êàâû÷êè
                ss.ignore(); // Ïðîïóñêàåì çàïÿòóþ ïîñëå êàâû÷êè
            }
            else {
                std::getline(ss, field, ',');
            }
            return field;
            };

        // ×òåíèå ïîëåé
        contact.firstName = readQuotedField(ss);
        contact.lastName = readQuotedField(ss);
        contact.middleName = readQuotedField(ss);
        contact.address = readQuotedField(ss);

        // Äàòà ðîæäåíèÿ (áåç êàâû÷åê)
        std::getline(ss, contact.birthDate, ',');

        // Email (áåç êàâû÷åê)
        std::getline(ss, contact.email, ',');

        // Íîìåðà òåëåôîíîâ (îñòàâøàÿñÿ ÷àñòü ñòðîêè)
        std::string phones;
        std::getline(ss, phones);

        // Ðàçáèâàåì íîìåðà òåëåôîíîâ
        std::stringstream phoneStream(phones);
        std::string phone;
        contact.phoneNumbers.clear();
        while (phoneStream >> phone) {
            contact.phoneNumbers.push_back(phone);
        }

        // Ïðîâåðÿåì âàëèäíîñòü ïåðåä äîáàâëåíèåì
        if (contact.validate()) {
            contacts.push_back(contact);
        }
        else {
            std::cout << "Skipped invalid contact: " << contact.email << "\n";
        }
    }
}
