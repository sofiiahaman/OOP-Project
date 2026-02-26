#ifndef STATISTICSREPOSITORY_H
#define STATISTICSREPOSITORY_H

#include "database.h"

/**
 * @class StatisticsRepository
 * @brief Provides statistical data and aggregated counters for the dashboard.
 *
 * The StatisticsRepository class is responsible for retrieving
 * statistical information from the database. It executes count-based
 * SQL queries to provide summary metrics such as total number of books,
 * authors, students, issued books, etc.
 *
 * This class acts as a data access layer dedicated to dashboard statistics.
 */
class StatisticsRepository
{
public:

    /**
     * @brief Constructs a StatisticsRepository instance.
     *
     * @param database Pointer to the Database object used for executing queries.
     *
     * The repository does not take ownership of the database pointer.
     * The caller is responsible for managing the database lifetime.
     */
    explicit StatisticsRepository(Database* database);

    /**
     * @brief Retrieves the total number of books.
     * @return Total count of books stored in the database.
     */
    int getBooksCount();

    /**
     * @brief Retrieves the total number of authors.
     * @return Total count of authors stored in the database.
     */
    int getAuthorsCount();

    /**
     * @brief Retrieves the total number of categories.
     * @return Total count of book categories.
     */
    int getCategoriesCount();

    /**
     * @brief Retrieves the total number of publishers.
     * @return Total count of publishers stored in the database.
     */
    int getPublishersCount();

    /**
     * @brief Retrieves the total number of students.
     * @return Total count of registered students.
     */
    int getStudentsCount();

    /**
     * @brief Retrieves the total number of available books.
     * @return Count of books currently available for issuing.
     */
    int getAvailableBooksCount();

    /**
     * @brief Retrieves the total number of issued books.
     * @return Count of books currently issued to students.
     */
    int getIssuedBooksCount();

    /**
     * @brief Retrieves the total number of returned books.
     * @return Count of books that have been returned.
     */
    int getReturnedBooksCount();

    /**
     * @brief Retrieves the total number of books that have not yet been returned.
     * @return Count of books currently issued and not returned.
     */
    int getNotReturnedBooksCount();

private:

    /**
     * @brief Pointer to the database instance.
     *
     * Used internally to execute SQL count queries.
     * This class does not manage the memory of this pointer.
     */
    Database* db;

    /**
     * @brief Executes a SQL COUNT query and returns the result.
     *
     * @param queryStr The SQL query string to execute.
     * @return The integer result of the COUNT query.
     *
     * This is a helper method used internally by public
     * statistics methods to avoid duplication of query execution logic.
     */
    int executeCountQuery(const QString& queryStr);
};

#endif // STATISTICSREPOSITORY_H
