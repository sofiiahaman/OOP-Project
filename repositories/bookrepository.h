/**
 * @file bookrepository.h
 * @brief Repository class for managing book data in the database.
 */

#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H

#include <QString>
#include <QSqlQueryModel>
#include "database.h"

/**
 * @class BookRepository
 * @brief Handles CRUD operations and searches for the Books table.
 */
class BookRepository
{
public:
    /**
     * @brief Constructs the BookRepository.
     * @param database Reference to the active Database connection object.
     */
    BookRepository(Database& database);

    /**
     * @brief Adds a new book record to the database.
     * @return true if the insertion was successful, false otherwise.
     */
    bool addBook(const QString& title,
                 const QString& author,
                 const QString& publisher,
                 const QString& category);

    /**
     * @brief Fetches all books from the database.
     * @return A pointer to a QSqlQueryModel containing all book records.
     */
    QSqlQueryModel* getAllBooks();

    /**
     * @brief Updates the availability status of a specific book.
     * @param bookId The unique ID of the book.
     * @param available Boolean flag (true for available, false for issued).
     */
    bool updateBookAvailability(int bookId, bool available);

    /**
     * @brief Searches for books by a general search term across multiple columns.
     * @param searchTerm The string to search for (title, author, etc.).
     */
    QSqlQueryModel* searchByAny(const QString& searchTerm);

    /**
     * @brief Searches for books within a specific category.
     */
    QSqlQueryModel* searchWithCategory(const QString& searchTerm, const QString& category);

    /**
     * @brief Retrieves a list of all unique categories present in the database.
     */
    QStringList getUniqueCategories();

private:
    Database& db; ///< Reference to the database instance.
};

#endif // BOOKREPOSITORY_H
