#include "addbooks.h"
#include "ui_addbooks.h"
#include "adminmenu.h"

#include "./repositories/BookRepository.h"
#include "database.h"

#include <QMessageBox>
#include <QCompleter>
#include <QSqlRecord>

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
    Database database;
    BookRepository repo(database);

    QSqlQueryModel* model = repo.getAllBooks();
    if (!model)
        return;

    QStringList categories;

    for (int i = 0; i < model->rowCount(); ++i) {
        QString category = model->record(i).value("category").toString();
        if (!categories.contains(category))
            categories << category;
    }

    delete model;

    QCompleter *completer = new QCompleter(categories, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    ui->bookCategoryLineEdit->setCompleter(completer);
}

void AddBooks::on_saveButton_clicked()
{
    // UI validation
    QString title = ui->bookNameLineEdit->text().trimmed();
    QString author = ui->bookAuthorLineEdit->text().trimmed();
    QString publisher = ui->bookPublisherLineEdit->text().trimmed();
    QString category = ui->bookCategoryLineEdit->text().trimmed();

    if (title.isEmpty() || author.isEmpty() ||
        publisher.isEmpty() || category.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please fill in all fields!");
        return;
    }

    // Repository call
    Database database;
    BookRepository repo(database);

    bool success = repo.addBook(title, author, publisher, category);

    if (success) {
        QMessageBox::information(this, "Success",
                                 "The book was successfully added!");
        clearFields();
        loadCategoryHints(); // refresh hints
    } else {
        QMessageBox::critical(this, "Error",
                              "Failed to add the book to the database.");
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
