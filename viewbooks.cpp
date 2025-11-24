#include "viewbooks.h"
#include "ui_viewbooks.h"
#include "adminmenu.h"
#include <QMessageBox>
#include <QHeaderView>

ViewBooks::ViewBooks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewBooks)
    , model(nullptr)
{
    ui->setupUi(this);
    loadBooks(); // одразу показуємо всі книги
}

ViewBooks::~ViewBooks()
{
    delete ui;
}

void ViewBooks::on_goBackButton_clicked()
{
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}

void ViewBooks::loadBooks()
{
    Database db;
    if (!db.openConnection()) {
        QMessageBox::critical(this, "Помилка", "Не вдалося підключитися до бази даних!");
        return;
    }

    // Якщо модель ще не створена — створюємо
    if (!model) {
        model = new QSqlTableModel(this);
        model->setTable("books");
    }

    // Отримуємо текст з поля пошуку
    QString searchText = ui->searchBookLineEdit->text().trimmed();

    // Якщо є введений текст — додаємо фільтр
    if (!searchText.isEmpty()) {
        // LIKE — дозволяє частковий збіг
        model->setFilter(QString("title LIKE '%%1%'").arg(searchText));
    } else {
        model->setFilter(""); // без фільтра — всі книги
    }

    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Назва книги");
    model->setHeaderData(2, Qt::Horizontal, "Автор");
    model->setHeaderData(3, Qt::Horizontal, "Видавництво");
    model->setHeaderData(4, Qt::Horizontal, "Доступна");

    ui->tableView->setModel(model);

    // Автоматичне підлаштування колонок
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    db.closeConnection();
}

void ViewBooks::on_refreshButton_clicked()
{
    loadBooks(); // кнопка "Refresh" оновлює таблицю з урахуванням пошуку
}
