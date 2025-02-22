#include "Contact.h"
#include <regex>
#include <ctime>
#include <algorithm>

// ��������� �������������� ��� localtime
#pragma warning(disable : 4996)

bool Contact::validateName(const std::string& name) {
    // �������� �������� �� �����
    std::string trimmed = name;
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));

    const std::regex pattern(R"(^[A-Za-z�-��-���][A-Za-z�-��-���0-9\s-]*[A-Za-z�-��-���0-9]$)");
    return !trimmed.empty() && std::regex_match(trimmed, pattern);
}

bool Contact::validateEmail(const std::string& email) {
    // �������� ���� ��������
    std::string trimmed = email;
    trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), ' '), trimmed.end());

    const std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(trimmed, pattern);
}

bool Contact::validatePhoneNumber(const std::string& phone) {
    const std::regex pattern(R"(^(\+7|8)[\s(-]*\d{3}[)\s-]*\d{3}[\s-]?\d{2}[\s-]?\d{2}$)");
    return std::regex_match(phone, pattern);
}

bool Contact::validateBirthDate(const std::string& date) {
    const std::regex pattern(R"(^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-(\d{4})$)");
    std::smatch match;
    if (!std::regex_match(date, match, pattern)) return false;

    int day = std::stoi(match[1]);
    int month = std::stoi(match[2]);
    int year = std::stoi(match[3]);

    // �������� �� ������� ����
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    if (year > (now->tm_year + 1900)) return false;
    if (year == (now->tm_year + 1900) && month > (now->tm_mon + 1)) return false;
    if (year == (now->tm_year + 1900) && month == (now->tm_mon + 1) && day > now->tm_mday) return false;

    // �������� ���� � ������
    static const int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int maxDays = daysInMonth[month - 1];
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && isLeap) maxDays = 29;

    return day <= maxDays;
}

bool Contact::validate() const {
    return validateName(firstName) &&
        validateName(lastName) &&
        validateEmail(email) &&
        validateBirthDate(birthDate) &&
        std::all_of(phoneNumbers.begin(), phoneNumbers.end(), validatePhoneNumber);
}