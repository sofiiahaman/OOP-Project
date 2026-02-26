#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "services/booktransactionservice.h"
#include "database.h"

class BookTransactionServiceTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testIssueBookSuccess();
    void testIssueBookStudentNotFound();
    void testReturnBookSuccess();

private:
    Database m_db;
};

void BookTransactionServiceTest::initTestCase() {
    m_db.openConnection();
    QSqlQuery query(m_db.getDb());

    query.exec("INSERT INTO students (name, email) VALUES ('Test Student', 'test@test.com')");
    query.exec("INSERT INTO books (title, author, available) VALUES ('Test Book', 'Author', 1)");
}

void BookTransactionServiceTest::testIssueBookSuccess() {
    BookTransactionService service(m_db);

    bool result = service.issueBook("Test Student", 1, "2026-02-26");

    QVERIFY(result);

    QSqlQuery check(m_db.getDb());
    check.exec("SELECT available FROM books WHERE id = 1");
    if (check.next()) {
        QCOMPARE(check.value(0).toInt(), 0);
    }
}

void BookTransactionServiceTest::testIssueBookStudentNotFound() {
    BookTransactionService service(m_db);

    bool result = service.issueBook("Non Existent", 1, "2026-02-26");

    QCOMPARE(result, false);
}

void BookTransactionServiceTest::testReturnBookSuccess() {
    BookTransactionService service(m_db);

    bool result = service.returnBook(1);

    QVERIFY(result);

    QSqlQuery check(m_db.getDb());
    check.exec("SELECT available FROM books WHERE id = 1");
    if (check.next()) {
        QCOMPARE(check.value(0).toInt(), 1);
    }
}

void BookTransactionServiceTest::cleanupTestCase() {
    QSqlQuery query(m_db.getDb());
    query.exec("DELETE FROM transactions");
    query.exec("DELETE FROM students");
    query.exec("DELETE FROM books");
}

QTEST_MAIN(BookTransactionServiceTest)

#include "tst_booktransactionservicetest.moc"
