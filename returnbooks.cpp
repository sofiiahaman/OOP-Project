#include "returnbooks.h"
#include "ui_returnbooks.h"
#include "adminmenu.h"
#include "database.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QCompleter>
#include <QStringListModel>

ReturnBooks::ReturnBooks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReturnBooks)
{
    ui->setupUi(this);

    ui->goBackButton_5->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    Database db;
    if (!db.openConnection()) {
        qDebug() << "Failed to open database!";
        return;
    }

    // Add dropdown list for studentNameLineEdit
    QSqlQuery query;
    query.prepare("SELECT name FROM students");

    if (query.exec()) {
        QStringList studentNames;
        while (query.next()) {
            studentNames << query.value(0).toString();  // Add student name to the list
        }

        // Check if list is empty
        if (studentNames.isEmpty()) {
            qDebug() << "Student list is empty!";
        } else {
            // Create a model for autocomplete
            QStringListModel *model = new QStringListModel(this);
            model->setStringList(studentNames);

            // Connect model to completer
            QCompleter *completer = new QCompleter(model, this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            completer->setFilterMode(Qt::MatchContains);
            ui->studentNameLineEdit->setCompleter(completer);
        }
    } else {
        qDebug() << "Error executing student query: " << query.lastError();
    }

    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);

    connect(ui->studentNameLineEdit, &QLineEdit::textEdited, this, [=](const QString &text){
        searchTimer->start(300); // 300 ms delay
    });

    connect(searchTimer, &QTimer::timeout, this, [=](){
        loadTransactionsByStudent(ui->studentNameLineEdit->text());
    });

    connect(ui->tableView, &QTableView::clicked,
            this, &ReturnBooks::onTransactionSelected);

    connect(ui->returnButton, &QPushButton::clicked,
            this, &ReturnBooks::onReturnClicked);

    connect(ui->clearButton, &QPushButton::clicked, this, &ReturnBooks::clearFields);

}

ReturnBooks::~ReturnBooks()
{
    delete ui;
}

/* ========================
 * Loading transactions
 * ======================== */
void ReturnBooks::loadTransactionsByStudent(const QString &name)
{
    QString trimmed = name.trimmed();

    if(trimmed.length() < 2)
    {
        ui->tableView->setModel(nullptr);
        return;
    }

    if (!QSqlDatabase::database().isOpen()) {
        if (!QSqlDatabase::database().open()) {
            qDebug() << "Database not open!";
            return;
        }
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM students WHERE LOWER(name) LIKE LOWER(:name)");
    query.bindValue(":name", "%" + trimmed + "%");

    if(!query.exec() || !query.next())
    {
        ui->tableView->setModel(nullptr);
        return;
    }

    int studentId = query.value(0).toInt();

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(QString(
                        "SELECT id AS 'Issue ID', book_id AS 'Book ID', issue_date AS 'Issue Date', return_date AS 'Return Date' "
                        "FROM transactions WHERE student_id = %1 AND return_date IS NULL")
                        .arg(studentId));

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}



/* ========================
 * Selecting a transaction
 * ======================== */
void ReturnBooks::onTransactionSelected(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    int row = index.row();
    QAbstractItemModel *model = ui->tableView->model();

    int transactionId = model->index(row, 0).data().toInt();
    int bookId = model->index(row, 1).data().toInt();

    QSqlQuery query;
    query.prepare(R"(
        SELECT t.issue_date,
               s.name,
               s.email,
               b.title,
               b.author
        FROM transactions t
        JOIN students s ON t.student_id = s.id
        JOIN books b ON t.book_id = b.id
        WHERE t.id = :tid
    )");

    query.bindValue(":tid", transactionId);

    if(query.exec() && query.next())
    {
        ui->issueIDLineEdit->setText(QString::number(transactionId));
        ui->bookIDLineEdit->setText(QString::number(bookId));
        ui->studentNameLineEdit->setText(query.value("name").toString());
        ui->studentEmailLineEdit->setText(query.value("email").toString());
        ui->bookTitleLineEdit->setText(query.value("title").toString());
        ui->authorLineEdit->setText(query.value("author").toString());
        ui->issueDateLineEdit->setText(query.value("issue_date").toString());
    }
}

/* ========================
 * Returning a book
 * ======================== */
void ReturnBooks::onReturnClicked()
{
    if(ui->issueIDLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Select a transaction to return.");
        return;
    }

    int transactionId = ui->issueIDLineEdit->text().toInt();
    int bookId = ui->bookIDLineEdit->text().toInt();

    QSqlDatabase::database().transaction();

    QSqlQuery query;

    // Update return_date
    query.prepare("UPDATE transactions SET return_date = :rd WHERE id = :id");
    query.bindValue(":rd", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":id", transactionId);

    if(!query.exec())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Error", "Failed to update transaction.");
        return;
    }

    // Make book available again
    query.prepare("UPDATE books SET available = 1 WHERE id = :bid");
    query.bindValue(":bid", bookId);

    if(!query.exec())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Error", "Failed to update book.");
        return;
    }

    QSqlDatabase::database().commit();

    QMessageBox::information(this, "Success", "Book successfully returned!");

    clearFields();
    loadTransactionsByStudent(ui->studentNameLineEdit->text());
}

/* ========================
 * Clearing input fields
 * ======================== */
void ReturnBooks::clearFields()
{
    ui->issueIDLineEdit->clear();
    ui->bookIDLineEdit->clear();
    ui->studentNameLineEdit->clear();
    ui->studentEmailLineEdit->clear();
    ui->bookTitleLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->issueDateLineEdit->clear();

    ui->tableView->setModel(nullptr);
}

void ReturnBooks::on_goBackButton_5_clicked()
{
    #ifndef UNIT_TEST
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    #endif
    this->close();
}
