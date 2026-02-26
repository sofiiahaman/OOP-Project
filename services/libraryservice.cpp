#include "LibraryService.h"
#include <QStringList>

LibraryService::LibraryService(Database& db)
    : m_db(db), m_bookRepo(db) {}

bool LibraryService::addNewBook(const QString& title, const QString& author,
                                const QString& publisher, const QString& category) {
    if (title.trimmed().isEmpty() || author.trimmed().isEmpty()) {
        return false;
    }

    return m_bookRepo.addBook(title.trimmed(), author.trimmed(),
                              publisher.trimmed(), category.trimmed());
}

QSqlQueryModel* LibraryService::searchBooks(const QString& searchTerm, const QString& category) {
    if (category == "All" || category.isEmpty()) {
        return m_bookRepo.searchByAny(searchTerm);
    }
    return m_bookRepo.searchWithCategory(searchTerm, category);
}

QStringList LibraryService::getAllCategories() {
    return m_bookRepo.getUniqueCategories();
}

QString LibraryService::buildFilterString(const BookSearchParams &params) {
    QStringList filters;

    if (!params.title.isEmpty())
        filters << QString("title LIKE '%%1%'").arg(params.title);
    if (!params.author.isEmpty())
        filters << QString("author LIKE '%%1%'").arg(params.author);
    if (!params.publisher.isEmpty())
        filters << QString("publisher LIKE '%%1%'").arg(params.publisher);
    if (!params.id.isEmpty())
        filters << QString("id = '%1'").arg(params.id);
    if (!params.category.isEmpty() && params.category != "Choose category")
        filters << QString("category = '%1'").arg(params.category);
    if (params.availableOnly)
        filters << "available = 1";

    return filters.join(" AND ");
}
