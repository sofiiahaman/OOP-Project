#include "UserRepository.h"
#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

UserRepository::UserRepository(Database& database)
    : db(database)
{
}

bool UserRepository::addStudent(const QString& name, const QString& email)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());
    query.prepare("INSERT INTO students (name, email) VALUES (:name, :email)");
    query.bindValue(":name", name);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Add student error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool UserRepository::updateStudent(int id,
                                   const QString& name,
                                   const QString& email)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());
    query.prepare("UPDATE students SET name = :name, email = :email WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Update student error:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* UserRepository::searchStudents(const QString& name)
{
    if (!db.openConnection())
        return nullptr;

    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query(db.getDb());
    query.prepare("SELECT id, name, email FROM students "
                  "WHERE name LIKE :name");
    query.bindValue(":name", "%" + name + "%");

    if (!query.exec()) {
        qDebug() << "Search error:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(query);
    return model;
}

bool UserRepository::checkLogin(const QString& username,
                                const QString& password)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("SELECT id FROM users "
                  "WHERE username = :u AND password = :p");

    query.bindValue(":u", username);
    query.bindValue(":p", password);

    if (query.exec() && query.next()) {
        return true;
    }

    return false;
}

QString UserRepository::getUserRole(const QString& username)
{
    if (!db.openConnection())
        return "";

    QSqlQuery query(db.getDb());

    query.prepare("SELECT role FROM users WHERE username = :u");
    query.bindValue(":u", username);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

bool UserRepository::userExists(const QString& username)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("SELECT id FROM users WHERE username = :u");
    query.bindValue(":u", username);

    return (query.exec() && query.next());
}

int UserRepository::addUserReturnId(const QString& username,
                                    const QString& password,
                                    const QString& role)
{
    if (!db.openConnection())
        return -1;

    QSqlQuery query(db.getDb());

    query.prepare("INSERT INTO users (username, password, role) "
                  "VALUES (:u, :p, :r)");

    query.bindValue(":u", username);
    query.bindValue(":p", password);
    query.bindValue(":r", role);

    if (!query.exec()) {
        qDebug() << "Add user error:" << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool UserRepository::linkStudentWithUser(int studentId, int userId)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("UPDATE students SET user_id = :u WHERE id = :s");
    query.bindValue(":u", userId);
    query.bindValue(":s", studentId);

    if (!query.exec()) {
        qDebug() << "Link student error:" << query.lastError().text();
        return false;
    }

    return true;
}
