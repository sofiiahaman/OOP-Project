#include "viewbooksstudent.h"
#include "ui_viewbooksstudent.h"
#include "studentmenu.h"
#include <QMessageBox>
#include <QHeaderView>

ViewBooksStudent::ViewBooksStudent(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewBooksStudent)
    , model(nullptr)
{
    ui->setupUi(this);
    loadBooks(); // immediately show all books

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));
}

ViewBooksStudent::~ViewBooksStudent()
{
    delete ui;
}

void ViewBooksStudent::on_goBackButton_clicked()
{
    StudentMenu *studentMenu = new StudentMenu();
    studentMenu->show();
    this->close();
}

void ViewBooksStudent::loadBooks()
{
    Database db;
    if (!db.openConnection()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database!");
        return;
    }

    if (!model) {
        model = new QSqlTableModel(this);
        model->setTable("books");
    }

    QString filter = "";

    // Get values from search fields
    QString title = ui->searchBookLineEdit->text().trimmed();
    QString author = ui->authorLineEdit->text().trimmed();
    QString publisher = ui->publisherLineEdit->text().trimmed();
    QString id = ui->idLineEdit->text().trimmed();
    QString genre = ui->categoryComboBox->currentText().trimmed();
    bool available = ui->availableCheckBox->isChecked();

    // Build filter
    if (!title.isEmpty()) {
        filter += QString("title LIKE '%%1%'").arg(title);
    }
    if (!author.isEmpty()) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("author LIKE '%%1%'").arg(author);
    }
    if (!publisher.isEmpty()) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("publisher LIKE '%%1%'").arg(publisher);
    }
    if (!id.isEmpty()) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("id = '%1'").arg(id);
    }
    if (!genre.isEmpty() && genre != "Choose category") {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("category = '%1'").arg(genre);
    }
    if (available) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += "available = 1";
    }

    // Apply filter if exists
    if (!filter.isEmpty()) {
        model->setFilter(filter);
    } else {
        model->setFilter(""); // show all books
    }

    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Назва книги");
    model->setHeaderData(2, Qt::Horizontal, "Автор");
    model->setHeaderData(3, Qt::Horizontal, "Видавництво");
    model->setHeaderData(4, Qt::Horizontal, "Жанр");
    model->setHeaderData(5, Qt::Horizontal, "Доступна");

    ui->tableView->setModel(model);

    // Auto-adjust columns
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Scrollbars
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    db.closeConnection();
}


void ViewBooksStudent::on_searchButton_clicked()
{
    loadBooks();
}

void ViewBooksStudent::on_clearButton_clicked()
{
    // Clear all fields
    ui->searchBookLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->publisherLineEdit->clear();
    ui->idLineEdit->clear();

    // Reset category to default
    ui->categoryComboBox->setCurrentIndex(0);

    // Uncheck availability checkbox
    ui->availableCheckBox->setChecked(false);

    // Refresh table without filters
    loadBooks();
}


