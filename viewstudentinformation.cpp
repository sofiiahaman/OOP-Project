#include "viewstudentinformation.h"
#include "ui_viewstudentinformation.h"
#include "adminmenu.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>
#include <QMessageBox>

ViewStudentInformation::ViewStudentInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewStudentInformation),
    model(new QSqlTableModel(this))
{
    ui->setupUi(this);

    // Підключення до бази даних
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Cannot open database connection!");
        return;
    }

    // Встановлюємо модель для таблиці
    model->setTable("students");
    model->select();

    ui->tableView->setModel(model);

    // Автоматичне розтягування колонок
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ViewStudentInformation::~ViewStudentInformation()
{
    delete ui;
}

// Завантаження даних студентів (із фільтром або без)
void ViewStudentInformation::loadStudentData(const QString &filter)
{
    if (filter.isEmpty()) {
        model->setFilter("");
    } else {
        model->setFilter(QString("name LIKE '%%1%'").arg(filter));
    }
    model->select();
}

// Кнопка "Refresh" — застосовує фільтр за іменем
void ViewStudentInformation::on_refreshButton_clicked()
{
    QString nameFilter = ui->studentNameLineEdit->text().trimmed();
    loadStudentData(nameFilter);
}

// Кнопка "← back" — повернення до меню адміністратора
void ViewStudentInformation::on_goBackButton_clicked()
{
    this->close();
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
}
