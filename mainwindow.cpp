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

// Clicking the Login button
void MainWindow::on_loginOptionButton_clicked() {
    // Create the login window
    LoginWindow *loginDialog = new LoginWindow();

    // Connect the successful login signal with a method in MainWindow
    connect(loginDialog, &LoginWindow::loginSuccessful,
            this, &MainWindow::onUserLoggedIn);

    // Show LoginWindow
    loginDialog->show();

    // Close the main window (MainWindow)
    this->close();
}


// Handling the signal after successful login
void MainWindow::onUserLoggedIn(const QString &role) {
    if (role == "admin") {
        AdminMenu *admin = new AdminMenu();
        admin->show();

        // Close both the main window and the login window
        this->close();  // closes MainWindow
        if (auto login = this->findChild<LoginWindow *>()) {
            login->close(); // closes LoginWindow if still open
        }
    } else {
        QMessageBox::information(this, "Login", "You have logged in as a user.");
        this->close(); // close main window after login if required
    }
}

void MainWindow::on_signupOptionButton_clicked()
{
    SignupWindow *signup = new SignupWindow();

   // When a student registers
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
