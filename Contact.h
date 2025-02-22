#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>

class Contact {
public:
    std::string firstName;
    std::string lastName;
    std::string middleName;
    std::string address;
    std::string birthDate;
    std::string email;
    std::vector<std::string> phoneNumbers;

    bool validate() const;

private:
    static bool validateName(const std::string& name);
    static bool validateEmail(const std::string& email);
    static bool validatePhoneNumber(const std::string& phone);
    static bool validateBirthDate(const std::string& date);
};

#endif