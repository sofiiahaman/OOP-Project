#include <QtTest>
#include <QLineEdit>
#include "issuebooks.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPushButton>
#include <QTest>

class IssueBooksTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();  // Called before all tests
    void cleanupTestCase();  // Called after all tests

    void testIssueBookAvailable(); // Test for issuing an available book
    void testIssueBookNotAvailable(); // Test for issuing an unavailable book
    void testIssueBookSuccess(); // Test for successful book issuance
    void testIssueBookError(); // Test for error (unsuccessful book issuance)

private:
    IssueBooks *issueBooks;
    Database *db;
    QString testDbPath;
};

QString testDbPath;

void IssueBooksTest::initTestCase()
{
    testDbPath = QCoreApplication::applicationDirPath() + "/test_database.db";
    QFile::remove(testDbPath);

    db = new Database();
    QVERIFY(db->openConnection());

    issueBooks = new IssueBooks();

    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS books (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, available INTEGER)");
    q.exec("CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, email TEXT)");
    q.exec("CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, book_id INTEGER, student_id INTEGER, issue_date TEXT, return_date TEXT)");

    // Adding test data
    q.exec("INSERT INTO books (title, available) VALUES ('Test Book', 1)");
    q.exec("INSERT INTO students (name, email) VALUES ('John Doe', 'john.doe@example.com')");
}

void IssueBooksTest::cleanupTestCase()
{
    QFile::remove(testDbPath);
}

void IssueBooksTest::testIssueBookAvailable()
{
    // 0. PREPARATION: The book should be available (1) and have no transactions.
    QSqlQuery resetQuery;
    resetQuery.exec("UPDATE books SET available = 1 WHERE id = 1");
    resetQuery.exec("DELETE FROM transactions WHERE book_id = 1");

    // 1. FIND UI ELEMENTS
    QLineEdit *studentNameLine = issueBooks->findChild<QLineEdit*>("studentNameLineEdit");
    QLineEdit *bookNameLine = issueBooks->findChild<QLineEdit*>("bookNameLineEdit");
    QLineEdit *bookIdLine = issueBooks->findChild<QLineEdit*>("bookIdLineEdit");
    QPushButton *issueButton = issueBooks->findChild<QPushButton*>("issueBookButton");
    QVERIFY(issueButton != nullptr && studentNameLine != nullptr && bookNameLine != nullptr && bookIdLine != nullptr);

    // 2. SET TEXT DIRECTLY
    studentNameLine->setText("John Doe");
    bookNameLine->setText("Test Book");
    bookIdLine->setText("1");

    // 3. SIMULATE BUTTON CLICK
    QTest::mouseClick(issueButton, Qt::LeftButton);

    // 4. CHECK A: Was the book set to unavailable (Actual: 0, Expected: 0)
    QSqlQuery q;
    q.exec("SELECT available FROM books WHERE id = 1");
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 0);  // The book should be unavailable (0)

    // 5. CHECK B: Was one record created in transactions (Actual: 1, Expected: 1)
    q.exec("SELECT COUNT(*) FROM transactions WHERE book_id = 1");
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 1); // There should be 1 record
}

void IssueBooksTest::testIssueBookNotAvailable()
{
    // 0. PREPARATION: The book is UNAVAILABLE (0) and there is 1 record in transactions.
    QSqlQuery resetQuery;
    resetQuery.exec("UPDATE books SET available = 0 WHERE id = 1");

    // Create a base transaction to check that a second one is not added
    resetQuery.exec("DELETE FROM transactions WHERE book_id = 1");
    resetQuery.exec("INSERT INTO transactions (book_id, student_id, issue_date, return_date) "
                    "VALUES (1, 1, '2025-01-01', NULL)");

    // 1. FIND UI ELEMENTS
    QLineEdit *studentNameLine = issueBooks->findChild<QLineEdit*>("studentNameLineEdit");
    QLineEdit *bookNameLine = issueBooks->findChild<QLineEdit*>("bookNameLineEdit");
    QLineEdit *bookIdLine = issueBooks->findChild<QLineEdit*>("bookIdLineEdit");
    QPushButton *issueButton = issueBooks->findChild<QPushButton*>("issueBookButton");
    QVERIFY(issueButton != nullptr && studentNameLine != nullptr && bookNameLine != nullptr && bookIdLine != nullptr);

    // 2. SET TEXT DIRECTLY
    studentNameLine->setText("John Doe");
    bookNameLine->setText("Test Book");
    bookIdLine->setText("1");

    // 3. SIMULATE USER ACTION (attempt to issue)
    QTest::mouseClick(issueButton, Qt::LeftButton);

    // 4. CHECK A: The book should remain unavailable (0)
    QSqlQuery q;
    q.exec("SELECT available FROM books WHERE id = 1");
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 0);

    // 5. CHECK B: The number of records in transactions should be 1 (not increased)
    q.exec("SELECT COUNT(*) FROM transactions WHERE book_id = 1");
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 1); // It should remain 1 record
}

void IssueBooksTest::testIssueBookSuccess()
{
    // 0. PREPARATION: The book should be available (1) and have no transactions.
    QSqlQuery resetQuery;
    resetQuery.exec("UPDATE books SET available = 1 WHERE id = 1");
    resetQuery.exec("DELETE FROM transactions WHERE book_id = 1");

    // 1. FIND UI ELEMENTS
    QLineEdit *studentNameLine = issueBooks->findChild<QLineEdit*>("studentNameLineEdit");
    QLineEdit *bookNameLine = issueBooks->findChild<QLineEdit*>("bookNameLineEdit");
    QLineEdit *bookIdLine = issueBooks->findChild<QLineEdit*>("bookIdLineEdit");
    QPushButton *issueButton = issueBooks->findChild<QPushButton*>("issueBookButton");

    QVERIFY(issueButton != nullptr && studentNameLine != nullptr && bookNameLine != nullptr && bookIdLine != nullptr);

    // 2. SET TEXT DIRECTLY
    studentNameLine->setText("John Doe");
    bookNameLine->setText("Test Book");
    bookIdLine->setText("1");

    // 3. SIMULATE USER ACTION
    QTest::mouseClick(issueButton, Qt::LeftButton);

    // 4. CHECK A: Was one record created in transactions (Actual: 1, Expected: 1)
    QSqlQuery q;
    q.exec("SELECT COUNT(*) FROM transactions WHERE book_id = 1");

    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 1); // Successful issuance creates 1 record

    // 5. CHECK B: Was the book set to unavailable (Actual: 0, Expected: 0)
    q.exec("SELECT available FROM books WHERE id = 1");
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 0);
}

void IssueBooksTest::testIssueBookError()
{
    // 0. PREPARATION: Clear transactions and ensure that "John Doe" student is not found
    QSqlQuery resetQuery;
    resetQuery.exec("DELETE FROM transactions");
    // Change the name of the only student so that IssueBooks logic doesn't find "John Doe"
    resetQuery.exec("UPDATE students SET name = 'Different Name' WHERE id = 1");

    // 1. FIND UI ELEMENTS
    QLineEdit *studentNameLine = issueBooks->findChild<QLineEdit*>("studentNameLineEdit");
    QLineEdit *bookNameLine = issueBooks->findChild<QLineEdit*>("bookNameLineEdit");
    QLineEdit *bookIdLine = issueBooks->findChild<QLineEdit*>("bookIdLineEdit");
    QPushButton *issueButton = issueBooks->findChild<QPushButton*>("issueBookButton");
    QVERIFY(issueButton != nullptr && studentNameLine != nullptr && bookNameLine != nullptr && bookIdLine != nullptr);

    // 2. SET TEXT DIRECTLY
    // "John Doe" student doesn't exist, this should trigger an error in IssueBooks
    studentNameLine->setText("John Doe");
    bookNameLine->setText("Test Book");
    bookIdLine->setText("1");

    // 3. SIMULATE BUTTON CLICK
    QTest::mouseClick(issueButton, Qt::LeftButton);

    // 4. CHECK: Was no record created in transactions (Actual: 0, Expected: 0)
    QSqlQuery q;
    q.exec("SELECT COUNT(*) FROM transactions");

    QVERIFY(q.next());
    // EXPECTATION: There should be 0 since issuance failed due to missing student

    QCOMPARE(q.value(0).toInt(), 0); // No transaction record should have been created
}

QTEST_MAIN(IssueBooksTest)
#include "tst_issuebookstest.moc"
