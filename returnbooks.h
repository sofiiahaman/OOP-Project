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

    // Load transactions by student name
    void loadTransactionsByStudent(const QString &name);

    // Click on table row
    void onTransactionSelected(const QModelIndex &index);

    // "Return" button click
    void onReturnClicked();

    // Clear all fields
    void clearFields();

private:
    Ui::ReturnBooks *ui;
    QTimer *searchTimer;
};

#endif // RETURNBOOKS_H
