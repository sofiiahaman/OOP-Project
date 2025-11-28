#include "studentmenu.h"
#include "ui_studentmenu.h"
#include "mainwindow.h"
#include "viewbooksstudent.h"
#include "logoutdialog.h"
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>

StudentMenu::StudentMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentMenu)
{
    ui->setupUi(this);
}

StudentMenu::~StudentMenu()
{
    delete ui;
}

void StudentMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Завантажуємо зображення з папки icons
    QPixmap background(":/icons/icons/library.jpg");

    // Масштабуємо зображення під розмір вікна
    painter.drawPixmap(0, 0, this->width(), this->height(), background);

    // Якщо потрібно, можна малювати інші елементи поверх фону
    QMainWindow::paintEvent(event);
}

void StudentMenu::on_actionView_books_triggered()
{
    ViewBooksStudent *vb = new ViewBooksStudent(this);
    vb->setModal(true);
    vb->show();
    this->hide();
}

void StudentMenu::on_actionLogout_triggered()
{
    LogoutDialog dlg(this);

    if (dlg.exec() == QDialog::Accepted)
    {
        MainWindow *mainwin = new MainWindow();
        mainwin->show();
        this->close();
    }
}

