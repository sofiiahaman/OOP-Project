#include "signupwindow.h"
#include "ui_signupwindow.h"

#include "mainwindow.h"
#include "database.h"
#include "./repositories/StudentRepository.h"
#include "./repositories/UserRepository.h"

#include <QMessageBox>
#include <QPixmap>

SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
{
    ui->setupUi(this);

    QPixmap pix1(":/icons/icons/user.png");
    ui->userIcon->setPixmap(
        pix1.scaled(ui->userIcon->width(),
                    ui->userIcon->height(),
                    Qt::KeepAspectRatio));

    QPixmap pix4(":/icons/icons/envelope.png");
    ui->emailIcon->setPixmap(
        pix4.scaled(ui->emailIcon->width(),
                    ui->emailIcon->height(),
                    Qt::KeepAspectRatio));

    QPixmap pix2(":/icons/icons/password.png");
    ui->passwordIcon->setPixmap(
        pix2.scaled(ui->passwordIcon->width(),
                    ui->passwordIcon->height(),
                    Qt::KeepAspectRatio));

    QPixmap pix3(":/icons/icons/password.png");
    ui->passwordIcon_2->setPixmap(
        pix3.scaled(ui->passwordIcon_2->width(),
                    ui->passwordIcon_2->height(),
                    Qt::KeepAspectRatio));

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));
}

SignupWindow::~SignupWindow()
{
    delete ui;
}

void SignupWindow::on_signupButton_clicked()
{
    QString email = ui->emailEdit->text().trimmed();
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();
    QString confirm = ui->confirmPasswordEdit->text().trimmed();

    if (email.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "Error", "Passwords do not match!");
        return;
    }

    Database database;
    StudentRepository studentRepo(database);
    UserRepository userRepo(database);

    // Check student by email
    int studentId = studentRepo.findStudentByEmail(email);

    if (studentId == -1) {
        QMessageBox::warning(this, "Error",
                             "This email is not registered. "
                             "Please contact the administrator.");
        return;
    }

    // Check if already linked
    if (studentRepo.studentHasUser(studentId)) {
        QMessageBox::warning(this, "Error",
                             "This student already has an account!");
        return;
    }

    // Check username uniqueness
    if (userRepo.userExists(username)) {
        QMessageBox::warning(this, "Error", "User already exists!");
        return;
    }

    // Create user
    int newUserId = userRepo.addUserReturnId(username, password, "student");

    if (newUserId == -1) {
        QMessageBox::critical(this, "Error", "Failed to create account!");
        return;
    }

    // Link student with user
    if (!userRepo.linkStudentWithUser(studentId, newUserId)) {
        QMessageBox::critical(this, "Error",
                              "Failed to link student to account!");
        return;
    }

    QMessageBox::information(this, "Success",
                             "Account created successfully!");

    emit signupSuccessful("student");

    this->close();
}


void SignupWindow::on_showPasswordCheckBox_stateChanged(int state)
{
    QLineEdit::EchoMode mode;

    if (state == Qt::Checked)
        mode = QLineEdit::Normal;            // show text
    else
        mode = QLineEdit::Password;          // hide text

    ui->passwordEdit->setEchoMode(mode);
    ui->confirmPasswordEdit->setEchoMode(mode);
}

void SignupWindow::on_goBackButton_clicked()
{
    MainWindow *addMainWindow = new MainWindow();
    addMainWindow->show();

    this->close();
}
