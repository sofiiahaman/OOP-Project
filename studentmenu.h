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

private:
    Ui::StudentMenu *ui;
};

#endif // STUDENTMENU_H
