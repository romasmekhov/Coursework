#include "Contact.h"
#include <regex>
#include <ctime>
#include <algorithm>
#include <QRegularExpression>

// ��������� �������������� ��� localtime
#pragma warning(disable : 4996)

QString Contact::getValidationError() const {
    // �������� �����
    if (firstName.trimmed().isEmpty()) {
        return "First name cannot be empty";
    }
    if (firstName.startsWith('-') || firstName.endsWith('-')) {
        return "First name cannot start or end with hyphen";
    }

    // �������� �������
    if (lastName.trimmed().isEmpty()) {
        return "Last name cannot be empty";
    }
    if (lastName.startsWith('-') || lastName.endsWith('-')) {
        return "Last name cannot start or end with hyphen";
    }


    // �������� ��������
    if (patronymic.startsWith('-') || patronymic.endsWith('-')) {
        return "Patronymic cannot start or end with hyphen";
    }

    // �������� email
    if (!validateEmail(email)) {
        return "Invalid email format (example: user@example.com)";
    }

    // �������� ���������
    for (const QString& phone : phoneNumbers) {
        if (!phone.trimmed().isEmpty() && !validatePhoneNumber(phone)) {
            return "Invalid phone format (examples: +79161234567 or 8(916)123-45-67)";
        }
    }

    // �������� ����
    if (!validateBirthDate(birthDate)) {
        return "Invalid birth date (must be valid and not in future)";
    }

    return ""; // ��� ������
}

// ��������������� ������ (��������� ��� ����, �� ����� ��������������)
bool Contact::validateName(const QString& name) {
    QRegularExpression regex(R"(^[A-Za-z�-��-���][A-Za-z�-��-���0-9\s-]*[A-Za-z�-��-���0-9]$)");
    return regex.match(name.trimmed()).hasMatch();
}

bool Contact::validateEmail(const QString& email) {
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email.trimmed()).hasMatch();
}

bool Contact::validatePhoneNumber(const QString& phone) {
    QRegularExpression regex(R"(^(\+7|8)[\s(-]*\d{3}[)\s-]*\d{3}[\s-]?\d{2}[\s-]?\d{2}$)");
    return regex.match(phone).hasMatch();
}

bool Contact::validateBirthDate(const QDate& date) {
    return date.isValid() && date < QDate::currentDate();
}