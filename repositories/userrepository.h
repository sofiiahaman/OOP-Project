#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QString>
#include <QSqlQueryModel>

class Database;

/**
 * @class UserRepository
 * @brief Provides database operations related to users and authentication.
 *
 * The UserRepository class manages user accounts, authentication,
 * role retrieval, and associations between users and students.
 * It acts as the data access layer between the application logic
 * and the database for user-related operations.
 */
class UserRepository
{
public:

    /**
     * @brief Constructs a UserRepository instance.
     *
     * @param database Reference to the Database object used for executing queries.
     *
     * The repository does not take ownership of the database instance.
     * The caller is responsible for managing its lifetime.
     */
    explicit UserRepository(Database& database);

    /**
     * @brief Adds a new student record.
     *
     * @param name The full name of the student.
     * @param email The email address of the student.
     * @return true if the student was successfully added, false otherwise.
     *
     * @note This function handles student data within the user context.
     */
    bool addStudent(const QString& name, const QString& email);

    /**
     * @brief Updates an existing student record.
     *
     * @param id The unique identifier of the student.
     * @param name The updated full name of the student.
     * @param email The updated email address of the student.
     * @return true if the update operation succeeded, false otherwise.
     */
    bool updateStudent(int id, const QString& name, const QString& email);

    /**
     * @brief Searches for students by name.
     *
     * @param name The name or partial name used for searching.
     * @return Pointer to a QSqlQueryModel containing matching student records.
     *
     * The caller is responsible for managing the returned model's lifetime.
     */
    QSqlQueryModel* searchStudents(const QString& name);

    /**
     * @brief Verifies user login credentials.
     *
     * @param username The username of the account.
     * @param password The password associated with the account.
     * @return true if the credentials are valid, false otherwise.
     */
    bool checkLogin(const QString& username,
                    const QString& password);

    /**
     * @brief Retrieves the role assigned to a specific user.
     *
     * @param username The username of the account.
     * @return The role of the user (e.g., admin, student).
     *
     * Returns an empty QString if the user does not exist.
     */
    QString getUserRole(const QString& username);

    /**
     * @brief Checks whether a user already exists.
     *
     * @param username The username to check.
     * @return true if the user exists, false otherwise.
     */
    bool userExists(const QString& username);

    /**
     * @brief Adds a new user and returns the generated user ID.
     *
     * @param username The username for the new account.
     * @param password The password for the new account.
     * @param role The role assigned to the user.
     * @return The ID of the newly created user, or -1 if creation failed.
     */
    int addUserReturnId(const QString& username,
                        const QString& password,
                        const QString& role);

    /**
     * @brief Links a student record with a user account.
     *
     * @param studentId The unique identifier of the student.
     * @param userId The unique identifier of the user.
     * @return true if the link was successfully created, false otherwise.
     */
    bool linkStudentWithUser(int studentId, int userId);

private:

    /**
     * @brief Reference to the database instance.
     *
     * Used internally to execute SQL queries.
     * This class does not manage the lifetime of the database object.
     */
    Database& db;
};

#endif // USERREPOSITORY_H
