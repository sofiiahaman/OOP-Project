#include "addbooks.h"
#include "ui_addbooks.h"
#include "adminmenu.h"
#include "database.h"
#include <QMessageBox>
#include <QCompleter>

AddBooks::AddBooks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddBooks)
{
    ui->setupUi(this);

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    loadCategoryHints();
}

AddBooks::~AddBooks()
{
    delete ui;
}

void AddBooks::loadCategoryHints()
{
    Database db;
    if (!db.openConnection()) {
        qDebug() << "Database connection failed";
        return;
    }

    QSqlQuery query("SELECT DISTINCT category FROM books");
    QStringList categories;

    while (query.next()) {
        categories << query.value(0).toString();
    }

    db.closeConnection();

    // Create a completer for category hints
    QCompleter *completer = new QCompleter(categories, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);  // case insensitive
    completer->setFilterMode(Qt::MatchContains);         // show suggestions containing text
    completer->setCompletionMode(QCompleter::PopupCompletion);

    ui->bookCategoryLineEdit->setCompleter(completer);
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
        QMessageBox::warning(this, "Error", "Please fill in all fields!");
        return;
    }

    Database db;
    if (!db.openConnection()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, publisher, category, available) VALUES (?, ?, ?, ?, 1)");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(publisher);
    query.addBindValue(category);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Book added successfully!");
        ui->bookNameLineEdit->clear();
        ui->bookAuthorLineEdit->clear();
        ui->bookPublisherLineEdit->clear();
        ui->bookCategoryLineEdit->clear();
    } else {
        QMessageBox::critical(this,  "Error", "Failed to add book: " + query.lastError().text());
    }

    db.closeConnection();
}

void AddBooks::on_cancelButton_clicked()
{
    // Clear all input fields
    ui->bookNameLineEdit->clear();
    ui->bookAuthorLineEdit->clear();
    ui->bookPublisherLineEdit->clear();
    ui->bookCategoryLineEdit->clear();
}
