#include "TransactionRepository.h"
#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

TransactionRepository::TransactionRepository(Database& database)
    : db(database)
{
}

bool TransactionRepository::issueBook(int studentId,
                                      int bookId,
                                      const QString& issueDate)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("SELECT available FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    if (!query.exec() || !query.next())
        return false;

    if (!query.value(0).toBool())
        return false;

    query.prepare("INSERT INTO transactions (student_id, book_id, issue_date, return_date) "
                  "VALUES (:s, :b, :d, NULL)");

    query.bindValue(":s", studentId);
    query.bindValue(":b", bookId);
    query.bindValue(":d", issueDate);

    if (!query.exec()) {
        qDebug() << "Issue error:" << query.lastError().text();
        return false;
    }

    // Update availability
    query.prepare("UPDATE books SET available = 0 WHERE id = :id");
    query.bindValue(":id", bookId);

    if (!query.exec())
        return false;

    return true;
}

bool TransactionRepository::returnBook(int transactionId)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("UPDATE transactions SET return_date = CURRENT_DATE WHERE id = :id");
    query.bindValue(":id", transactionId);

    if (!query.exec()) {
        qDebug() << "Return book error:" << query.lastError().text();
        return false;
    }

    QSqlQuery bookQuery(db.getDb());

    bookQuery.prepare("SELECT book_id FROM transactions WHERE id = :id");
    bookQuery.bindValue(":id", transactionId);

    if (!bookQuery.exec() || !bookQuery.next()) {
        qDebug() << "Cannot get book id";
        return false;
    }

    int bookId = bookQuery.value(0).toInt();

    // Set available = 1
    QSqlQuery updateQuery(db.getDb());

    updateQuery.prepare("UPDATE books SET available = 1 WHERE id = :id");
    updateQuery.bindValue(":id", bookId);

    if (!updateQuery.exec()) {
        qDebug() << "Update availability error:" << updateQuery.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* TransactionRepository::getStudentBooks(int studentId) {
    if (!db.openConnection()) return nullptr;

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(
        "SELECT t.id, t.book_id, b.title, b.author, t.issue_date "
        "FROM transactions t "
        "JOIN books b ON t.book_id = b.id "
        "WHERE t.student_id = " + QString::number(studentId) +
            " AND t.return_date IS NULL",
        db.getDb());

    return model;
}

QSqlQuery TransactionRepository::getTransactionDetails(int transactionId) {
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

