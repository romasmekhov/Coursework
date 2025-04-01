#ifndef CONTACT_H  
#define CONTACT_H
#include <QString>
#include <QVector>
#include <QDate>

class Contact {
public:
    QString firstName;
    QString lastName;
    QString patronymic;
    QString address;
    QDate birthDate;  // Используем QDate вместо строки
    QString email;
    QVector<QString> phoneNumbers;

    //bool validate() const
    static bool validateName(const QString& name);
    static bool validateEmail(const QString& email);
    static bool validatePhoneNumber(const QString& phone);
    static bool validateBirthDate(const QDate& date);  // Принимает QDate

    bool validate() const;
    QString getValidationError() const; // Возвращает текст ошибки или пустую строку


};

#endif // CONTACT_H 
