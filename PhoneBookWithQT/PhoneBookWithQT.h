#ifndef PHONEBOOKWITHQT_H
#define PHONEBOOKWITHQT_H

#include <QMainWindow>
#include "ContactManager.h"

// Объявление forward declaration класса из UI
QT_BEGIN_NAMESPACE
namespace Ui {
    class PhoneBookWithQTClass;  // <- Изменили имя класса 
}
QT_END_NAMESPACE

class PhoneBookWithQT : public QMainWindow
{
    Q_OBJECT  // Макрос для поддержки сигналов/слотов

public:
    PhoneBookWithQT(QWidget* parent = nullptr);  // Конструктор
    ~PhoneBookWithQT();  // Деструктор

private slots:  // Слоты для кнопок
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_sortButton_clicked();
    void on_searchButton_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_header_clicked(int column);


private:
    Ui::PhoneBookWithQTClass* ui;  // <- Указатель на UI-класс (имя изменено!)
    ContactManager manager;        // Менеджер контактов
    void updateTable();            // Обновление таблицы
    Contact createContactForAdd() const;
    Contact createContactForEdit(const Contact& currentContact, QVector<bool>& fieldsToEdit) const;

};
#endif // PHONEBOOKWITHQT_H