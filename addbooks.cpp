#include "addbooks.h"
#include "ui_addbooks.h"
#include "adminmenu.h"

#include "./repositories/BookRepository.h"
#include "services/libraryservice.h"
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
    LibraryService service(database);
    QStringList categories = service.getAllCategories();

    if (categories.isEmpty()) return;

    QCompleter *completer = new QCompleter(categories, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    ui->bookCategoryLineEdit->setCompleter(completer);
}

void AddBooks::on_saveButton_clicked() {
    Database db;
    LibraryService service(db);

    if (service.addNewBook(ui->bookNameLineEdit->text(), ui->bookAuthorLineEdit->text(),
                           ui->bookPublisherLineEdit->text(), ui->bookCategoryLineEdit->text())) {
        QMessageBox::information(this, "Success", "Book added!");
        clearFields();
    } else {
        QMessageBox::warning(this, "Error", "Invalid data!");
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
