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

private:
    Database& db;
};

#endif // BOOKREPOSITORY_H
