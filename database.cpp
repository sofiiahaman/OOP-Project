#include "database.h"
#include <QDir>
#include <QCoreApplication>

Database::Database() {
    // Initializing SQLite connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QCoreApplication::applicationDirPath() + "/library.db";
    db.setDatabaseName(path);
}

bool Database::openConnection() {
    qDebug() << "Database path:" << QDir::currentPath();

    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        return false;
    }
    qDebug() << "Database connection successful!";
    return true;
}

void Database::closeConnection() {
    db.close();
}

bool Database::checkLogin(const QString& username, const QString& password) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        qDebug() << "User login successful:" << username;
        return true;
    } else {
        qDebug() << "No user found with such login or password.";
    }

    return false;
}

QString Database::getUserRole(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return QString();
}

int Database::getBooksCount() {
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(*) FROM books");
    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getAuthorsCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(DISTINCT author) FROM books");

    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getCategoriesCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(DISTINCT category) FROM books");
    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getPublishersCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(DISTINCT publisher) FROM books");

    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getStudentsCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(*) FROM students");
    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getAvailableBooksCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(*) FROM books WHERE available = 1");

    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getIssuedBooksCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(*) FROM transactions");
    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getReturnedBooksCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(*) FROM transactions WHERE return_date IS NOT NULL;");
    if (query.next())
        return query.value(0).toInt();

    return 0;
}

int Database::getNotReturnBooksCount()
{
    if (!db.isOpen())
        db.open();

    QSqlQuery query("SELECT COUNT(*) FROM transactions WHERE return_date IS NULL;");
    if (query.next())
        return query.value(0).toInt();

    return 0;
}

bool Database::userExists(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :u");
    query.bindValue(":u", username);

    if (!query.exec()) {
        qDebug() << "userExists error:" << query.lastError();
        return false;
    }

    if (query.next())
        return query.value(0).toInt() > 0;

    return false;
}

bool Database::addUser(const QString &username, const QString &password, const QString &role) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) VALUES (:u, :p, :r)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);
    query.bindValue(":r", role);

    if (!query.exec()) {
        qDebug() << "addUser error:" << query.lastError();
        return false;
    }

    return true;
}

int Database::findStudentByEmail(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM students WHERE email = :email");
    query.bindValue(":email", email);

    if (!query.exec() || !query.next())
        return -1;

    return query.value(0).toInt();
}

bool Database::studentHasUser(int studentId)
{
    QSqlQuery query;
    query.prepare("SELECT user_id FROM students WHERE id = :id");
    query.bindValue(":id", studentId);
    query.exec();

    if (!query.next()) return false;

    return !query.value(0).isNull();
}

int Database::addUserReturnId(const QString &username,
                              const QString &password,
                              const QString &role)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) "
                  "VALUES (:u, :p, :r)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);
    query.bindValue(":r", role);

    if (!query.exec()) {
        qDebug() << "Add user error:" << query.lastError();
        return -1;
    }

    return query.lastInsertId().toInt();
}


bool Database::linkStudentWithUser(int studentId, int userId)
{
    QSqlQuery query;
    query.prepare("UPDATE students SET user_id = :uid WHERE id = :sid");
    query.bindValue(":uid", userId);
    query.bindValue(":sid", studentId);

    return query.exec();
}


