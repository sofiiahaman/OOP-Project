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

    // Завантажуємо студентів у таблицю при введенні імені
    connect(ui->enterNameLineEdit, &QLineEdit::textChanged, this, &EditStudentInformation::loadStudents);

    // Підключаємо сигнал для вибору рядка таблиці
    connect(ui->studentsTableView, &QTableView::clicked, this, &EditStudentInformation::onStudentSelected);

    // Підключаємо кнопку збереження змін
    connect(ui->saveButton, &QPushButton::clicked, this, &EditStudentInformation::saveChanges);
}

EditStudentInformation::~EditStudentInformation()
{
    delete ui;
}

/* ========================
 * Завантаження студентів
 * ======================== */
void EditStudentInformation::loadStudents(const QString &name)
{
    // Перевірка на мінімум 2 символи для пошуку
    if (name.trimmed().length() < 2) {
        ui->studentsTableView->setModel(nullptr);
        return;
    }

    // Підключаємо базу даних
    Database db;
    if (!db.openConnection()) {
        qDebug() << "Не вдалося відкрити базу!";
        return;
    }

    // Створюємо SQL запит для отримання студентів за іменем
    QSqlQuery query;
    query.prepare("SELECT id, name, email FROM students WHERE name LIKE :name");
    query.bindValue(":name", "%" + name + "%");

    // Виконуємо запит
    if (!query.exec()) {
        qDebug() << "Помилка запиту:" << query.lastError().text();
        return;
    }

    // Завантажуємо результат у модель для таблиці
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);
    ui->studentsTableView->setModel(model);
    ui->studentsTableView->resizeColumnsToContents();
}

/* ========================
 * Вибір студента з таблиці
 * ======================== */
void EditStudentInformation::onStudentSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();
    QAbstractItemModel *model = ui->studentsTableView->model();

    int studentId = model->index(row, 0).data().toInt();  // ID студента
    QString studentName = model->index(row, 1).data().toString();  // Ім'я студента
    QString studentEmail = model->index(row, 2).data().toString();  // Електронна пошта студента

    // Заповнюємо відповідні поля
    ui->studentNameLineEdit->setText(studentName);
    ui->studentEmailLineEdit->setText(studentEmail);

    // Зберігаємо ID студента для подальшого оновлення
    currentStudentId = studentId;
}

/* ========================
 * Збереження змін
 * ======================== */
void EditStudentInformation::saveChanges()
{
    QString updatedName = ui->studentNameLineEdit->text();
    QString updatedEmail = ui->studentEmailLineEdit->text();

    if (updatedName.isEmpty() || updatedEmail.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Всі поля повинні бути заповнені!");
        return;
    }

    // Підключаємо базу даних
    Database db;
    if (!db.openConnection()) {
        qDebug() << "Не вдалося відкрити базу!";
        return;
    }

    // Оновлюємо дані студента в базі даних
    QSqlQuery query;
    query.prepare("UPDATE students SET name = :name, email = :email WHERE id = :id");
    query.bindValue(":name", updatedName);
    query.bindValue(":email", updatedEmail);
    query.bindValue(":id", currentStudentId);

    if (!query.exec()) {
        qDebug() << "Помилка при оновленні даних студента:" << query.lastError().text();
        return;
    }

    QMessageBox::information(this, "Успішно", "Дані студента оновлено!");

    // Очищаємо поля
    clearFields();
}

/* ========================
 * Очищення полів
 * ======================== */
void EditStudentInformation::clearFields()
{
    ui->enterNameLineEdit->clear();
    ui->studentNameLineEdit->clear();
    ui->studentEmailLineEdit->clear();
    ui->studentsTableView->setModel(nullptr);  // Очищаємо таблицю
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
