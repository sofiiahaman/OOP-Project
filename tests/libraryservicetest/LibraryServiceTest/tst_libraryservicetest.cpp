#include <QtTest>
#include "services/LibraryService.h"
#include "database.h"
#include "basebookview.h"

class LibraryServiceTest : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase() {
        m_db.openConnection();
    }

    void testAddNewBookValidation() {
        LibraryService service(m_db);

        QCOMPARE(service.addNewBook("", "Author", "Publisher", "Category"), false);

        QCOMPARE(service.addNewBook("Title", "   ", "Publisher", "Category"), false);

    }

    void testBuildFilterString() {
        LibraryService service(m_db);
        BookSearchParams params;

        QCOMPARE(service.buildFilterString(params), QString(""));

        params.title = "Potter";
        QCOMPARE(service.buildFilterString(params), QString("title LIKE '%Potter%'"));

        params.author = "Rowling";
        params.availableOnly = true;
        QString expected = "title LIKE '%Potter%' AND author LIKE '%Rowling%' AND available = 1";
        QCOMPARE(service.buildFilterString(params), expected);
    }

    void cleanupTestCase() {
    }

private:
    Database m_db;
};

QTEST_MAIN(LibraryServiceTest)

#include "tst_libraryservicetest.moc"
