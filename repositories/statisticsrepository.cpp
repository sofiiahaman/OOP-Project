#include "statisticsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

StatisticsRepository::StatisticsRepository(Database* database)
    : db(database)
{
}

int StatisticsRepository::executeCountQuery(const QString& queryStr)
{
    QSqlQuery query;

    if (!query.exec(queryStr)) {
        qDebug() << "Statistics query error:" << query.lastError().text();
        return 0;
    }

    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int StatisticsRepository::getBooksCount()
{
    return executeCountQuery("SELECT COUNT(*) FROM books");
}

int StatisticsRepository::getAuthorsCount()
{
    return executeCountQuery("SELECT COUNT(DISTINCT author) FROM books");
}

int StatisticsRepository::getCategoriesCount()
{
    return executeCountQuery("SELECT COUNT(DISTINCT category) FROM books");
}

int StatisticsRepository::getPublishersCount()
{
    return executeCountQuery("SELECT COUNT(DISTINCT publisher) FROM books");
}

int StatisticsRepository::getStudentsCount()
{
    return executeCountQuery("SELECT COUNT(*) FROM students");
}

int StatisticsRepository::getAvailableBooksCount()
{
    return executeCountQuery("SELECT COUNT(*) FROM books WHERE available = 1");
}

int StatisticsRepository::getIssuedBooksCount()
{
    return executeCountQuery("SELECT COUNT(*) FROM transactions");
}

int StatisticsRepository::getReturnedBooksCount()
{
    return executeCountQuery("SELECT COUNT(*) FROM transactions WHERE return_date IS NOT NULL");
}

int StatisticsRepository::getNotReturnedBooksCount()
{
    return executeCountQuery("SELECT COUNT(*) FROM transactions WHERE return_date IS NULL");
}
