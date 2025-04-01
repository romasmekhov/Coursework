#ifndef CONTACTMANAGER_H
#define CONTACTMANAGER_H

#include <QObject>
#include <QVector>
#include "Contact.h"
//#include <QtSql/qsqldatabase.h>
//#include <QtSql/qsqlquery.h>
//#include <QtSql/qsqlerror.h>


class ContactManager : public QObject
{
    Q_OBJECT

public:
    explicit ContactManager(QObject* parent = nullptr);

    bool addContact(const Contact& contact, QString* error = nullptr);
    bool removeContact(const QString& email);
    const QVector<Contact>& getContacts() const;
    bool saveToFile(const QString& filename);
    bool loadFromFile(const QString& filename);

    // ����������� ������
    Contact* findContact(const QString& email);
    void sortContacts(int column, bool ascending = true);

signals:
    void contactsChanged();

private:
    QVector<Contact> contacts; // �������� ���� ������
    //QSqlDatabase db; // ��������� ������ ���� ������

    //bool initDatabase(); // ������������� ��
    //bool createTables(); // �������� ������
};

#endif // CONTACTMANAGER_H
