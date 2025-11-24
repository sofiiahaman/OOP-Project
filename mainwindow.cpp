#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "adminmenu.h"
#include "signupwindow.h"
#include "studentmenu.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Натискання на кнопку Login
void MainWindow::on_loginOptionButton_clicked() {
    // Створюємо вікно логіну
    LoginWindow *loginDialog = new LoginWindow();

    // З'єднуємо сигнал успішного входу з методом у MainWindow
    connect(loginDialog, &LoginWindow::loginSuccessful,
            this, &MainWindow::onUserLoggedIn);

    // Показуємо LoginWindow
    loginDialog->show();

    // Закриваємо головне вікно (MainWindow)
    this->close();
}


// Обробка сигналу після успішного входу
void MainWindow::onUserLoggedIn(const QString &role) {
    if (role == "admin") {
        AdminMenu *admin = new AdminMenu();
        admin->show();

        // Закриваємо і головне, і вікно логіну
        this->close();  // закриває MainWindow
        if (auto login = this->findChild<LoginWindow *>()) {
            login->close(); // закриває LoginWindow, якщо ще відкрите
        }
    } else {
        QMessageBox::information(this, "Вхід", "Ви увійшли як користувач.");
        this->close(); // якщо треба також закривати головне вікно після входу
    }
}

void MainWindow::on_signupOptionButton_clicked()
{
    SignupWindow *signup = new SignupWindow();

    // коли студент зареєструвався
    connect(signup, &SignupWindow::signupSuccessful, this,
            [this](QString role){
                if (role == "student") {
                    StudentMenu *stud = new StudentMenu();
                    stud->show();
                }
                this->close();
            }
            );

    signup->show();
    this->close();
}
