#ifndef RETURNBOOKS_H
#define RETURNBOOKS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTimer>

namespace Ui {
class ReturnBooks;
}

class ReturnBooks : public QDialog
{
    Q_OBJECT

public:
    explicit ReturnBooks(QWidget *parent = nullptr);
    ~ReturnBooks();

private slots:
    void on_goBackButton_5_clicked();

    // Завантажити транзакції по імені студента
    void loadTransactionsByStudent(const QString &name);

    // Клік по рядку таблиці
    void onTransactionSelected(const QModelIndex &index);

    // Натискання кнопки Return
    void onReturnClicked();

    // Очищення всіх полів
    void clearFields();

private:
    Ui::ReturnBooks *ui;
    QTimer *searchTimer;
};

#endif // RETURNBOOKS_H
