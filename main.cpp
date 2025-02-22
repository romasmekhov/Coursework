#include <iostream>
#include "ContactManager.h"

using namespace std;

// ������� ���������� ���� � ���������� �������� �������
void showMenu() {
    cout << "Phonebook:\n";
    cout << "1. Create contact\n";
    cout << "2. Edit contact\n";
    cout << "3. Delete contact\n";
    cout << "4. Save to file\n";
    cout << "5. Load from file\n";
    cout << "6. Display contacts\n";
    cout << "7. Sort contacts\n";
    cout << "8. Search contacts\n";
    cout << "9. Find contact by email\n";
    cout << "0. Exit\n";
    cout << "Your choice: ";
}

int main() {
    setlocale(LC_ALL, "RU"); // ������������� ������� ������
    ContactManager manager;
    manager.loadFromFile("contacts.csv"); // ��������� �������� ��� ������ ���������

    int choice;
    do {
        showMenu(); // ������� ����
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ����� ����� cin

        switch (choice) {
        case 1: { // �������� ��������
            Contact contact;
            cout << "Enter first name: ";
            getline(cin, contact.firstName);
            cout << "Enter last name: ";
            getline(cin, contact.lastName);
            cout << "Enter middle name: ";
            getline(cin, contact.middleName);
            cout << "Enter address: ";
            getline(cin, contact.address);
            cout << "Enter birth date (dd-mm-yyyy): ";
            getline(cin, contact.birthDate);
            cout << "Enter email: ";
            getline(cin, contact.email);
            cout << "Enter phone numbers (separate by spaces, end with '0'): ";

            string phone;
            while (cin >> phone) { // ������ ������ ���������
                if (phone == "0") break;
                contact.phoneNumbers.push_back(phone);
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ����� ����� ���������

            if (manager.addContact(contact)) {
                cout << "Contact added!\n";
            }
            else {
                cout << "Contact not added. Invalid data or duplicate email!\n";
            }




            break;
        }
        case 2: { // �������������� ��������
            string email;
            cout << "Enter email of the contact to edit: ";
            getline(cin, email);
            if (manager.updateContact(email)) {
                cout << "Contact updated.\n";
            }
            else {
                cout << "Contact not found.\n";
            }
            break;
        }
        case 3: { // �������� ��������
            string email;
            cout << "Enter email of the contact to delete: ";
            getline(cin, email);
            if (manager.removeContact(email))
                cout << "Contact deleted.\n";
            else
                cout << "Contact not found.\n";
            break;
        }
        case 4: // ���������� � ����
            manager.saveToFile("contacts.csv");
            cout << "Contacts saved.\n";
            break;
        case 5: // �������� �� �����
            manager.loadFromFile("contacts.csv");
            cout << "Contacts loaded.\n";
            break;
        case 6: // ����� ������ ���������
            manager.displayContacts();
            break;
        case 7: // ����������
            manager.sortContacts();
            cout << "Contacts sorted.\n";
            break;
        case 8: { // ����� �� ������
            string term;
            cout << "Enter search term: ";
            getline(cin, term);
            manager.searchContacts(term);
            break;
        }
        case 9: { // ����� �� email
            string email;
            cout << "Enter email of the contact to find: ";
            getline(cin, email);
            Contact* contact = manager.findContact(email);
            if (contact) {
                cout << "Contact found:\n";
                cout << contact->firstName << " " << contact->lastName << "\n";
            }
            else {
                cout << "Contact not found.\n";
            }
            break;
        }
        case 0:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid input. Try again.\n";
        }
        cout << endl;
    } while (choice != 0);

    return 0;
}
