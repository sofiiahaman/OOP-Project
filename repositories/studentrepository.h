#ifndef STUDENTREPOSITORY_H
#define STUDENTREPOSITORY_H

#include <QString>
#include <QSqlQueryModel>
#include "database.h"

class StudentRepository
{
public:
    StudentRepository(Database& database);

    bool addStudent(const QString& name,
                    const QString& email);

    bool updateStudent(int id,
                       const QString& name,
                       const QString& email);

    int findStudentByEmail(const QString& email);

    QSqlQueryModel* getAllStudents();

    bool studentHasUser(int studentId);

    int findStudentByName(const QString& name);

private:
    Database& db;
};

#endif // STUDENTREPOSITORY_H
