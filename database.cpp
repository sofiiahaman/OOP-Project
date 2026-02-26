#include "database.h"

#include <QCoreApplication>
#include <QSqlError>
#include <QDebug>

Database::Database()
{
    // Create or reuse default Qt SQL connection
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QSQLITE");

        QString path = QCoreApplication::applicationDirPath() + "/library.db";
        db.setDatabaseName(path);
    } else {
        db = QSqlDatabase::database();
    }
}

bool Database::openConnection()
{
    if (!db.isOpen()) {
        if (!db.open()) {
            qDebug() << "Database open error:" << db.lastError().text();
            return false;
        }
        qDebug() << "Database connection successful!";
    }
    return true;
}

void Database::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}

QSqlDatabase& Database::getDb()
{
    return db;
}
