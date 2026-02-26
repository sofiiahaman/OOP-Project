#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QString>
#include <QSqlQueryModel>

class Database;

class UserRepository
{
public:
    explicit UserRepository(Database& database);

    bool addStudent(const QString& name, const QString& email);
    bool updateStudent(int id, const QString& name, const QString& email);
    QSqlQueryModel* searchStudents(const QString& name);

    bool checkLogin(const QString& username,
                    const QString& password);

    QString getUserRole(const QString& username);

    bool userExists(const QString& username);
    int addUserReturnId(const QString& username,
                        const QString& password,
                        const QString& role);

    bool linkStudentWithUser(int studentId, int userId);

private:
    Database& db;
};

#endif // USERREPOSITORY_H
