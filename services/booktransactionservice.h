#ifndef BOOKTRANSACTIONSERVICE_H
#define BOOKTRANSACTIONSERVICE_H

#include <QString>
#include "database.h"
#include "./repositories/StudentRepository.h"
#include "./repositories/BookRepository.h"
#include "./repositories/TransactionRepository.h"

class BookTransactionService {
public:
    explicit BookTransactionService(Database& db);

    bool issueBook(const QString& studentName, int bookId, const QString& date);

    bool returnBook(int transactionId);

private:
    Database& m_db;
    StudentRepository m_studentRepo;
    BookRepository m_bookRepo;
    TransactionRepository m_transRepo;
};

#endif // BOOKTRANSACTIONSERVICE_H
