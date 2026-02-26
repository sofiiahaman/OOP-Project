#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>

class Database;

/**
 * @class TransactionRepository
 * @brief Handles database operations related to book transactions.
 *
 * The TransactionRepository class provides methods for managing
 * book issue and return transactions. It encapsulates all
 * transaction-related database queries and acts as a data access
 * layer between the application logic and the database.
 */
class TransactionRepository
{
public:

    /**
     * @brief Constructs a TransactionRepository instance.
     *
     * @param database Reference to the Database object used for executing queries.
     *
     * The repository does not take ownership of the database instance.
     * The caller is responsible for managing its lifetime.
     */
    explicit TransactionRepository(Database& database);

    /**
     * @brief Adds a new book issue transaction.
     *
     * @param studentId The unique identifier of the student issuing the book.
     * @param bookId The unique identifier of the book being issued.
     * @param issueDate The date when the book was issued.
     * @return true if the transaction was successfully created, false otherwise.
     */
    bool addTransaction(int studentId, int bookId, const QString& issueDate);

    /**
     * @brief Marks a transaction as returned.
     *
     * @param transactionId The unique identifier of the transaction.
     * @return true if the transaction was successfully updated as returned,
     *         false otherwise.
     */
    bool markAsReturned(int transactionId);

    /**
     * @brief Retrieves the book ID associated with a specific transaction.
     *
     * @param transactionId The unique identifier of the transaction.
     * @return The book ID if found, or -1 if the transaction does not exist.
     */
    int getBookIdByTransaction(int transactionId);

    /**
     * @brief Retrieves all books issued to a specific student.
     *
     * @param studentId The unique identifier of the student.
     * @return Pointer to a QSqlQueryModel containing the student's issued books.
     *
     * The caller is responsible for managing the returned model's lifetime.
     */
    QSqlQueryModel* getStudentBooks(int studentId);

    /**
     * @brief Retrieves detailed information about a specific transaction.
     *
     * @param transactionId The unique identifier of the transaction.
     * @return A QSqlQuery object containing the transaction details.
     *
     * The returned query can be iterated to access detailed transaction data.
     */
    QSqlQuery getTransactionDetails(int transactionId);

private:

    /**
     * @brief Reference to the database instance.
     *
     * Used internally to execute SQL queries.
     * This class does not manage the lifetime of the database object.
     */
    Database& db;
};

#endif // TRANSACTIONREPOSITORY_H
