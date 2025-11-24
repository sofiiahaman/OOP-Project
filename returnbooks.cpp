#include "returnbooks.h"
#include "ui_returnbooks.h"
#include "adminmenu.h"
#include "database.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QCompleter>
#include <QStringListModel>

ReturnBooks::ReturnBooks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReturnBooks)
{
    ui->setupUi(this);

    Database db;
    if (!db.openConnection()) {
        qDebug() << "Не вдалося відкрити базу!";
        return;
    }

    // Додаємо випадаючий список для studentNameLineEdit
    QSqlQuery query;
    query.prepare("SELECT name FROM students");

    if (query.exec()) {
        QStringList studentNames;
        while (query.next()) {
            studentNames << query.value(0).toString();  // Додаємо ім'я студента до списку
        }

        // Перевіряємо, чи є студенти в списку
        if (studentNames.isEmpty()) {
            qDebug() << "Список студентів порожній!";
        } else {
            // Створюємо модель для автодоповнення
            QStringListModel *model = new QStringListModel(this);
            model->setStringList(studentNames);

            // Підключаємо модель до completer
            QCompleter *completer = new QCompleter(model, this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);  // Чутливість до регістру
            completer->setFilterMode(Qt::MatchContains); // Використовуємо частковий збіг
            ui->studentNameLineEdit->setCompleter(completer);
        }
    } else {
        qDebug() << "Помилка виконання запиту на вибір студентів: " << query.lastError();
    }

    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);

    connect(ui->studentNameLineEdit, &QLineEdit::textEdited, this, [=](const QString &text){
        searchTimer->start(300); // 300 мс затримка
    });

    connect(searchTimer, &QTimer::timeout, this, [=](){
        loadTransactionsByStudent(ui->studentNameLineEdit->text());
    });

    connect(ui->tableView, &QTableView::clicked,
            this, &ReturnBooks::onTransactionSelected);

    connect(ui->returnButton, &QPushButton::clicked,
            this, &ReturnBooks::onReturnClicked);

    connect(ui->clearButton, &QPushButton::clicked, this, &ReturnBooks::clearFields);

}

ReturnBooks::~ReturnBooks()
{
    delete ui;
}

/* ========================
 * Завантаження транзакцій
 * ======================== */
void ReturnBooks::loadTransactionsByStudent(const QString &name)
{
    QString trimmed = name.trimmed();

    if(trimmed.length() < 2)
    {
        ui->tableView->setModel(nullptr);
        return;
    }

    // Перевірка, чи база відкрита
    if (!QSqlDatabase::database().isOpen()) {
        if (!QSqlDatabase::database().open()) {
            qDebug() << "База не відкрита!";
            return;
        }
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM students WHERE LOWER(name) LIKE LOWER(:name)");
    query.bindValue(":name", "%" + trimmed + "%");

    if(!query.exec() || !query.next())
    {
        ui->tableView->setModel(nullptr);
        return;
    }

    int studentId = query.value(0).toInt();

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(QString(
                        "SELECT id AS 'Issue ID', book_id AS 'Book ID', issue_date AS 'Issue Date', return_date AS 'Return Date' "
                        "FROM transactions WHERE student_id = %1 AND return_date IS NULL")
                        .arg(studentId));

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}



/* ========================
 * Вибір транзакції
 * ======================== */
void ReturnBooks::onTransactionSelected(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    int row = index.row();
    QAbstractItemModel *model = ui->tableView->model();

    int transactionId = model->index(row, 0).data().toInt();
    int bookId = model->index(row, 1).data().toInt();

    QSqlQuery query;
    query.prepare(R"(
        SELECT t.issue_date,
               s.name,
               s.email,
               b.title,
               b.author
        FROM transactions t
        JOIN students s ON t.student_id = s.id
        JOIN books b ON t.book_id = b.id
        WHERE t.id = :tid
    )");

    query.bindValue(":tid", transactionId);

    if(query.exec() && query.next())
    {
        ui->issueIDLineEdit->setText(QString::number(transactionId));
        ui->bookIDLineEdit->setText(QString::number(bookId));
        ui->studentNameLineEdit->setText(query.value("name").toString());
        ui->studentEmailLineEdit->setText(query.value("email").toString());
        ui->bookTitleLineEdit->setText(query.value("title").toString());
        ui->authorLineEdit->setText(query.value("author").toString());
        ui->issueDateLineEdit->setText(query.value("issue_date").toString());
    }
}

/* ========================
 * Повернення книги
 * ======================== */
void ReturnBooks::onReturnClicked()
{
    if(ui->issueIDLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Помилка", "Оберіть транзакцію для повернення.");
        return;
    }

    int transactionId = ui->issueIDLineEdit->text().toInt();
    int bookId = ui->bookIDLineEdit->text().toInt();

    QSqlDatabase::database().transaction();

    QSqlQuery query;

    // Оновлюємо return_date
    query.prepare("UPDATE transactions SET return_date = :rd WHERE id = :id");
    query.bindValue(":rd", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":id", transactionId);

    if(!query.exec())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Помилка", "Не вдалося оновити транзакцію.");
        return;
    }

    // Робимо книгу доступною
    query.prepare("UPDATE books SET available = 1 WHERE id = :bid");
    query.bindValue(":bid", bookId);

    if(!query.exec())
    {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Помилка", "Не вдалося оновити книгу.");
        return;
    }

    QSqlDatabase::database().commit();

    QMessageBox::information(this, "Успішно", "Книга успішно повернена!");

    clearFields();
    loadTransactionsByStudent(ui->studentNameLineEdit->text());
}

/* ========================
 * Очищення полів
 * ======================== */
void ReturnBooks::clearFields()
{
    ui->issueIDLineEdit->clear();
    ui->bookIDLineEdit->clear();
    ui->studentNameLineEdit->clear();
    ui->studentEmailLineEdit->clear();
    ui->bookTitleLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->issueDateLineEdit->clear();

    ui->tableView->setModel(nullptr);
}

void ReturnBooks::on_goBackButton_5_clicked()
{
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}
