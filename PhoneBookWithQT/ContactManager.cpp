#include "ContactManager.h"
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>
#include <QFile>

#include <QMessageBox>
#include <algorithm>
#include <QDebug> 



//#include <QDebug>
//#include <QtSql/qsqldatabase.h>
//#include <QtSql/qsqlquery.h>
//#include <QtSql/qsqlerror.h>


// В конструкторе ContactManager:


//bool ContactManager::initDatabase()
//{
//    // Настройка подключения к SQLite
//    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("phonebook.db"); // Имя файла БД
//
//    if (!db.open()) {
//        qWarning() << "Database error:" << db.lastError().text();
//        return false;
//    }
//
//    return createTables();
//}
//
//bool ContactManager::createTables()
//{
//    QSqlQuery query;
//    return query.exec(
//        "CREATE TABLE IF NOT EXISTS contacts ("
//        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//        "first_name TEXT NOT NULL,"
//        "last_name TEXT NOT NULL,"
//        "patronymic TEXT,"
//        "address TEXT,"
//        "birth_date TEXT NOT NULL,"
//        "email TEXT UNIQUE NOT NULL"
//        ");"
//    ) && query.exec(
//        "CREATE TABLE IF NOT EXISTS phones ("
//        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//        "contact_id INTEGER NOT NULL,"
//        "number TEXT NOT NULL,"
//        "FOREIGN KEY(contact_id) REFERENCES contacts(id)"
//        ");"
//    );
//}
//bool ContactManager::initDatabase() {
//    // 1. Создаём подключение к SQLite
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("phonebook.db");  // Файл БД будет в папке с программой
//
//    // 2. Открываем БД
//    if (!db.open()) {
//        qCritical() << "Ошибка открытия БД:" << db.lastError().text();
//        return false;
//    }
//
//    // 3. Создаём таблицы (если их нет)
//    QSqlQuery query;
//    query.exec("CREATE TABLE IF NOT EXISTS contacts ("
//        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//        "first_name TEXT NOT NULL, "
//        "last_name TEXT NOT NULL, "
//        "email TEXT UNIQUE NOT NULL)");
//
//    qDebug() << "База данных инициализирована!";
//    return true;
//}

ContactManager::ContactManager(QObject* parent)
    : QObject(parent)
{
}
bool ContactManager::addContact(const Contact& contact, QString* error) {
    QString validationError = contact.getValidationError();
    if (validationError.isEmpty()) {
        contacts.append(contact);
        emit contactsChanged();
        return true;
    }

    if (error) {
        *error = validationError;
    }
    else {
        qWarning() << "Contact validation failed:" << validationError; // Исправлено
    }
    return false;
}

bool ContactManager::removeContact(const QString& email) {
    auto it = std::remove_if(contacts.begin(), contacts.end(),
        [&email](const Contact& c) { return c.email == email; });

    if (it != contacts.end()) {
        contacts.erase(it, contacts.end());
        emit contactsChanged();
        return true;
    }
    return false;
}

const QVector<Contact>& ContactManager::getContacts() const {
    return contacts;
}

Contact* ContactManager::findContact(const QString& email) {
    auto it = std::find_if(contacts.begin(), contacts.end(),
        [&email](const Contact& c) { return c.email == email; });
    return (it != contacts.end()) ? &(*it) : nullptr;
}

void ContactManager::sortContacts(int column, bool ascending) {
    std::sort(contacts.begin(), contacts.end(),
        [column, ascending](const Contact& a, const Contact& b) {
            auto caseInsensitiveCompare = [](const QString& s1, const QString& s2) {
                return QString::compare(s1, s2, Qt::CaseInsensitive);
                };

            switch (column) {
            case 0: // First Name
                return ascending ? caseInsensitiveCompare(a.firstName, b.firstName) < 0
                    : caseInsensitiveCompare(a.firstName, b.firstName) > 0;
            case 1: // Last Name
                return ascending ? caseInsensitiveCompare(a.lastName, b.lastName) < 0
                    : caseInsensitiveCompare(a.lastName, b.lastName) > 0;
            case 2: // Patronymic
                return ascending ? caseInsensitiveCompare(a.patronymic, b.patronymic) < 0
                    : caseInsensitiveCompare(a.patronymic, b.patronymic) > 0;
            case 3: // Phone (сравниваем первый номер)
                if (!a.phoneNumbers.isEmpty() && !b.phoneNumbers.isEmpty()) {
                    return ascending ? caseInsensitiveCompare(a.phoneNumbers.first(), b.phoneNumbers.first()) < 0
                        : caseInsensitiveCompare(a.phoneNumbers.first(), b.phoneNumbers.first()) > 0;
                }
                return false;
            case 5: // Email (регистр может быть важен)
                return ascending ? a.email < b.email : a.email > b.email;
            case 6: // Birth Date
                return ascending ? a.birthDate < b.birthDate : a.birthDate > b.birthDate;
            default:
                return ascending ? caseInsensitiveCompare(a.lastName, b.lastName) < 0
                    : caseInsensitiveCompare(a.lastName, b.lastName) > 0;
            }
        });
    emit contactsChanged();
}

bool ContactManager::saveToFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    for (const Contact& contact : contacts) {
        out << contact.firstName << ","
            << contact.lastName << ","
            << contact.patronymic << ","
            << QStringList::fromVector(contact.phoneNumbers).join(";") << ","
            << contact.address << ","
            << contact.email << ","
            << contact.birthDate.toString("dd.MM.yyyy") << "\n";
    }

    file.close();
    return true;
}


bool ContactManager::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    contacts.clear(); // Очищаем текущие контакты
    QTextStream in(&file);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');

        if (parts.size() >= 7) {
            Contact contact;
            contact.firstName = parts[0];
            contact.lastName = parts[1];
            contact.patronymic = parts[2];
            contact.phoneNumbers = parts[3].split(';', QString::SkipEmptyParts).toVector();
            contact.address = parts[4];
            contact.email = parts[5];
            contact.birthDate = QDate::fromString(parts[6], "dd.MM.yyyy");

            if (contact.birthDate.isValid() && !contact.firstName.isEmpty()) {
                contacts.append(contact);
            }
        }
    }

    file.close();
    return true;

};
