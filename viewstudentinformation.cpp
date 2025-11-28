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

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    // Database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Cannot open database connection!");
        return;
    }

    // Set table model
    model->setTable("students");
    model->select();

    ui->tableView->setModel(model);

    // Auto-resize columns
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ViewStudentInformation::~ViewStudentInformation()
{
    delete ui;
}

// Load student data (with or without filter)
void ViewStudentInformation::loadStudentData(const QString &filter)
{
    if (filter.isEmpty()) {
        model->setFilter("");
    } else {
        model->setFilter(QString("name LIKE '%%1%'").arg(filter));
    }
    model->select();
}

// "Refresh" button — applies filter by student name
void ViewStudentInformation::on_refreshButton_clicked()
{
    QString nameFilter = ui->studentNameLineEdit->text().trimmed();
    loadStudentData(nameFilter);
}

// "Back" button — return to admin menu
void ViewStudentInformation::on_goBackButton_clicked()
{
    this->close();
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
}
