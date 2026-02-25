#include "database.h"
#include <QDir>
#include <QCoreApplication>
#include <QSqlError>
#include <QDebug>
#include <QDate>

Database::Database() {
    // Initialize SQLite connection
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        QString path = QCoreApplication::applicationDirPath() + "/library.db";
        db.setDatabaseName(path);
    } else {
        db = QSqlDatabase::database();
    }
}

bool Database::openConnection() {
    if (!db.isOpen()) {
        if (!db.open()) {
            qDebug() << "Database open error:" << db.lastError().text();
            return false;
        }
        qDebug() << "Database connection successful!";
    }
    return true;
}

void Database::closeConnection() {
    if (db.isOpen()) {
        db.close();
    }
}

// --- AUTHENTICATION AND USERS ---

bool Database::checkLogin(const QString& username, const QString& password) {
    if (!openConnection()) return false;

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", username);
    query.bindValue(":p", password);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

QString Database::getUserRole(const QString &username) {
    if (!openConnection()) return QString();

    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = :u");
    query.bindValue(":u", username);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return QString();
}

bool Database::userExists(const QString &username) {
    if (!openConnection()) return false;

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :u");
    query.bindValue(":u", username);

    return query.exec() && query.next() && query.value(0).toInt() > 0;
}

int Database::addUserReturnId(const QString &username, const QString &password, const QString &role) {
    if (!openConnection()) return -1;

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) VALUES (:u, :p, :r)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);
    query.bindValue(":r", role);

    if (!query.exec()) return -1;

    return query.lastInsertId().toInt();
}

// --- BUSINESS LOGIC: BOOKS AND TRANSACTIONS ---

bool Database::addBook(const QString &title, const QString &author, const QString &publisher, const QString &category) {
    if (!openConnection()) return false;

    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, publisher, category, available) "
                  "VALUES (:t, :a, :p, :c, 1)");
    query.bindValue(":t", title);
    query.bindValue(":a", author);
    query.bindValue(":p", publisher);
    query.bindValue(":c", category);

    return query.exec();
}

bool Database::returnBookTransaction(int transactionId, int bookId) {
    if (!openConnection()) return false;

    db.transaction();
    QSqlQuery query;

    // 1. Update the return date
    query.prepare("UPDATE transactions SET return_date = :rd WHERE id = :id");
    query.bindValue(":rd", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":id", transactionId);

    if(!query.exec()) {
        db.rollback();
        return false;
    }

    // 2. Update the book availability status
    query.prepare("UPDATE books SET available = 1 WHERE id = :bid");
    query.bindValue(":bid", bookId);

    if(!query.exec()) {
        db.rollback();
        return false;
    }

    return db.commit();
}

// --- STUDENTS ---

int Database::findStudentByEmail(const QString &email) {
    if (!openConnection()) return -1;

    QSqlQuery query;
    query.prepare("SELECT id FROM students WHERE email = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) return query.value(0).toInt();
    return -1;
}

bool Database::studentHasUser(int studentId) {
    if (!openConnection()) return false;

    QSqlQuery query;
    query.prepare("SELECT user_id FROM students WHERE id = :id");
    query.bindValue(":id", studentId);

    return query.exec() && query.next() && !query.value(0).isNull();
}

bool Database::linkStudentWithUser(int studentId, int userId) {
    if (!openConnection()) return false;

    QSqlQuery query;
    query.prepare("UPDATE students SET user_id = :uid WHERE id = :sid");
    query.bindValue(":uid", userId);
    query.bindValue(":sid", studentId);

    return query.exec();
}

// --- STATISTICS ---

int Database::getBooksCount() { return getScalarValue("SELECT COUNT(*) FROM books"); }
int Database::getAuthorsCount() { return getScalarValue("SELECT COUNT(DISTINCT author) FROM books"); }
int Database::getCategoriesCount() { return getScalarValue("SELECT COUNT(DISTINCT category) FROM books"); }
int Database::getPublishersCount() { return getScalarValue("SELECT COUNT(DISTINCT publisher) FROM books"); }
int Database::getStudentsCount() { return getScalarValue("SELECT COUNT(*) FROM students"); }
int Database::getAvailableBooksCount() { return getScalarValue("SELECT COUNT(*) FROM books WHERE available = 1"); }
int Database::getIssuedBooksCount() { return getScalarValue("SELECT COUNT(*) FROM transactions"); }
int Database::getReturnedBooksCount() { return getScalarValue("SELECT COUNT(*) FROM transactions WHERE return_date IS NOT NULL"); }
int Database::getNotReturnBooksCount() { return getScalarValue("SELECT COUNT(*) FROM transactions WHERE return_date IS NULL"); }

// Helper method to reduce duplication in statistics
int Database::getScalarValue(const QString &sql) {
    if (!openConnection()) return 0;
    QSqlQuery query(sql);
    return (query.next()) ? query.value(0).toInt() : 0;
}
