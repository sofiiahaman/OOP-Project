#ifndef LIBRARYSERVICE_H
#define LIBRARYSERVICE_H

#include <QString>
#include <QSqlQueryModel>
#include "database.h"
#include "repositories/BookRepository.h"
#include "basebookview.h"

class LibraryService {
public:
    explicit LibraryService(Database& db);

    bool addNewBook(const QString& title, const QString& author,
                    const QString& publisher, const QString& category);

    QSqlQueryModel* searchBooks(const QString& searchTerm, const QString& category = "All");

    QStringList getAllCategories();

    QString buildFilterString(const BookSearchParams &params);

private:
    Database& m_db;
    BookRepository m_bookRepo;
};

#endif //LIBRARYSERVICE_H
