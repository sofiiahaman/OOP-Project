#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>

class Database;

class TransactionRepository
{
public:
    explicit TransactionRepository(Database& database);

    bool addTransaction(int studentId, int bookId, const QString& issueDate);

    bool markAsReturned(int transactionId);

    int getBookIdByTransaction(int transactionId);

    QSqlQueryModel* getStudentBooks(int studentId);

    QSqlQuery getTransactionDetails(int transactionId);

private:
    Database& db;
};

#endif // TRANSACTIONREPOSITORY_H
