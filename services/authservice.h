#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>
#include "database.h"
#include "repositories/UserRepository.h"
#include "repositories/StudentRepository.h"

/**
 * @class AuthService
 * @brief Provides authentication and registration business logic.
 *
 * The AuthService class handles user authentication and registration
 * processes. It coordinates between UserRepository and StudentRepository
 * to validate credentials and link existing students to user accounts.
 *
 * This class represents the service layer between the application
 * controllers and the repository layer.
 */
class AuthService {
public:

    /**
     * @brief Constructs an AuthService instance.
     *
     * @param db Reference to the Database object.
     *
     * Initializes internal repositories using the provided database.
     * The service does not take ownership of the database instance.
     */
    explicit AuthService(Database& db);

    /**
     * @brief Authenticates a user with the provided credentials.
     *
     * @param username The username of the account.
     * @param password The password associated with the account.
     * @return The role of the authenticated user (e.g., "admin", "student")
     *         if login is successful; otherwise, an empty QString.
     *
     * This method validates the credentials using the UserRepository.
     */
    QString login(const QString& username, const QString& password);

    /**
     * @brief Registers a user account for an existing student.
     *
     * @param email The email address of the existing student.
     * @param username The desired username for the new account.
     * @param password The desired password for the new account.
     * @return A status message indicating the result of the registration
     *         (e.g., success or error description).
     *
     * This method verifies that:
     * - The student exists.
     * - The username is not already taken.
     * - The student is not already linked to a user account.
     *
     * If validation succeeds, a new user account is created and linked
     * to the corresponding student record.
     */
    QString registerExistingStudent(const QString& email,
                                    const QString& username,
                                    const QString& password);

private:

    /**
     * @brief Reference to the database instance.
     */
    Database& m_db;

    /**
     * @brief Repository responsible for user-related operations.
     */
    UserRepository m_userRepo;

    /**
     * @brief Repository responsible for student-related operations.
     */
    StudentRepository m_studentRepo;
};

#endif //AUTHSERVICE_H
