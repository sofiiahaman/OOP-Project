#include "editstudentinformation.h"
#include "ui_editstudentinformation.h"
#include "adminmenu.h"

#include "./repositories/StudentRepository.h"
#include "database.h"

#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>

EditStudentInformation::EditStudentInformation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditStudentInformation),
    currentStudentId(-1)
{
    ui->setupUi(this);

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    connect(ui->enterNameLineEdit, &QLineEdit::textChanged,
            this, &EditStudentInformation::loadStudents);

    connect(ui->studentsTableView, &QTableView::clicked,
            this, &EditStudentInformation::onStudentSelected);

    connect(ui->saveButton, &QPushButton::clicked,
            this, &EditStudentInformation::saveChanges);
}

EditStudentInformation::~EditStudentInformation()
{
    delete ui;
}

void EditStudentInformation::loadStudents(const QString &name)
{
    if (name.trimmed().length() < 2) {
        ui->studentsTableView->setModel(nullptr);
        return;
    }

    Database database;
    StudentRepository repo(database);

    QSqlQueryModel* model = repo.getAllStudents();
    if (!model)
        return;

    QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setFilterKeyColumn(1); // name column
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterFixedString(name);

    ui->studentsTableView->setModel(proxy);
    ui->studentsTableView->resizeColumnsToContents();
}

void EditStudentInformation::onStudentSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QSortFilterProxyModel *proxy =
        qobject_cast<QSortFilterProxyModel*>(ui->studentsTableView->model());

    if (!proxy) return;

    QModelIndex sourceIndex = proxy->mapToSource(index);

    QSqlQueryModel *sourceModel =
        qobject_cast<QSqlQueryModel*>(proxy->sourceModel());

    if (!sourceModel) return;

    int row = sourceIndex.row();

    currentStudentId = sourceModel->index(row, 0).data().toInt();
    QString studentName = sourceModel->index(row, 1).data().toString();
    QString studentEmail = sourceModel->index(row, 2).data().toString();

    ui->studentNameLineEdit->setText(studentName);
    ui->studentEmailLineEdit->setText(studentEmail);
}

void EditStudentInformation::saveChanges()
{
    if (currentStudentId == -1) {
        QMessageBox::warning(this, "Error", "No student selected!");
        return;
    }

    QString updatedName = ui->studentNameLineEdit->text().trimmed();
    QString updatedEmail = ui->studentEmailLineEdit->text().trimmed();

    if (updatedName.isEmpty() || updatedEmail.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    Database database;
    StudentRepository repo(database);

    bool success = repo.updateStudent(currentStudentId,
                                      updatedName,
                                      updatedEmail);

    if (!success) {
        QMessageBox::critical(this, "Error", "Failed to update student.");
        return;
    }

    QMessageBox::information(this, "Success", "Student information updated!");
    clearFields();
}

void EditStudentInformation::clearFields()
{
    currentStudentId = -1;

    ui->enterNameLineEdit->clear();
    ui->studentNameLineEdit->clear();
    ui->studentEmailLineEdit->clear();
    ui->studentsTableView->setModel(nullptr);
}

void EditStudentInformation::on_goBackButton_clicked()
{
#ifndef UNIT_TEST
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
#endif
    this->close();
}

void EditStudentInformation::on_clearButton_clicked()
{
    clearFields();
}
