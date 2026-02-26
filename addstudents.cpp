#include "addstudents.h"
#include "ui_addstudents.h"
#include "adminmenu.h"

#include "./repositories/StudentRepository.h"
#include "database.h"

#include <QMessageBox>

AddStudents::AddStudents(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddStudents)
{
    ui->setupUi(this);
    ui->goBackButton_2->setIcon(QIcon(":/icons/icons/left-arrow.png"));
}

AddStudents::~AddStudents()
{
    delete ui;
}

void AddStudents::on_goBackButton_2_clicked()
{
#ifndef UNIT_TEST
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
#endif
    this->close();
}

void AddStudents::on_saveButton_2_clicked()
{
    QString name = ui->studentNameLineEdit_2->text().trimmed();
    QString email = ui->studentEmailLineEdit_2->text().trimmed();

    if (name.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields!");
        return;
    }

    Database database;
    StudentRepository repo(database);

    bool success = repo.addStudent(name, email);

    if (success) {
        QMessageBox::information(this, "Success", "Student added successfully!");
        ui->studentNameLineEdit_2->clear();
        ui->studentEmailLineEdit_2->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add student.");
    }
}

void AddStudents::on_cancelButton_2_clicked()
{
    ui->studentNameLineEdit_2->clear();
    ui->studentEmailLineEdit_2->clear();
}
