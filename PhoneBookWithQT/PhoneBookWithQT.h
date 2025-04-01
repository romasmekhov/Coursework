#ifndef PHONEBOOKWITHQT_H
#define PHONEBOOKWITHQT_H

#include <QMainWindow>
#include "ContactManager.h"

// ���������� forward declaration ������ �� UI
QT_BEGIN_NAMESPACE
namespace Ui {
    class PhoneBookWithQTClass;  // <- �������� ��� ������ 
}
QT_END_NAMESPACE

class PhoneBookWithQT : public QMainWindow
{
    Q_OBJECT  // ������ ��� ��������� ��������/������

public:
    PhoneBookWithQT(QWidget* parent = nullptr);  // �����������
    ~PhoneBookWithQT();  // ����������

private slots:  // ����� ��� ������
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_sortButton_clicked();
    void on_searchButton_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_header_clicked(int column);


private:
    Ui::PhoneBookWithQTClass* ui;  // <- ��������� �� UI-����� (��� ��������!)
    ContactManager manager;        // �������� ���������
    void updateTable();            // ���������� �������
    Contact createContactForAdd() const;
    Contact createContactForEdit(const Contact& currentContact, QVector<bool>& fieldsToEdit) const;

};
#endif // PHONEBOOKWITHQT_H