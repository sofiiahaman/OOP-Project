#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

/**
 * @class Database
 * @brief Manages the application's database connection.
 *
 * The Database class is responsible for initializing,
 * opening, and closing the database connection.
 * It also provides access to the underlying QSqlDatabase
 * instance for use by repositories and services.
 *
 * This class centralizes database connection handling
 * to ensure consistent access throughout the application.
 */
class Database
{
public:

    /**
     * @brief Constructs a Database instance.
     *
     * Initializes the QSqlDatabase object with the required
     * driver and connection settings (configured in implementation).
     */
    Database();

    /**
     * @brief Opens the database connection.
     *
     * @return true if the connection was successfully opened,
     *         false otherwise.
     */
    bool openConnection();

    /**
     * @brief Closes the database connection.
     *
     * Safely terminates the active database connection.
     */
    void closeConnection();

    /**
     * @brief Provides access to the underlying QSqlDatabase object.
     *
     * @return Reference to the QSqlDatabase instance.
     *
     * This method allows repositories to execute SQL queries
     * using the active database connection.
     */
    QSqlDatabase& getDb();

private:

    /**
     * @brief The underlying Qt SQL database connection object.
     */
    QSqlDatabase db;
};

#endif // DATABASE_H
