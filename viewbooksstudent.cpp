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
    loadBooks(); // одразу показуємо всі книги

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
        QMessageBox::critical(this, "Помилка", "Не вдалося підключитися до бази даних!");
        return;
    }

    if (!model) {
        model = new QSqlTableModel(this);
        model->setTable("books");
    }

    QString filter = "";

    // Отримуємо значення з полів пошуку
    QString title = ui->searchBookLineEdit->text().trimmed();
    QString author = ui->authorLineEdit->text().trimmed();
    QString publisher = ui->publisherLineEdit->text().trimmed();
    QString id = ui->idLineEdit->text().trimmed();
    QString genre = ui->categoryComboBox->currentText().trimmed();
    bool available = ui->availableCheckBox->isChecked();

    // Формуємо фільтр
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
        filter += "available = 1";  // або "available = 0" якщо хочете показувати відсутні книги
    }

    // Якщо є фільтр, застосовуємо його
    if (!filter.isEmpty()) {
        model->setFilter(filter);
    } else {
        model->setFilter(""); // без фільтра — всі книги
    }

    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Назва книги");
    model->setHeaderData(2, Qt::Horizontal, "Автор");
    model->setHeaderData(3, Qt::Horizontal, "Видавництво");
    model->setHeaderData(4, Qt::Horizontal, "Жанр");
    model->setHeaderData(5, Qt::Horizontal, "Доступна");

    ui->tableView->setModel(model);

    // Автоматичне підлаштування колонок
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Налаштування скролбарів
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
    // Очищаємо всі поля
    ui->searchBookLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->publisherLineEdit->clear();
    ui->idLineEdit->clear();

    // Повертаємо category на вибір за замовчуванням
    ui->categoryComboBox->setCurrentIndex(0);

    // Забираємо чекбокс доступності
    ui->availableCheckBox->setChecked(false);

    // Оновлюємо таблицю без фільтрів
    loadBooks();
}


