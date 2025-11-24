#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "adminmenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginOptionButton_clicked();
    void on_signupOptionButton_clicked();
    void onUserLoggedIn(const QString &role);


private:
    Ui::MainWindow *ui;
    Database db;
};

#endif // MAINWINDOW_H
