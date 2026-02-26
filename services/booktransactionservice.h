#ifndef BOOKTRANSACTIONSERVICE_H
#define BOOKTRANSACTIONSERVICE_H

#include <QString>
#include "database.h"
#include "./repositories/StudentRepository.h"
#include "./repositories/BookRepository.h"
#include "./repositories/TransactionRepository.h"

/**
 * @class BookTransactionService
 * @brief Provides business logic for issuing and returning books.
 *
 * The BookTransactionService class coordinates operations between
 * StudentRepository, BookRepository, and TransactionRepository
 * to handle book issuing and returning workflows.
 *
 * This class represents the service layer responsible for enforcing
 * transaction rules and maintaining consistency between entities.
 */
class BookTransactionService {
public:

    /**
     * @brief Constructs a BookTransactionService instance.
     *
     * @param db Reference to the Database object.
     *
     * Initializes all required repositories using the provided database.
     * The service does not take ownership of the database instance.
     */
    explicit BookTransactionService(Database& db);

    /**
     * @brief Issues a book to a student.
     *
     * @param studentName The full name of the student issuing the book.
     * @param bookId The unique identifier of the book to be issued.
     * @param date The issue date.
     * @return true if the book was successfully issued, false otherwise.
     *
     * This method typically:
     * - Verifies that the student exists.
     * - Ensures the book is available.
     * - Creates a new transaction record.
     * - Updates the book availability status.
     */
    bool issueBook(const QString& studentName,
                   int bookId,
                   const QString& date);

    /**
     * @brief Processes the return of a book.
     *
     * @param transactionId The unique identifier of the transaction.
     * @return true if the return operation succeeded, false otherwise.
     *
     * This method typically:
     * - Marks the transaction as returned.
     * - Updates the book availability status.
     */
    bool returnBook(int transactionId);

private:

    /**
     * @brief Reference to the database instance.
     */
    Database& m_db;

    /**
     * @brief Repository responsible for student-related operations.
     */
    StudentRepository m_studentRepo;

    /**
     * @brief Repository responsible for book-related operations.
     */
    BookRepository m_bookRepo;

    /**
     * @brief Repository responsible for transaction-related operations.
     */
    TransactionRepository m_transRepo;
};

#endif // BOOKTRANSACTIONSERVICE_H
