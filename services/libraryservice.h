#ifndef LIBRARYSERVICE_H
#define LIBRARYSERVICE_H

#include <QString>
#include <QSqlQueryModel>
#include "database.h"
#include "repositories/BookRepository.h"
#include "basebookview.h"

/**
 * @class LibraryService
 * @brief Provides business logic for managing and searching books.
 *
 * The LibraryService class acts as a service layer between the
 * application logic (e.g., UI/controllers) and the BookRepository.
 * It coordinates book creation, searching, category retrieval,
 * and dynamic filter construction.
 */
class LibraryService {
public:

    /**
     * @brief Constructs a LibraryService instance.
     *
     * @param db Reference to the Database object.
     *
     * Initializes the internal BookRepository using the provided database.
     * The service does not take ownership of the database instance.
     */
    explicit LibraryService(Database& db);

    /**
     * @brief Adds a new book to the library.
     *
     * @param title The title of the book.
     * @param author The author name.
     * @param publisher The publisher name.
     * @param category The category name.
     * @return true if the book was successfully added, false otherwise.
     *
     * This method ensures that related entities such as author,
     * publisher, and category are properly handled before insertion.
     */
    bool addNewBook(const QString& title,
                    const QString& author,
                    const QString& publisher,
                    const QString& category);

    /**
     * @brief Searches for books using a search term and optional category filter.
     *
     * @param searchTerm The keyword used to search (e.g., title or author).
     * @param category The category filter (default is "All" for no filtering).
     * @return Pointer to a QSqlQueryModel containing the search results.
     *
     * The caller is responsible for managing the returned model's lifetime.
     */
    QSqlQueryModel* searchBooks(const QString& searchTerm,
                                const QString& category = "All");

    /**
     * @brief Retrieves all available book categories.
     *
     * @return A QStringList containing all category names.
     */
    QStringList getAllCategories();

    /**
     * @brief Builds a SQL filter string based on search parameters.
     *
     * @param params A structure containing book search parameters.
     * @return A formatted SQL WHERE clause fragment.
     *
     * This helper method converts structured search parameters
     * into a dynamic SQL condition string.
     */
    QString buildFilterString(const BookSearchParams &params);

private:

    /**
     * @brief Reference to the database instance.
     */
    Database& m_db;

    /**
     * @brief Repository responsible for book-related database operations.
     */
    BookRepository m_bookRepo;
};

#endif //LIBRARYSERVICE_H
