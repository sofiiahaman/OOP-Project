#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Database {
public:
    Database();
    bool openConnection();
    void closeConnection();
    bool checkLogin(const QString& username, const QString& password);
    QString getUserRole(const QString &username);

    int getBooksCount();
    int getAuthorsCount();
    int getCategoriesCount();
    int getPublishersCount();
    int getIssuedBooksCount();
    int getReturnedBooksCount();
    int getNotReturnBooksCount();

    bool userExists(const QString &username);
    bool addUser(const QString &username, const QString &password, const QString &role);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
