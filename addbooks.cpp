#include "addbooks.h"
#include "ui_addbooks.h"
#include "adminmenu.h"
#include "database.h"
#include <QMessageBox>

AddBooks::AddBooks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddBooks)
{
    ui->setupUi(this);

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));
}

AddBooks::~AddBooks()
{
    delete ui;
}

void AddBooks::on_goBackButton_clicked()
{
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}

void AddBooks::on_saveButton_clicked()
{
    QString title = ui->bookNameLineEdit->text().trimmed();
    QString author = ui->bookAuthorLineEdit->text().trimmed();
    QString publisher = ui->bookPublisherLineEdit->text().trimmed();
    QString category = ui->bookCategoryLineEdit->text().trimmed();

    if (title.isEmpty() || author.isEmpty() || publisher.isEmpty() || category.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, заповніть усі поля!");
        return;
    }

    Database db;
    if (!db.openConnection()) {
        QMessageBox::critical(this, "Помилка", "Не вдалося підключитися до бази даних.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, publisher, category, available) VALUES (?, ?, ?, ?, 1)");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(publisher);
    query.addBindValue(category);

    if (query.exec()) {
        QMessageBox::information(this, "Успіх", "Книгу успішно додано!");
        ui->bookNameLineEdit->clear();
        ui->bookAuthorLineEdit->clear();
        ui->bookPublisherLineEdit->clear();
        ui->bookCategoryLineEdit->clear();
    } else {
        QMessageBox::critical(this, "Помилка", "Не вдалося додати книгу: " + query.lastError().text());
    }

    db.closeConnection();
}

void AddBooks::on_cancelButton_clicked()
{
    // Очищення всіх полів вводу
    ui->bookNameLineEdit->clear();
    ui->bookAuthorLineEdit->clear();
    ui->bookPublisherLineEdit->clear();
    ui->bookCategoryLineEdit->clear();
}
