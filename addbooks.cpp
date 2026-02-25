#include "addbooks.h"
#include "ui_addbooks.h"
#include "adminmenu.h"
#include <QMessageBox>
#include <QCompleter>
#include <QSqlQuery>

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
    if (!db.openConnection()) return;

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

void AddBooks::on_saveButton_clicked()
{
    // 1. Data validation (UI responsibility)
    QString title = ui->bookNameLineEdit->text().trimmed();
    QString author = ui->bookAuthorLineEdit->text().trimmed();
    QString publisher = ui->bookPublisherLineEdit->text().trimmed();
    QString category = ui->bookCategoryLineEdit->text().trimmed();

    if (title.isEmpty() || author.isEmpty() || publisher.isEmpty() || category.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields!");
        return;
    }

    // 2. Call business logic through the Repository (Stage 1)
    Database db;
    if (db.addBook(title, author, publisher, category)) {
        QMessageBox::information(this, "Success", "The book was successfully added!");
        clearFields();
        loadCategoryHints(); // Refresh the hints list
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the book to the database.");
    }
}

void AddBooks::on_goBackButton_clicked()
{
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}


void AddBooks::on_cancelButton_clicked()
{
    clearFields();
}

void AddBooks::clearFields()
{
    ui->bookNameLineEdit->clear();
    ui->bookAuthorLineEdit->clear();
    ui->bookPublisherLineEdit->clear();
    ui->bookCategoryLineEdit->clear();
}
