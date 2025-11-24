#include "database.h"
#include <QDir>
#include <QCoreApplication>

Database::Database() {
    // Ініціалізація підключення до SQLite
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QCoreApplication::applicationDirPath() + "/library.db";
    db.setDatabaseName(path);
}

bool Database::openConnection() {
    qDebug() << "Шлях до бази:" << QDir::currentPath();

    if (!db.open()) {
        qDebug() << "Помилка відкриття бази:" << db.lastError().text();
        return false;
    }
    qDebug() << "Підключення до бази успішне!";
    return true;
}

void Database::closeConnection() {
    db.close();
}

bool Database::checkLogin(const QString& username, const QString& password) {
    if (!db.isOpen()) {
        qDebug() << "База не відкрита!";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Помилка запиту:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        qDebug() << "Успішний вхід користувача:" << username;
        return true;
    } else {
        qDebug() << "Немає користувача з таким логіном або паролем.";
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
