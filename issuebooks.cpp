#include "issuebooks.h"
#include "ui_issuebooks.h"
#include "adminmenu.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

IssueBooks::IssueBooks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IssueBooks)
{
    ui->setupUi(this);

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));
    ui->goBackButton->setIconSize(QSize(20, 20));
}

IssueBooks::~IssueBooks()
{
    delete ui;
}

void IssueBooks::on_issueBookButton_clicked()
{
    QString studentName = ui->studentNameLineEdit->text().trimmed();
    QString studentEmail = ui->studentEmailLineEdit->text().trimmed();
    QString bookName = ui->bookNameLineEdit->text().trimmed();
    QString bookId = ui->bookIdLineEdit->text().trimmed();
    QString issueDate = ui->issueDateLineEdit->text().trimmed();

    if (studentName.isEmpty() || bookName.isEmpty() || bookId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
        return;
    }

    if (issueDate.isEmpty()) {
        issueDate = QDate::currentDate().toString("yyyy-MM-dd");
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query;

    // 1️⃣ Отримуємо student_id
    query.prepare("SELECT id FROM students WHERE name = :name");
    query.bindValue(":name", studentName);
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Not Found", "No student found with this name.");
        return;
    }
    int studentId = query.value(0).toInt();

    // 2️⃣ Перевіряємо, чи існує книга і чи доступна
    query.prepare("SELECT available FROM books WHERE id = :book_id AND title = :book_title");
    query.bindValue(":book_id", bookId);
    query.bindValue(":book_title", bookName);

    if (!query.exec()) {
        QMessageBox::critical(this, "SQL Error", query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Not Found", "No book found with this ID and title.");
        return;
    }

    bool available = query.value(0).toBool();
    if (!available) {
        QMessageBox::warning(this, "Unavailable", "This book is currently not available.");
        return;
    }

    // 3️⃣ Додаємо запис у таблицю transactions
    query.prepare("INSERT INTO transactions (book_id, student_id, issue_date, return_date) "
                  "VALUES (:book_id, :student_id, :issue_date, NULL)");
    query.bindValue(":book_id", bookId);
    query.bindValue(":student_id", studentId);
    query.bindValue(":issue_date", issueDate);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to issue book.\n" + query.lastError().text());
        return;
    }

    // 4️⃣ Оновлюємо статус книги (available = 0)
    query.prepare("UPDATE books SET available = 0 WHERE id = :book_id");
    query.bindValue(":book_id", bookId);
    if (!query.exec()) {
        QMessageBox::warning(this, "Warning", "Book issued, but failed to update availability.\n" + query.lastError().text());
    }

    // 5️⃣ Повідомлення та очищення полів
    QMessageBox::information(this, "Success", "Book issued successfully!");
    clearFields();
}

void IssueBooks::on_cancelButton_clicked()
{
    clearFields();
}

void IssueBooks::on_goBackButton_clicked()
{
    this->close();
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
}

void IssueBooks::clearFields()
{
    ui->studentNameLineEdit->clear();
    ui->studentEmailLineEdit->clear();
    ui->bookNameLineEdit->clear();
    ui->bookIdLineEdit->clear();
    ui->issueDateLineEdit->clear();
}
