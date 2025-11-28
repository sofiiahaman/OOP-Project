#ifndef VIEWBOOKSSTUDENT_H
#define VIEWBOOKSSTUDENT_H

#include <QDialog>
#include "studentmenu.h"
#include <QSqlTableModel>
#include "database.h"

namespace Ui {
class ViewBooksStudent;
}

class ViewBooksStudent : public QDialog
{
    Q_OBJECT

public:
    explicit ViewBooksStudent(QWidget *parent = nullptr);
    ~ViewBooksStudent();

private slots:
    void on_goBackButton_clicked();
    void on_searchButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::ViewBooksStudent *ui;
    QSqlTableModel *model;   // модель для таблиці
    void loadBooks();
};

#endif // VIEWBOOKSSTUDENT_H
