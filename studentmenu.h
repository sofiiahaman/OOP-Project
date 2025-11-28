#ifndef STUDENTMENU_H
#define STUDENTMENU_H

#include <QMainWindow>

namespace Ui {
class StudentMenu;
}

class StudentMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentMenu(QWidget *parent = nullptr);
    ~StudentMenu();

private slots:
    void on_actionLogout_triggered();
    void on_actionView_books_triggered();
    void paintEvent(QPaintEvent *event);

private:
    Ui::StudentMenu *ui;
};

#endif // STUDENTMENU_H
