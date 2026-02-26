#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H

#include <QString>
#include <QSqlQueryModel>
#include "database.h"

class BookRepository
{
public:
    BookRepository(Database& database);

    bool addBook(const QString& title,
                 const QString& author,
                 const QString& publisher,
                 const QString& category);

    QSqlQueryModel* getAllBooks();

    bool updateBookAvailability(int bookId, bool available);

    QSqlQueryModel* searchByAny(const QString& searchTerm);
    QSqlQueryModel* searchWithCategory(const QString& searchTerm, const QString& category);
    QStringList getUniqueCategories();

private:
    Database& db;
};

#endif // BOOKREPOSITORY_H
