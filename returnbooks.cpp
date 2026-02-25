#include "returnbooks.h"
#include "ui_returnbooks.h"
#include "adminmenu.h"
#include "database.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>

ReturnBooks::ReturnBooks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReturnBooks)
{
    ui->setupUi(this);

    ui->goBackButton_5->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    setupAutocompleter();

    // Timer setup for live search
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);

    connect(ui->studentNameLineEdit, &QLineEdit::textEdited, this, [=](){
        searchTimer->start(300);
    });

    connect(searchTimer, &QTimer::timeout, this, [=](){
        loadTransactionsByStudent(ui->studentNameLineEdit->text());
    });

    connect(ui->tableView, &QTableView::clicked, this, &ReturnBooks::onTransactionSelected);
    connect(ui->returnButton, &QPushButton::clicked, this, &ReturnBooks::onReturnClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &ReturnBooks::clearFields);

}

ReturnBooks::~ReturnBooks()
{
    delete ui;
}

void ReturnBooks::setupAutocompleter()
{
    Database db;
    if (!db.openConnection()) return;

    QSqlQuery query("SELECT name FROM students");
    QStringList studentNames;
    while (query.next()) {
        studentNames << query.value(0).toString();
    }

    QCompleter *completer = new QCompleter(studentNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->studentNameLineEdit->setCompleter(completer);
}

/* ========================
 * Loading transactions
 * ======================== */
void ReturnBooks::loadTransactionsByStudent(const QString &name)
{
    QString trimmed = name.trimmed();
    if(trimmed.length() < 2) {
        ui->tableView->setModel(nullptr);
        return;
    }

    Database db;
    if (!db.openConnection()) return;

    QSqlQuery query;
    query.prepare("SELECT id FROM students WHERE LOWER(name) LIKE LOWER(:name)");
    query.bindValue(":name", "%" + trimmed + "%");

    if(query.exec() && query.next()) {
        int studentId = query.value(0).toInt();
        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(QString("SELECT id AS 'Issue ID', book_id AS 'Book ID', issue_date AS 'Date' "
                                "FROM transactions WHERE student_id = %1 AND return_date IS NULL").arg(studentId));
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    }
}



/* ========================
 * Selecting a transaction
 * ======================== */
void ReturnBooks::onTransactionSelected(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    int row = index.row();
    auto model = ui->tableView->model();

    int transactionId = model->index(row, 0).data().toInt();
    int bookId = model->index(row, 1).data().toInt();

    Database db;
    db.openConnection();
    QSqlQuery query;
    query.prepare("SELECT t.issue_date, s.name, s.email, b.title, b.author "
                  "FROM transactions t JOIN students s ON t.student_id = s.id "
                  "JOIN books b ON t.book_id = b.id WHERE t.id = :tid");

    query.bindValue(":tid", transactionId);

    if(query.exec() && query.next()) {
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

    int tId = ui->issueIDLineEdit->text().toInt();
    int bId = ui->bookIDLineEdit->text().toInt();

    Database db;
    if(db.returnBookTransaction(tId, bId)) {
        QMessageBox::information(this, "Success", "The book has been successfully returned!");
        clearFields();
        loadTransactionsByStudent(ui->studentNameLineEdit->text());
    } else {
        QMessageBox::critical(this, "Error", "Failed to process the return.");
    }
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
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}
