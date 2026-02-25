#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>

class Database {
public:
    Database();

    // --- Connection management ---
    bool openConnection();
    void closeConnection();

    // --- Authentication and Users ---
    bool checkLogin(const QString& username, const QString& password);
    bool userExists(const QString &username);
    QString getUserRole(const QString &username);
    int addUserReturnId(const QString &username, const QString &password, const QString &role);

    // --- Students ---
    int findStudentByEmail(const QString &email);
    bool studentHasUser(int studentId);
    bool linkStudentWithUser(int studentId, int userId);

    // --- Book and transaction management ---
    bool addBook(const QString &title,
                 const QString &author,
                 const QString &publisher,
                 const QString &category);
    bool returnBookTransaction(int transactionId, int bookId);

    // --- Dashboard statistics ---
    int getBooksCount();
    int getAuthorsCount();
    int getCategoriesCount();
    int getPublishersCount();
    int getStudentsCount();
    int getAvailableBooksCount();
    int getIssuedBooksCount();
    int getReturnedBooksCount();
    int getNotReturnBooksCount();

private:
    QSqlDatabase db;

    // Helper method for executing queries that return a single scalar value
    int getScalarValue(const QString &sql);
};

#endif // DATABASE_H
