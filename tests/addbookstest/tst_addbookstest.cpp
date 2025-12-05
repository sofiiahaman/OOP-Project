#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "addbooks.h"
#include "ui_addbooks.h"

class TestAddBooks : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_saveButton_noFieldsFilled();
    void test_saveButton_withValidData();
    void test_cancelButton();
    void test_goBackButton();

private:
    AddBooks *addBooksWindow = nullptr;

    QLineEdit *nameEdit = nullptr;
    QLineEdit *authorEdit = nullptr;
    QLineEdit *publisherEdit = nullptr;
    QLineEdit *categoryEdit = nullptr;
};

void TestAddBooks::initTestCase()
{
    addBooksWindow = new AddBooks(true);
    addBooksWindow->show();

    nameEdit      = addBooksWindow->findChild<QLineEdit*>("bookNameLineEdit");
    authorEdit    = addBooksWindow->findChild<QLineEdit*>("bookAuthorLineEdit");
    publisherEdit = addBooksWindow->findChild<QLineEdit*>("bookPublisherLineEdit");
    categoryEdit  = addBooksWindow->findChild<QLineEdit*>("bookCategoryLineEdit");

    QVERIFY(nameEdit);
    QVERIFY(authorEdit);
    QVERIFY(publisherEdit);
    QVERIFY(categoryEdit);
}

void TestAddBooks::cleanupTestCase()
{
    delete addBooksWindow;
    addBooksWindow = nullptr;
}

void TestAddBooks::test_saveButton_noFieldsFilled()
{
    nameEdit->clear();
    authorEdit->clear();
    publisherEdit->clear();
    categoryEdit->clear();

    QTest::mouseClick(addBooksWindow->getSaveButton(), Qt::LeftButton);

    QCOMPARE(nameEdit->text(), QString());
    QCOMPARE(authorEdit->text(), QString());
    QCOMPARE(publisherEdit->text(), QString());
    QCOMPARE(categoryEdit->text(), QString());
}

void TestAddBooks::test_saveButton_withValidData()
{
    nameEdit->setText("Test Book");
    authorEdit->setText("A1");
    publisherEdit->setText("TestPub");
    categoryEdit->setText("Fiction");

    QTest::mouseClick(addBooksWindow->getSaveButton(), Qt::LeftButton);

    QTest::qWait(150);

    QCOMPARE(nameEdit->text(), QString());
    QCOMPARE(authorEdit->text(), QString());
    QCOMPARE(publisherEdit->text(), QString());
    QCOMPARE(categoryEdit->text(), QString());

    QSqlDatabase db = QSqlDatabase::database("test_conn");
    QVERIFY(db.isOpen());

    QSqlQuery check(db);
    QVERIFY(check.exec("SELECT COUNT(*) FROM books"));

    check.next();
    QCOMPARE(check.value(0).toInt(), 1);
}

void TestAddBooks::test_cancelButton()
{
    nameEdit->setText("x");
    authorEdit->setText("x");
    publisherEdit->setText("x");
    categoryEdit->setText("x");

    QTest::mouseClick(addBooksWindow->getCancelButton(), Qt::LeftButton);

    QCOMPARE(nameEdit->text(), QString());
    QCOMPARE(authorEdit->text(), QString());
    QCOMPARE(publisherEdit->text(), QString());
    QCOMPARE(categoryEdit->text(), QString());
}

void TestAddBooks::test_goBackButton()
{
    addBooksWindow->show();
    QVERIFY(addBooksWindow->isVisible());

    QTest::mouseClick(addBooksWindow->getGoBackButton(), Qt::LeftButton);

    QCOMPARE(addBooksWindow->isVisible(), false);
}

QTEST_MAIN(TestAddBooks)
#include "tst_addbookstest.moc"
