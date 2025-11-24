#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QMainWindow>
#include "addbooks.h"
#include "database.h"

namespace Ui {
class AdminMenu;
}

class AdminMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminMenu(QWidget *parent = nullptr);
    ~AdminMenu();
    void loadDashboardStats();

private slots:
    void on_actionAdd_new_book_triggered();
    void on_actionView_books_triggered();
    void on_actionAdd_student_triggered();
    void on_actionView_student_information_triggered();
    void on_actionIssue_books_triggered();
    void on_actionReturn_books_triggered();
    void on_actionEdit_student_information_triggered();
    void on_actionLogout_triggered();

private:
    Ui::AdminMenu *ui;
    Database db;
};

#endif // ADMINMENU_H
