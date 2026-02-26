#include "TransactionRepository.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

TransactionRepository::TransactionRepository(Database& database)
    : db(database)
{
}

bool TransactionRepository::addTransaction(int studentId, int bookId, const QString& issueDate)
{
    QSqlQuery query(db.getDb());
    query.prepare("INSERT INTO transactions (student_id, book_id, issue_date, return_date) "
                  "VALUES (:s, :b, :d, NULL)");
    query.bindValue(":s", studentId);
    query.bindValue(":b", bookId);
    query.bindValue(":d", issueDate);

    if (!query.exec()) {
        qDebug() << "Add transaction error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool TransactionRepository::markAsReturned(int transactionId)
{
    QSqlQuery query(db.getDb());
    query.prepare("UPDATE transactions SET return_date = CURRENT_DATE WHERE id = :id");
    query.bindValue(":id", transactionId);

    if (!query.exec()) {
        qDebug() << "Mark as returned error:" << query.lastError().text();
        return false;
    }
    return true;
}

int TransactionRepository::getBookIdByTransaction(int transactionId)
{
    QSqlQuery query(db.getDb());
    query.prepare("SELECT book_id FROM transactions WHERE id = :id");
    query.bindValue(":id", transactionId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

QSqlQueryModel* TransactionRepository::getStudentBooks(int studentId)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(db.getDb());

    query.prepare("SELECT t.id, t.book_id, b.title, b.author, t.issue_date "
                  "FROM transactions t "
                  "JOIN books b ON t.book_id = b.id "
                  "WHERE t.student_id = :sid AND t.return_date IS NULL");
    query.bindValue(":sid", studentId);

    if (!query.exec()) {
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    return model;
}

QSqlQuery TransactionRepository::getTransactionDetails(int transactionId)
{
    QSqlQuery query(db.getDb());
    query.prepare("SELECT t.issue_date, s.name, s.email, b.title, b.author "
                  "FROM transactions t "
                  "JOIN students s ON t.student_id = s.id "
                  "JOIN books b ON t.book_id = b.id "
                  "WHERE t.id = :tid");
    query.bindValue(":tid", transactionId);
    query.exec();
    return query;
}
