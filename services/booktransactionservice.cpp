#include "BookTransactionService.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

BookTransactionService::BookTransactionService(Database& db)
    : m_db(db)
    , m_studentRepo(db)
    , m_bookRepo(db)
    , m_transRepo(db)
{
}

bool BookTransactionService::issueBook(const QString& studentName, int bookId, const QString& date)
{

    int studentId = m_studentRepo.findStudentByName(studentName);
    if (studentId == -1) {
        qDebug() << "Service: Student not found";
        return false;
    }

    QSqlDatabase db = m_db.getDb();
    if (!db.transaction()) {
        qDebug() << "Service: Could not start transaction" << db.lastError().text();
        return false;
    }

    try {

        if (!m_transRepo.addTransaction(studentId, bookId, date)) {
            throw std::runtime_error("Failed to create transaction record");
        }

        if (!m_bookRepo.updateBookAvailability(bookId, false)) {
            throw std::runtime_error("Failed to update book availability status");
        }

        if (db.commit()) {
            return true;
        } else {
            throw std::runtime_error("Commit failed");
        }

    } catch (const std::exception& e) {
        qDebug() << "Service Error (Issue):" << e.what();
        db.rollback();
        return false;
    }
}

bool BookTransactionService::returnBook(int transactionId)
{
    QSqlDatabase db = m_db.getDb();
    if (!db.transaction()) return false;

    try {

        int bookId = m_transRepo.getBookIdByTransaction(transactionId);
        if (bookId == -1) throw std::runtime_error("Transaction not found");

        if (!m_transRepo.markAsReturned(transactionId)) {
            throw std::runtime_error("Failed to update return date");
        }

        if (!m_bookRepo.updateBookAvailability(bookId, true)) {
            throw std::runtime_error("Failed to update book availability");
        }

        if (db.commit()) {
            return true;
        } else {
            throw std::runtime_error("Commit failed");
        }

    } catch (const std::exception& e) {
        qDebug() << "Service Error (Return):" << e.what();
        db.rollback();
        return false;
    }
}
