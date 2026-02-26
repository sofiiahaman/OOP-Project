#include "StudentRepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

StudentRepository::StudentRepository(Database& database)
    : db(database)
{
}

bool StudentRepository::addStudent(const QString& name,
                                   const QString& email)
{
    if (!db.openConnection())
        return false;

    if (findStudentByEmail(email) != -1) {
        qDebug() << "Student with this email already exists!";
        return false;
    }

    QSqlQuery query(db.getDb());

    query.prepare("INSERT INTO students (name, email) "
                  "VALUES (:n, :e)");

    query.bindValue(":n", name);
    query.bindValue(":e", email);

    if (!query.exec()) {
        qDebug() << "Add student error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool StudentRepository::updateStudent(int id,
                                      const QString& name,
                                      const QString& email)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("UPDATE students SET name = :n, email = :e "
                  "WHERE id = :id");

    query.bindValue(":n", name);
    query.bindValue(":e", email);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Update student error:" << query.lastError().text();
        return false;
    }

    return true;
}

int StudentRepository::findStudentByEmail(const QString& email)
{
    if (!db.openConnection())
        return -1;

    QSqlQuery query(db.getDb());

    query.prepare("SELECT id FROM students WHERE email = :e");
    query.bindValue(":e", email);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}

QSqlQueryModel* StudentRepository::getAllStudents()
{
    if (!db.openConnection())
        return nullptr;

    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT id, name, email FROM students",
                    db.getDb());

    if (model->lastError().isValid()) {
        qDebug() << "Get students error:" << model->lastError().text();
    }

    return model;
}

bool StudentRepository::studentHasUser(int studentId)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("SELECT user_id FROM students WHERE id = :id");
    query.bindValue(":id", studentId);

    if (query.exec() && query.next()) {
        return !query.value(0).isNull();
    }

    return false;
}

int StudentRepository::findStudentByName(const QString& name)
{
    if (!db.openConnection())
        return -1;

    QSqlQuery query(db.getDb());

    query.prepare("SELECT id FROM students WHERE LOWER(name) LIKE LOWER(:n) LIMIT 1");

    query.bindValue(":n", "%" + name.trimmed() + "%");

    if (query.exec() && query.next())
        return query.value(0).toInt();

    return -1;
}
