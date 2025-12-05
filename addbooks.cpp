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

AddBooks::AddBooks(bool testMode, QWidget *parent)
    : QDialog(parent), ui(new Ui::AddBooks)
{
    ui->setupUi(this);

    connect(ui->saveButton,   &QPushButton::clicked,
            this, &AddBooks::on_saveButton_clicked);

    connect(ui->cancelButton, &QPushButton::clicked,
            this, &AddBooks::on_cancelButton_clicked);

    connect(ui->goBackButton, &QPushButton::clicked,
            this, &AddBooks::on_goBackButton_clicked);

    if (testMode) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "test_conn");
        db.setDatabaseName(":memory:");
        db.open();

        QSqlQuery q(db);
        q.exec("CREATE TABLE books ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "title TEXT, author TEXT, publisher TEXT, category TEXT, available INTEGER)");
    }
}

AddBooks::~AddBooks()
{
    delete ui;
}

void AddBooks::setBookNameLineEdit(QLineEdit *edit)
{
    if (!edit) return;
    edit->setParent(this);
    ui->bookNameLineEdit->deleteLater();
    ui->bookNameLineEdit = edit;
}

void AddBooks::setBookAuthorLineEdit(QLineEdit *edit)
{
    if (!edit) return;
    edit->setParent(this);
    ui->bookAuthorLineEdit->deleteLater();
    ui->bookAuthorLineEdit = edit;
}

void AddBooks::setBookPublisherLineEdit(QLineEdit *edit)
{
    if (!edit) return;
    edit->setParent(this);
    ui->bookPublisherLineEdit->deleteLater();
    ui->bookPublisherLineEdit = edit;
}

void AddBooks::setBookCategoryLineEdit(QLineEdit *edit)
{
    if (!edit) return;
    edit->setParent(this);
    ui->bookCategoryLineEdit->deleteLater();
    ui->bookCategoryLineEdit = edit;
}

// ----- Getters -----
QLineEdit* AddBooks::getBookNameLineEdit() const      { return ui->bookNameLineEdit; }
QLineEdit* AddBooks::getBookAuthorLineEdit() const    { return ui->bookAuthorLineEdit; }
QLineEdit* AddBooks::getBookPublisherLineEdit() const { return ui->bookPublisherLineEdit; }
QLineEdit* AddBooks::getBookCategoryLineEdit() const  { return ui->bookCategoryLineEdit; }

QPushButton* AddBooks::getSaveButton() const   { return ui->saveButton; }
QPushButton* AddBooks::getCancelButton() const { return ui->cancelButton; }
QPushButton* AddBooks::getGoBackButton() const { return ui->goBackButton; }

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
    #ifndef UNIT_TEST
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    #endif
    this->close();
}

void AddBooks::on_saveButton_clicked()
{
    QString title = ui->bookNameLineEdit->text().trimmed();
    QString author = ui->bookAuthorLineEdit->text().trimmed();
    QString publisher = ui->bookPublisherLineEdit->text().trimmed();
    QString category = ui->bookCategoryLineEdit->text().trimmed();

    if (title.isEmpty() || author.isEmpty() || publisher.isEmpty() || category.isEmpty()) {
        #ifndef UNIT_TEST
        QMessageBox::warning(this, "Error", "Please fill in all fields!");
        #endif
        return;
    }

    Database db;
    if (!db.openConnection()) {
        #ifndef UNIT_TEST
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        #endif
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, publisher, category, available) VALUES (?, ?, ?, ?, 1)");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(publisher);
    query.addBindValue(category);

    if (query.exec()) {
        #ifndef UNIT_TEST
        QMessageBox::information(this, "Success", "Book added successfully!");
        #endif
        ui->bookNameLineEdit->clear();
        ui->bookAuthorLineEdit->clear();
        ui->bookPublisherLineEdit->clear();
        ui->bookCategoryLineEdit->clear();
    } else {
        #ifndef UNIT_TEST
        QMessageBox::critical(this,  "Error", "Failed to add book: " + query.lastError().text());
        #endif
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
