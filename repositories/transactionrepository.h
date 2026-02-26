#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QSqlQueryModel>
#include <QSqlQuery>

class Database;

class TransactionRepository
{
public:
    TransactionRepository(Database& database);

    bool issueBook(int studentId,
                   int bookId,
                   const QString& issueDate);
    bool returnBook(int transactionId);

    QSqlQueryModel* getStudentBooks(int studentId);

    QSqlQuery getTransactionDetails(int transactionId);

private:
    Database& db;
};

#endif
