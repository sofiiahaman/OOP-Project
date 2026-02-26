#include "BookRepository.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BookRepository::BookRepository(Database& database)
    : db(database)
{
}

bool BookRepository::addBook(const QString& title,
                             const QString& author,
                             const QString& publisher,
                             const QString& category)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("INSERT INTO books (title, author, publisher, category, available) "
                  "VALUES (:t, :a, :p, :c, 1)");

    query.bindValue(":t", title);
    query.bindValue(":a", author);
    query.bindValue(":p", publisher);
    query.bindValue(":c", category);

    if (!query.exec()) {
        qDebug() << "Add book error:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* BookRepository::getAllBooks()
{
    if (!db.openConnection())
        return nullptr;

    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT id, title, author, publisher, category, available FROM books",
                    db.getDb());

    return model;
}

bool BookRepository::updateBookAvailability(int bookId, bool available)
{
    if (!db.openConnection())
        return false;

    QSqlQuery query(db.getDb());

    query.prepare("UPDATE books SET available = :a WHERE id = :id");
    query.bindValue(":a", available ? 1 : 0);
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qDebug() << "Update availability error:" << query.lastError().text();
        return false;
    }

    return true;
}
