#include "addstudents.h"
#include "ui_addstudents.h"
#include "adminmenu.h"
#include <QMessageBox>

AddStudents::AddStudents(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddStudents)
{
    ui->setupUi(this);
}

AddStudents::~AddStudents()
{
    delete ui;
}

void AddStudents::on_goBackButton_2_clicked()
{
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}

void AddStudents::on_saveButton_2_clicked()
{
    QString name = ui->studentNameLineEdit_2->text().trimmed();
    QString email = ui->studentEmailLineEdit_2->text().trimmed();

    if (name.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, заповніть усі поля!");
        return;
    }

    Database db;
    if (!db.openConnection()) {
        QMessageBox::critical(this, "Помилка", "Не вдалося підключитися до бази даних.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO students (name, email) VALUES (?, ?)");
    query.addBindValue(name);
    query.addBindValue(email);

    if (query.exec()) {
        QMessageBox::information(this, "Успіх", "Студента успішно додано!");
        ui->studentNameLineEdit_2->clear();
        ui->studentEmailLineEdit_2->clear();
    } else {
        QMessageBox::critical(this, "Помилка", "Не вдалося додати студента: " + query.lastError().text());
    }

    db.closeConnection();
}

void AddStudents::on_cancelButton_2_clicked()
{
    ui->studentNameLineEdit_2->clear();
    ui->studentEmailLineEdit_2->clear();
}
