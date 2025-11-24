#ifndef VIEWBOOKS_H
#define VIEWBOOKS_H

#include <QDialog>
#include "adminmenu.h"
#include <QSqlTableModel>
#include "database.h"

namespace Ui {
class ViewBooks;
}

class ViewBooks : public QDialog
{
    Q_OBJECT

public:
    explicit ViewBooks(QWidget *parent = nullptr);
    ~ViewBooks();

private slots:
    void on_goBackButton_clicked();
    void on_refreshButton_clicked();

private:
    Ui::ViewBooks *ui;
    QSqlTableModel *model;   // модель для таблиці
    void loadBooks();
};

#endif // VIEWBOOKS_H
