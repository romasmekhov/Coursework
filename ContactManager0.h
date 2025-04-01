#ifndef CONTACT_MANAGER0_H
#define CONTACT_MANAGER0_H

#include "Contact0.h"
#include <vector>

class ContactManager {
public:
    bool addContact(const Contact& contact);
    bool removeContact(const std::string& email);
    void displayContacts() const;
    void sortContacts();
    void searchContacts(const std::string& term) const;
    Contact* findContact(const std::string& email);
    bool updateContact(const std::string& email);
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

private:
    std::vector<Contact> contacts;
    bool isEmailUnique(const std::string& email) const;
};

#endif
