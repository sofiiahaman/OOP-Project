#include <QtTest>
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

class DatabaseTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();      // Run before all tests
    void cleanupTestCase();   // Run after all tests

    void testOpenConnection();
    void testAddUser();
    void testCheckLogin();
    void testUserExists();
    void testGetUserRole();
    void testFindStudentByEmail();
    void testLinkStudentWithUser();
};

QString testDbPath;

Database db;

void DatabaseTest::initTestCase()
{
    // 1. Create a temporary test database
    testDbPath = QCoreApplication::applicationDirPath() + "/test_database.db";
    qDebug() << "Database path: " << testDbPath;

    QFile::remove(testDbPath);

    qDebug() << "AVAILABLE DRIVERS:" << QSqlDatabase::drivers();

    // Use the db object to establish the connection
    db = Database(); // Rebuild the object
    QVERIFY(db.openConnection());

    QSqlQuery q;

    // 2. Create tables
    QVERIFY(q.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "username TEXT,"
                   "password TEXT,"
                   "role TEXT)"));

    QVERIFY(q.exec("CREATE TABLE IF NOT EXISTS students ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "email TEXT,"
                   "user_id INTEGER)"));
}


void DatabaseTest::cleanupTestCase()
{
    QFile::remove(testDbPath);
}

void DatabaseTest::testOpenConnection()
{
    QVERIFY(db.openConnection());
}

void DatabaseTest::testAddUser()
{
    db.openConnection();
    QVERIFY(db.addUser("testuser", "1234", "admin"));
}

void DatabaseTest::testCheckLogin()
{
    db.openConnection();
    QVERIFY(db.checkLogin("testuser", "1234"));
    QVERIFY(!db.checkLogin("wrong", "wrong"));
}

void DatabaseTest::testUserExists()
{
    db.openConnection();
    QVERIFY(db.userExists("testuser"));
    QVERIFY(!db.userExists("ghost"));
}

void DatabaseTest::testGetUserRole()
{
    db.openConnection();
    QCOMPARE(db.getUserRole("testuser"), QString("admin"));
}

void DatabaseTest::testFindStudentByEmail()
{
    db.openConnection();
    QSqlQuery q;
    q.exec("INSERT INTO students (email) VALUES ('student@mail.com')");
    QVERIFY(q.exec("INSERT INTO students (email) VALUES ('student@mail.com')"));

    int id = db.findStudentByEmail("student@mail.com");
    QVERIFY(id > 0);
}

void DatabaseTest::testLinkStudentWithUser()
{
    db.openConnection();

    QSqlQuery q;
    q.exec("INSERT INTO users (username,password,role) VALUES ('std','pw','student')");
    q.next();
    int userId = q.lastInsertId().toInt();

    q.exec("INSERT INTO students (email) VALUES ('std2@mail.com')");
    q.next();
    int studentId = q.lastInsertId().toInt();

    QVERIFY(db.linkStudentWithUser(studentId, userId));

    q.exec("SELECT user_id FROM students WHERE id=" + QString::number(studentId));
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), userId);
}

QTEST_MAIN(DatabaseTest)
#include "tst_databasetest.moc"
