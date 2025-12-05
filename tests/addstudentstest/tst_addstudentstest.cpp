#include <QtTest>
#include "addstudents.h"
#include "database.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QApplication>
#include <QSqlDatabase>
#include <QLineEdit>
#include <QPushButton>

class MockDatabase : public Database {
public:
    bool openConnection() override {
        if (QSqlDatabase::contains("qt_sql_default_connection")) {
            QSqlDatabase::removeDatabase("qt_sql_default_connection");
        }

        // Establish a new in-memory connection
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName(":memory:");

        if (!db.open()) {
            qDebug() << "MockDatabase failed to open:" << db.lastError().text();
            return false;
        }
        return true;
    }

    void closeConnection() override {
        if (QSqlDatabase::contains("qt_sql_default_connection")) {
            QSqlDatabase::removeDatabase("qt_sql_default_connection");
        }
    }

    // Function to get the current mock connection
    QSqlDatabase connection() const {
        return QSqlDatabase::database("qt_sql_default_connection");
    }
};

class TestAddStudents : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testInitialization();
    void testSaveButton_validInput();
    void testSaveButton_emptyInput();
    void testCancelButton();
    void testGoBackButton();

private:
    AddStudents *addStudents = nullptr;
};

void TestAddStudents::initTestCase()
{
    // Cleanup before all tests
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void TestAddStudents::cleanupTestCase()
{
    // Clean up AddStudents object if it still exists
    if (addStudents) {
        delete addStudents;
        addStudents = nullptr;
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void TestAddStudents::testInitialization()
{
    addStudents = new AddStudents();
    QVERIFY(addStudents->findChild<QPushButton*>("goBackButton_2") != nullptr);
    QVERIFY(addStudents->findChild<QPushButton*>("saveButton_2") != nullptr);
    QVERIFY(addStudents->findChild<QPushButton*>("cancelButton_2") != nullptr);

    delete addStudents;
    addStudents = nullptr;
}

void TestAddStudents::testSaveButton_validInput()
{
    addStudents = new AddStudents();

    addStudents->findChild<QLineEdit*>("studentNameLineEdit_2")->setText("John Doe");
    addStudents->findChild<QLineEdit*>("studentEmailLineEdit_2")->setText("johndoe@example.com");

    MockDatabase db;
    if (!db.openConnection()) {
        QFAIL("Failed to connect to the database.");
        return;
    }

    // Get explicit connection
    QSqlDatabase mockDb = db.connection();

    // Create "students" table, passing the connection
    QSqlQuery createQuery(mockDb);
    createQuery.prepare("CREATE TABLE students (id INTEGER PRIMARY KEY, name TEXT, email TEXT)");
    if (!createQuery.exec()) {
        qDebug() << "Failed to create table:" << createQuery.lastError().text();
        QFAIL("Failed to create table.");
        db.closeConnection();
        return;
    }

    // Simulate clicking the "Save" button
    QTest::mouseClick(addStudents->findChild<QPushButton*>("saveButton_2"), Qt::LeftButton);

    // INSERT CHECK: Use explicit connection for the query
    QSqlQuery selectQuery(mockDb);
    selectQuery.prepare("SELECT name, email FROM students WHERE name = ?");
    selectQuery.addBindValue("John Doe");

    // Check if the query executes and show error
    QVERIFY2(selectQuery.exec(), qPrintable(QString("Помилка SELECT: %1").arg(selectQuery.lastError().text())));

    // Check that at least one row is found
    QVERIFY(selectQuery.next());

    // Verify that the data is correct
    QCOMPARE(selectQuery.value("name").toString(), QString("John Doe"));
    QCOMPARE(selectQuery.value("email").toString(), QString("johndoe@example.com"));

    // Check if fields were cleared (indicating successful insert)
    QVERIFY(addStudents->findChild<QLineEdit*>("studentNameLineEdit_2")->text().isEmpty());
    QVERIFY(addStudents->findChild<QLineEdit*>("studentEmailLineEdit_2")->text().isEmpty());

    db.closeConnection();
    delete addStudents;
    addStudents = nullptr;
}

void TestAddStudents::testSaveButton_emptyInput()
{
    addStudents = new AddStudents();

    addStudents->findChild<QLineEdit*>("studentNameLineEdit_2")->clear();
    addStudents->findChild<QLineEdit*>("studentEmailLineEdit_2")->clear();

    QTest::mouseClick(addStudents->findChild<QPushButton*>("saveButton_2"), Qt::LeftButton);

    // Check if fields remain empty
    QVERIFY(addStudents->findChild<QLineEdit*>("studentNameLineEdit_2")->text().isEmpty());
    QVERIFY(addStudents->findChild<QLineEdit*>("studentEmailLineEdit_2")->text().isEmpty());

    delete addStudents;
    addStudents = nullptr;
}

void TestAddStudents::testCancelButton()
{
    addStudents = new AddStudents();

    addStudents->findChild<QLineEdit*>("studentNameLineEdit_2")->setText("John Doe");
    addStudents->findChild<QLineEdit*>("studentEmailLineEdit_2")->setText("johndoe@example.com");

    QTest::mouseClick(addStudents->findChild<QPushButton*>("cancelButton_2"), Qt::LeftButton);

    // Check if fields are cleared
    QVERIFY(addStudents->findChild<QLineEdit*>("studentNameLineEdit_2")->text().isEmpty());
    QVERIFY(addStudents->findChild<QLineEdit*>("studentEmailLineEdit_2")->text().isEmpty());

    delete addStudents;
    addStudents = nullptr;
}

QTEST_MAIN(TestAddStudents)

#include "tst_addstudentstest.moc"
