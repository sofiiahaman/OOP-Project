#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Database {
public:
    Database();
    virtual ~Database() = default;
    virtual bool openConnection();
    virtual void closeConnection();
    virtual bool checkLogin(const QString& username, const QString& password);
    virtual QString getUserRole(const QString &username);

    static void setTestDatabase(const QString& path);

    int getBooksCount();
    int getAuthorsCount();
    int getCategoriesCount();
    int getPublishersCount();
    int getStudentsCount();
    int getAvailableBooksCount();
    int getIssuedBooksCount();
    int getReturnedBooksCount();
    int getNotReturnBooksCount();


    virtual bool userExists(const QString &username);
    bool addUser(const QString &username, const QString &password, const QString &role);
    virtual int findStudentByEmail(const QString &email);
    virtual bool studentHasUser(int studentId);
    virtual int addUserReturnId(const QString &username, const QString &password, const QString &role);
    virtual bool linkStudentWithUser(int studentId, int userId);

private:
    QSqlDatabase db;
    static QString testDbPath;
};

#endif // DATABASE_H
