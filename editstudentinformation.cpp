#include "editstudentinformation.h"
#include "ui_editstudentinformation.h"
#include "database.h"
#include "adminmenu.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>

EditStudentInformation::EditStudentInformation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditStudentInformation)
{
    ui->setupUi(this);

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    // Load students into the table when typing a name
    connect(ui->enterNameLineEdit, &QLineEdit::textChanged, this, &EditStudentInformation::loadStudents);

    // Connect table row selection
    connect(ui->studentsTableView, &QTableView::clicked, this, &EditStudentInformation::onStudentSelected);

    // Connect save changes button
    connect(ui->saveButton, &QPushButton::clicked, this, &EditStudentInformation::saveChanges);
}

EditStudentInformation::~EditStudentInformation()
{
    delete ui;
}

/* ========================
 * Loading students
 * ======================== */
void EditStudentInformation::loadStudents(const QString &name)
{
    // Check minimum 2 characters for search
    if (name.trimmed().length() < 2) {
        ui->studentsTableView->setModel(nullptr);
        return;
    }

    // Connect to database
    Database db;
    if (!db.openConnection()) {
        qDebug() << "Не вдалося відкрити базу!";
        return;
    }

    // Create SQL query to retrieve students by name
    QSqlQuery query;
    query.prepare("SELECT id, name, email FROM students WHERE name LIKE :name");
    query.bindValue(":name", "%" + name + "%");

    // Execute query
    if (!query.exec()) {
        qDebug() << "Помилка запиту:" << query.lastError().text();
        return;
    }

    // Load results into the table model
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);
    ui->studentsTableView->setModel(model);
    ui->studentsTableView->resizeColumnsToContents();
}

/* ========================
 * Selecting a student from table
 * ======================== */
void EditStudentInformation::onStudentSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();
    QAbstractItemModel *model = ui->studentsTableView->model();

    int studentId = model->index(row, 0).data().toInt();           // Student ID
    QString studentName = model->index(row, 1).data().toString();  // Student name
    QString studentEmail = model->index(row, 2).data().toString(); // Student email

    // Fill input fields
    ui->studentNameLineEdit->setText(studentName);
    ui->studentEmailLineEdit->setText(studentEmail);

    // Store student ID for update
    currentStudentId = studentId;
}

/* ========================
 * Saving changes
 * ======================== */
void EditStudentInformation::saveChanges()
{
    QString updatedName = ui->studentNameLineEdit->text();
    QString updatedEmail = ui->studentEmailLineEdit->text();

    if (updatedName.isEmpty() || updatedEmail.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    // Connect to database
    Database db;
    if (!db.openConnection()) {
        qDebug() << "Failed to open database!";
        return;
    }

    // Update student information in the database
    QSqlQuery query;
    query.prepare("UPDATE students SET name = :name, email = :email WHERE id = :id");
    query.bindValue(":name", updatedName);
    query.bindValue(":email", updatedEmail);
    query.bindValue(":id", currentStudentId);

    if (!query.exec()) {
        qDebug() << "Error updating student data:" << query.lastError().text();
        return;
    }

    QMessageBox::information(this, "Success", "Student information updated!");

    // Clear fields
    clearFields();
}

/* ========================
 * Clearing fields
 * ======================== */
void EditStudentInformation::clearFields()
{
    ui->enterNameLineEdit->clear();
    ui->studentNameLineEdit->clear();
    ui->studentEmailLineEdit->clear();
    ui->studentsTableView->setModel(nullptr);  // Clear table
}

void EditStudentInformation::on_goBackButton_clicked()
{
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}

void EditStudentInformation::on_clearButton_clicked()
{
    clearFields();
}
