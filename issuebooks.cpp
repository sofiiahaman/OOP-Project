#include "issuebooks.h"
#include "ui_issuebooks.h"
#include "adminmenu.h"

#include "database.h"
#include "repositories/StudentRepository.h"
#include "repositories/BookRepository.h"
#include "repositories/TransactionRepository.h"

#include <QMessageBox>
#include <QDate>
#include <QCompleter>
#include <QMap>

IssueBooks::IssueBooks(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::IssueBooks)
{
    ui->setupUi(this);

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));
    ui->goBackButton->setIconSize(QSize(20, 20));

    loadStudentHints();
    loadBookHints();
}

IssueBooks::~IssueBooks()
{
    delete ui;
}

void IssueBooks::loadStudentHints()
{
    Database database;
    StudentRepository repo(database);

    QSqlQueryModel* model = repo.getAllStudents();
    if (!model)
        return;

    QStringList students;

    for (int i = 0; i < model->rowCount(); ++i) {
        QString name = model->data(model->index(i, 1)).toString();  // name
        QString email = model->data(model->index(i, 2)).toString(); // email

        students << name;
        studentEmails[name] = email;
    }

    QCompleter *completer = new QCompleter(students, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    ui->studentNameLineEdit->setCompleter(completer);

    connect(completer,
            QOverload<const QString &>::of(&QCompleter::activated),
            this,
            &IssueBooks::fillStudentEmail);

    connect(ui->studentNameLineEdit,
            &QLineEdit::textChanged,
            this,
            &IssueBooks::fillStudentEmail);
}

void IssueBooks::fillStudentEmail(const QString &name)
{
    if (studentEmails.contains(name))
        ui->studentEmailLineEdit->setText(studentEmails[name]);
}

void IssueBooks::loadBookHints()
{
    Database database;
    BookRepository repo(database);

    QSqlQueryModel* model = repo.getAllBooks();
    if (!model)
        return;

    QStringList titles;

    for (int i = 0; i < model->rowCount(); ++i) {
        QString title = model->data(model->index(i, 1)).toString();
        int id = model->data(model->index(i, 0)).toInt();

        titles << title;
        bookIds[title] = id;
    }

    QCompleter *completer = new QCompleter(titles, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    ui->bookNameLineEdit->setCompleter(completer);

    connect(completer,
            QOverload<const QString &>::of(&QCompleter::activated),
            this,
            &IssueBooks::fillBookId);

    connect(ui->bookNameLineEdit,
            &QLineEdit::textChanged,
            this,
            &IssueBooks::fillBookId);
}

void IssueBooks::fillBookId(const QString &title)
{
    if (bookIds.contains(title))
        ui->bookIdLineEdit->setText(QString::number(bookIds[title]));
}

void IssueBooks::on_issueBookButton_clicked()
{
    QString studentName = ui->studentNameLineEdit->text().trimmed();
    QString bookIdStr = ui->bookIdLineEdit->text().trimmed();
    QString issueDate = ui->issueDateLineEdit->text().trimmed();

    if (studentName.isEmpty() || bookIdStr.isEmpty()) {
        QMessageBox::warning(this,
                             "Input Error",
                             "Please fill in all required fields.");
        return;
    }

    if (issueDate.isEmpty())
        issueDate = QDate::currentDate().toString("yyyy-MM-dd");

    int bookId = bookIdStr.toInt();

    Database database;
    StudentRepository studentRepo(database);
    TransactionRepository transactionRepo(database);

    // 1️⃣ Get student ID
    int studentId = studentRepo.findStudentByName(studentName);

    if (studentId == -1) {
        QMessageBox::warning(this,
                             "Not Found",
                             "No student found with this name.");
        return;
    }

    // 2️⃣ Issue via repository
    if (!transactionRepo.issueBook(studentId, bookId, issueDate)) {
        QMessageBox::warning(this,
                             "Error",
                             "Failed to issue book or book unavailable.");
        return;
    }

    QMessageBox::information(this,
                             "Success",
                             "Book issued successfully!");

    clearFields();
}

void IssueBooks::on_cancelButton_clicked()
{
    clearFields();
}

void IssueBooks::on_goBackButton_clicked()
{
    this->close();

#ifndef UNIT_TEST
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
#endif
}

void IssueBooks::clearFields()
{
    ui->studentNameLineEdit->clear();
    ui->studentEmailLineEdit->clear();
    ui->bookNameLineEdit->clear();
    ui->bookIdLineEdit->clear();
    ui->issueDateLineEdit->clear();
}
