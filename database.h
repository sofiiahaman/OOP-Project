#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database
{
public:
    Database();

    bool openConnection();
    void closeConnection();

    // Access to QSqlDatabase for repositories
    QSqlDatabase& getDb();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
