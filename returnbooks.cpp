#include "returnbooks.h"
#include "ui_returnbooks.h"
#include "adminmenu.h"

#include "database.h"
#include "repositories/StudentRepository.h"
#include "repositories/TransactionRepository.h"
#include "services/booktransactionservice.h"

#include <QMessageBox>
#include <QCompleter>
#include <QTimer>
#include <QStringList>
#include <QSqlQuery>

ReturnBooks::ReturnBooks(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ReturnBooks)
{
    ui->setupUi(this);

    ui->goBackButton_5->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    setupAutocompleter();

    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);

    connect(ui->studentNameLineEdit, &QLineEdit::textEdited, this, [=](){
        searchTimer->start(300);
    });

    connect(searchTimer, &QTimer::timeout, this, [=](){
        loadTransactionsByStudent(ui->studentNameLineEdit->text());
    });

    connect(ui->tableView, &QTableView::clicked,
            this, &ReturnBooks::onTransactionSelected);

    connect(ui->returnButton, &QPushButton::clicked,
            this, &ReturnBooks::onReturnClicked);

    connect(ui->clearButton, &QPushButton::clicked,
            this, &ReturnBooks::clearFields);
}

ReturnBooks::~ReturnBooks()
{
    delete ui;
}

void ReturnBooks::setupAutocompleter()
{
    Database database;
    StudentRepository repo(database);

    QSqlQueryModel* model = repo.getAllStudents();
    if (!model)
        return;

    QStringList names;

    for (int i = 0; i < model->rowCount(); ++i) {
        names << model->data(model->index(i, 1)).toString();
    }

    QCompleter *completer = new QCompleter(names, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);

    ui->studentNameLineEdit->setCompleter(completer);
}


void ReturnBooks::loadTransactionsByStudent(const QString &name)
{
    QString trimmed = name.trimmed();

    if (trimmed.length() < 2) {
        ui->tableView->setModel(nullptr);
        return;
    }

    Database database;
    StudentRepository studentRepo(database);
    TransactionRepository transactionRepo(database);

    int studentId = studentRepo.findStudentByName(trimmed);

    if (studentId == -1) {
        ui->tableView->setModel(nullptr);
        return;
    }

    QSqlQueryModel* model = transactionRepo.getStudentBooks(studentId);

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void ReturnBooks::onTransactionSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();
    auto model = ui->tableView->model();

    int transactionId = model->index(row, 0).data().toInt();
    int bookId = model->index(row, 1).data().toInt();

    Database database;
    TransactionRepository repo(database);
    QSqlQuery query = repo.getTransactionDetails(transactionId);

    if (query.next()) {
        ui->issueIDLineEdit->setText(QString::number(transactionId));
        ui->bookIDLineEdit->setText(QString::number(bookId));

        ui->studentNameLineEdit->setText(query.value("name").toString());
        ui->studentEmailLineEdit->setText(query.value("email").toString());
        ui->bookTitleLineEdit->setText(query.value("title").toString());
        ui->authorLineEdit->setText(query.value("author").toString());
        ui->issueDateLineEdit->setText(query.value("issue_date").toString());
    }
}

void ReturnBooks::onReturnClicked()
{
    if (ui->issueIDLineEdit->text().isEmpty()) {
        QMessageBox::warning(this,
                             "Error",
                             "Select a transaction to return.");
        return;
    }

    int transactionId = ui->issueIDLineEdit->text().toInt();

    Database database;
    BookTransactionService service(database);

    if (service.returnBook(transactionId)) {
        QMessageBox::information(this, "Success", "The book has been successfully returned!");
        clearFields();
        loadTransactionsByStudent(ui->studentNameLineEdit->text());
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to process the return.");
    }
}

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
