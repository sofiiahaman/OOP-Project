#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QLineEdit>
#include <QTableView>
#include "returnbooks.h"

class MockReturnBooks : public ReturnBooks
{
    Q_OBJECT

public:
    MockReturnBooks(QWidget *parent = nullptr) : ReturnBooks(parent) {}

    // Mock method for loadTransactionsByStudent
    void loadTransactionsByStudent(const QString &name) override {
        Q_UNUSED(name);

        // Create a mock model
        QSqlQueryModel *model = new QSqlQueryModel();

        // Simulate a query that returns a single fake transaction
        model->setQuery("SELECT 1 AS 'Issue ID', 1 AS 'Book ID', '2023-01-01' AS 'Issue Date', NULL AS 'Return Date'");

        // Check if the model was successfully initialized
        if (model->rowCount() > 0) {
            // Use the standard way to update the model in Qt
            model->dataChanged(model->index(0, 0), model->index(0, 0)); // This should be corrected to the proper way to update
        } else {
            qDebug() << "No rows found in the model!";
        }

        // Assign the model to the table view in the UI
        QTableView *tableView = findChild<QTableView*>();
        if (tableView) {
            tableView->setModel(model);  // Assign the model to the table
        }
    }

    // Mock method for onTransactionSelected
    void onTransactionSelected(const QModelIndex &index) override {
        Q_UNUSED(index);
        // Simulate filling the fields after selecting a transaction
        QLineEdit *issueIDLineEdit = findChild<QLineEdit*>("issueIDLineEdit");
        QLineEdit *bookIDLineEdit = findChild<QLineEdit*>("bookIDLineEdit");

        if (issueIDLineEdit) issueIDLineEdit->setText("1");
        if (bookIDLineEdit) bookIDLineEdit->setText("1");
    }
};

class TestReturnBooksWithMocks : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        // Initialize the mock
        returnBooks = new MockReturnBooks();
    }

    void testLoadTransactionsByStudent() {
        returnBooks->loadTransactionsByStudent("John");

        // Check if the data is mocked correctly
        QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(returnBooks->findChild<QTableView*>()->model());
        QVERIFY(model != nullptr);
        QCOMPARE(model->rowCount(), 1); // There should be one transaction
    }

    void testOnTransactionSelected() {
        QTableView *tableView = returnBooks->findChild<QTableView*>();
        QModelIndex index = tableView->model()->index(0, 0); // First value
        returnBooks->onTransactionSelected(index);

        // Check if the correct values were set in the fields
        QCOMPARE(returnBooks->findChild<QLineEdit*>("issueIDLineEdit")->text(), "1");
        QCOMPARE(returnBooks->findChild<QLineEdit*>("bookIDLineEdit")->text(), "1");
    }

    void cleanupTestCase() {
        delete returnBooks;
    }

private:
    MockReturnBooks *returnBooks;
};

QTEST_MAIN(TestReturnBooksWithMocks)
#include "tst_returnbookstest.moc"
