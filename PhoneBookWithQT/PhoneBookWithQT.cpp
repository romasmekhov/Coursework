#include "PhoneBookWithQT.h"
#include "ui_PhoneBookWithQT.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QCheckBox> 
#include <QHeaderView> 


PhoneBookWithQT::PhoneBookWithQT(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::PhoneBookWithQTClass)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->contactsTable->setColumnCount(7);
    QStringList headers;
    headers << "First Name" << "Last Name" << "Patronymic"
        << "Phone" << "Address" << "Email" << "Birth Date";
    ui->contactsTable->setHorizontalHeaderLabels(headers);
    ui->contactsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->contactsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Добавляем кнопку сортировки (новый код)
    QPushButton* sortButton = new QPushButton("Sort (A-Z)", this);
    connect(sortButton, &QPushButton::clicked, this, &PhoneBookWithQT::on_sortButton_clicked);
    ui->mainToolBar->addWidget(sortButton);  // Добавляем на главный тулбар

    connect(ui->contactsTable->horizontalHeader(), &QHeaderView::sectionClicked,
        this, &PhoneBookWithQT::on_header_clicked);

    // Загрузка данных при старте (без сообщения об ошибке если файла нет)
    QFile file("contacts.csv");
    if (file.exists()) {
        manager.loadFromFile("contacts.csv");
    }
    updateTable();
}
PhoneBookWithQT::~PhoneBookWithQT()
{
    delete ui;
}

void PhoneBookWithQT::updateTable()
{
    ui->contactsTable->setRowCount(0);
    const QVector<Contact>& contacts = manager.getContacts();
    for (const Contact& contact : contacts) {
        int row = ui->contactsTable->rowCount();
        ui->contactsTable->insertRow(row);

        ui->contactsTable->setItem(row, 0, new QTableWidgetItem(contact.firstName));
        ui->contactsTable->setItem(row, 1, new QTableWidgetItem(contact.lastName));
        ui->contactsTable->setItem(row, 2, new QTableWidgetItem(contact.patronymic));

        QString phones = QStringList::fromVector(contact.phoneNumbers).join(" ");
        ui->contactsTable->setItem(row, 3, new QTableWidgetItem(phones));

        ui->contactsTable->setItem(row, 4, new QTableWidgetItem(contact.address));
        ui->contactsTable->setItem(row, 5, new QTableWidgetItem(contact.email));
        ui->contactsTable->setItem(row, 6, new QTableWidgetItem(contact.birthDate.toString("dd.MM.yyyy")));
    }
}

Contact PhoneBookWithQT::createContactForAdd() const {
    Contact contact;

    QDialog dialog(nullptr);
    dialog.setWindowTitle("Add Contact");
    dialog.setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
    QFormLayout* formLayout = new QFormLayout();

    // Поля ввода
    QLineEdit* firstNameEdit = new QLineEdit(&dialog);
    QLineEdit* lastNameEdit = new QLineEdit(&dialog);
    QLineEdit* patronymicEdit = new QLineEdit(&dialog);
    QLineEdit* phoneEdit = new QLineEdit(&dialog);
    QLineEdit* addressEdit = new QLineEdit(&dialog);
    QLineEdit* emailEdit = new QLineEdit(&dialog);
    QDateEdit* birthDateEdit = new QDateEdit(QDate::currentDate(), &dialog);
    birthDateEdit->setDisplayFormat("dd.MM.yyyy");

    // Добавляем поля в форму
    formLayout->addRow("First Name:", firstNameEdit);
    formLayout->addRow("Last Name:", lastNameEdit);
    formLayout->addRow("Patronymic:", patronymicEdit);
    formLayout->addRow("Phone:", phoneEdit);
    formLayout->addRow("Address:", addressEdit);
    formLayout->addRow("Email:", emailEdit);
    formLayout->addRow("Birth Date:", birthDateEdit);

    mainLayout->addLayout(formLayout);

    // Кнопки OK/Cancel
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        &dialog
    );
    mainLayout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        contact.firstName = firstNameEdit->text().trimmed();
        contact.lastName = lastNameEdit->text().trimmed();
        contact.patronymic = patronymicEdit->text().trimmed();

        QStringList phones = phoneEdit->text().trimmed().split(' ', QString::SkipEmptyParts);
        contact.phoneNumbers = phones.toVector();

        contact.address = addressEdit->text().trimmed();
        contact.email = emailEdit->text().trimmed();
        contact.birthDate = birthDateEdit->date();
    }

    return contact;
}


Contact PhoneBookWithQT::createContactForEdit(const Contact& currentContact, QVector<bool>& fieldsToEdit) const {
    Contact contact = currentContact;

    QDialog dialog(nullptr);
    dialog.setWindowTitle("Edit Contact");
    dialog.setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
    QFormLayout* formLayout = new QFormLayout();

    // Поля ввода + чекбоксы
    QLineEdit* firstNameEdit = new QLineEdit(contact.firstName, &dialog);
    QCheckBox* firstNameCheck = new QCheckBox("Edit?", &dialog);

    QLineEdit* lastNameEdit = new QLineEdit(contact.lastName, &dialog);
    QCheckBox* lastNameCheck = new QCheckBox("Edit?", &dialog);

    QLineEdit* patronymicEdit = new QLineEdit(contact.patronymic, &dialog);
    QCheckBox* patronymicCheck = new QCheckBox("Edit?", &dialog);

    QLineEdit* phoneEdit = new QLineEdit(QStringList::fromVector(contact.phoneNumbers).join(" "), &dialog);
    QCheckBox* phoneCheck = new QCheckBox("Edit?", &dialog);

    QLineEdit* addressEdit = new QLineEdit(contact.address, &dialog);
    QCheckBox* addressCheck = new QCheckBox("Edit?", &dialog);

    QLineEdit* emailEdit = new QLineEdit(contact.email, &dialog);
    QCheckBox* emailCheck = new QCheckBox("Edit?", &dialog);

    QDateEdit* birthDateEdit = new QDateEdit(contact.birthDate, &dialog);
    birthDateEdit->setDisplayFormat("dd.MM.yyyy");
    QCheckBox* birthDateCheck = new QCheckBox("Edit?", &dialog);

    // Добавляем поля и чекбоксы в форму
    auto addFieldWithCheckbox = [&](const QString& label, QWidget* input, QCheckBox* checkbox) {
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(input);
        layout->addWidget(checkbox);
        formLayout->addRow(label + ":", layout);
        };

    addFieldWithCheckbox("First Name", firstNameEdit, firstNameCheck);
    addFieldWithCheckbox("Last Name", lastNameEdit, lastNameCheck);
    addFieldWithCheckbox("Patronymic", patronymicEdit, patronymicCheck);
    addFieldWithCheckbox("Phone", phoneEdit, phoneCheck);
    addFieldWithCheckbox("Address", addressEdit, addressCheck);
    addFieldWithCheckbox("Email", emailEdit, emailCheck);
    addFieldWithCheckbox("Birth Date", birthDateEdit, birthDateCheck);

    mainLayout->addLayout(formLayout);

    // Кнопки OK/Cancel
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        &dialog
    );
    mainLayout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        fieldsToEdit = {
            firstNameCheck->isChecked(),
            lastNameCheck->isChecked(),
            patronymicCheck->isChecked(),
            phoneCheck->isChecked(),
            addressCheck->isChecked(),
            emailCheck->isChecked(),
            birthDateCheck->isChecked()
        };

        if (fieldsToEdit[0]) contact.firstName = firstNameEdit->text().trimmed();
        if (fieldsToEdit[1]) contact.lastName = lastNameEdit->text().trimmed();
        if (fieldsToEdit[2]) contact.patronymic = patronymicEdit->text().trimmed();
        if (fieldsToEdit[3]) {
            QStringList phones = phoneEdit->text().trimmed().split(' ', QString::SkipEmptyParts);
            contact.phoneNumbers = phones.toVector();
        }
        if (fieldsToEdit[4]) contact.address = addressEdit->text().trimmed();
        if (fieldsToEdit[5]) contact.email = emailEdit->text().trimmed();
        if (fieldsToEdit[6]) contact.birthDate = birthDateEdit->date();
    }

    return contact;
}
void PhoneBookWithQT::on_addButton_clicked()
{
    Contact contact = createContactForAdd();

    QString error = contact.getValidationError();
    if (!error.isEmpty()) {
        QMessageBox::warning(this, "Error", error);
        return;
    }

    manager.addContact(contact);
    updateTable();
}


void PhoneBookWithQT::on_editButton_clicked()
{
    int row = ui->contactsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Choose contact for edit");
        return;
    }

    QString email = ui->contactsTable->item(row, 5)->text();
    if (Contact* contact = manager.findContact(email)) {
        QVector<bool> fieldsToEdit;
        Contact editedContact = createContactForEdit(*contact, fieldsToEdit);

        QString error = editedContact.getValidationError();
        if (!error.isEmpty()) {
            QMessageBox::warning(this, "Error", error);
            return;
        }

        *contact = editedContact;
        updateTable();
    }
}

void PhoneBookWithQT::on_deleteButton_clicked()
{
    int row = ui->contactsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Choose contact to remove");
        return;
    }

    QString email = ui->contactsTable->item(row, 5)->text();
    if (QMessageBox::question(this, "Confirmation", "Remove contact?") == QMessageBox::Yes) {
        manager.removeContact(email);
        updateTable();
    }
}
//void PhoneBookWithQT::on_deleteButton_clicked()
//{
//    int row = ui->contactsTable->currentRow();
//    if (row < 0) {
//        QMessageBox::warning(this, "Error", "Please select a contact to delete");
//        return;
//    }
//
//    QString email = ui->contactsTable->item(row, 5)->text();
//    if (QMessageBox::question(this, "Confirm", "Delete this contact?",
//        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
//        if (manager.removeContact(email)) {
//            updateTable();
//        }
//    }
//}

void PhoneBookWithQT::on_searchButton_clicked()
{
    QString searchText = QInputDialog::getText(this, "Search", "Enter search term:");
    if (searchText.isEmpty()) return;

    for (int i = 0; i < ui->contactsTable->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < ui->contactsTable->columnCount(); ++j) {
            if (ui->contactsTable->item(i, j)->text().contains(searchText, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->contactsTable->setRowHidden(i, !match);
    }
}

void PhoneBookWithQT::on_sortButton_clicked()
{
    static bool ascending = true;  // Состояние сортировки (статическая переменная)

    // Сортируем контакты
    manager.sortContacts(ascending);
    updateTable();

    // Меняем текст кнопки
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        ascending = !ascending;  // Инвертируем направление сортировки
        button->setText(ascending ? "Sort (A-Z)" : "Sort (Z-A)");
    }
}

void PhoneBookWithQT::on_loadButton_clicked()
{
    if (manager.loadFromFile("contacts.csv")) {
        updateTable();
        QMessageBox::information(this, "Success", "Contacts loaded successfully");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to load contacts");
    }
}


void PhoneBookWithQT::on_saveButton_clicked()
{
    if (manager.saveToFile("contacts.csv")) {
        QMessageBox::information(this, "Success", "Contacts saved successfully");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save contacts");
    }
}

void PhoneBookWithQT::on_header_clicked(int column) {
    static bool ascending = true;
    manager.sortContacts(column, ascending);
    updateTable();
    ascending = !ascending;
}
