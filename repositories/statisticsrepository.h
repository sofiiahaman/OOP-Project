#ifndef STATISTICSREPOSITORY_H
#define STATISTICSREPOSITORY_H

#include "database.h"

class StatisticsRepository
{
public:
    explicit StatisticsRepository(Database* database);

    int getBooksCount();
    int getAuthorsCount();
    int getCategoriesCount();
    int getPublishersCount();
    int getStudentsCount();

    int getAvailableBooksCount();
    int getIssuedBooksCount();
    int getReturnedBooksCount();
    int getNotReturnedBooksCount();

private:
    Database* db;

    int executeCountQuery(const QString& queryStr);
};

#endif // STATISTICSREPOSITORY_H
