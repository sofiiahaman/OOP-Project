#ifndef STUDENTREPOSITORY_H
#define STUDENTREPOSITORY_H

#include <QString>
#include <QSqlQueryModel>
#include "database.h"

/**
 * @class StudentRepository
 * @brief Provides data access operations for managing students.
 *
 * The StudentRepository class encapsulates all database operations
 * related to students such as creation, updates, lookups, and retrieval.
 *
 * It acts as the data access layer between the application logic
 * and the underlying database.
 */
class StudentRepository
{
public:

    /**
     * @brief Constructs a StudentRepository instance.
     *
     * @param database Reference to the Database object used for executing queries.
     *
     * The repository does not own the database instance.
     * The caller is responsible for managing its lifetime.
     */
    StudentRepository(Database& database);

    /**
     * @brief Adds a new student to the database.
     *
     * @param name The full name of the student.
     * @param email The email address of the student.
     * @return true if the student was successfully added, false otherwise.
     */
    bool addStudent(const QString& name,
                    const QString& email);

    /**
     * @brief Updates an existing student's information.
     *
     * @param id The unique identifier of the student.
     * @param name The updated full name of the student.
     * @param email The updated email address of the student.
     * @return true if the update operation succeeded, false otherwise.
     */
    bool updateStudent(int id,
                       const QString& name,
                       const QString& email);

    /**
     * @brief Finds a student by email address.
     *
     * @param email The email address to search for.
     * @return The student's ID if found, or -1 if no matching student exists.
     */
    int findStudentByEmail(const QString& email);

    /**
     * @brief Retrieves all students from the database.
     *
     * @return Pointer to a QSqlQueryModel containing all student records.
     *
     * The caller is responsible for managing the returned model's lifetime.
     */
    QSqlQueryModel* getAllStudents();

    /**
     * @brief Checks whether a student is associated with a user account.
     *
     * @param studentId The unique identifier of the student.
     * @return true if the student has an associated user account, false otherwise.
     */
    bool studentHasUser(int studentId);

    /**
     * @brief Finds a student by name.
     *
     * @param name The full name of the student.
     * @return The student's ID if found, or -1 if no matching student exists.
     */
    int findStudentByName(const QString& name);

private:

    /**
     * @brief Reference to the database instance.
     *
     * Used internally for executing SQL queries.
     * This class does not manage the lifetime of the database object.
     */
    Database& db;
};

#endif // STUDENTREPOSITORY_H
