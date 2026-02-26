#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "services/authservice.h"
#include "database.h"

class TestAuthService : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void testLogin();
    void testRegistration();

private:
    Database db;
    void setupDatabaseSchema();
};

void TestAuthService::setupDatabaseSchema() {
    QSqlQuery query(db.getDb());

    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT UNIQUE, "
               "password TEXT, "
               "role TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS students ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "email TEXT UNIQUE, "
               "name TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS student_users ("
               "student_id INTEGER, "
               "user_id INTEGER)");
}

void TestAuthService::init() {
    db.openConnection();
    setupDatabaseSchema();

    QSqlQuery query(db.getDb());

    query.exec("DELETE FROM users");
    query.exec("DELETE FROM students");

    query.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
    query.addBindValue("admin");
    query.addBindValue("admin123");
    query.addBindValue("admin");
    if (!query.exec()) {
        qDebug() << "Init error:" << query.lastError().text();
    }
}

void TestAuthService::testLogin() {
    AuthService service(db);

    QString role = service.login("admin", "admin123");
    QVERIFY2(!role.isEmpty(), "Login failed for existing user 'admin'");
    QCOMPARE(role, QString("admin"));

    QString wrongRole = service.login("admin", "wrong_pass");
    QVERIFY(wrongRole.isEmpty());

    QString nonExistent = service.login("nobody", "123");
    QVERIFY(nonExistent.isEmpty());
}

void TestAuthService::testRegistration() {
    AuthService service(db);

    QString error = service.registerExistingStudent("unknown@test.com", "user1", "pass1");
    QVERIFY(!error.isEmpty());
    QVERIFY(error.contains("not registered"));

    QSqlQuery query(db.getDb());
    query.prepare("INSERT INTO students (email, name) VALUES (?, ?)");
    query.addBindValue("student@test.com");
    query.addBindValue("Test Student");
    query.exec();

    QString regError = service.registerExistingStudent("student@test.com", "new_student", "pass123");
    QVERIFY2(regError.isEmpty(), regError.toStdString().c_str());
}

QTEST_MAIN(TestAuthService)
#include "tst_testauthservice.moc"
