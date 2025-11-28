#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "studentmenu.h"
#include "database.h"
#include "mainwindow.h"
#include <QMessageBox>

SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
{
    ui->setupUi(this);

    QPixmap pix1(":/icons/icons/user.png");
    int w = ui->userIcon->width();
    int h = ui->userIcon->height();
    ui->userIcon->setPixmap(pix1.scaled(w,h, Qt::KeepAspectRatio));

    QPixmap pix4(":/icons/icons/envelope.png");
    int w4 = ui->emailIcon->width();
    int h4 = ui->emailIcon->height();
    ui->emailIcon->setPixmap(pix4.scaled(w4,h4, Qt::KeepAspectRatio));

    QPixmap pix2(":/icons/icons/password.png");
    int w2 = ui->passwordIcon->width();
    int h2 = ui->passwordIcon->height();
    ui->passwordIcon->setPixmap(pix2.scaled(w2,h2, Qt::KeepAspectRatio));

    QPixmap pix3(":/icons/icons/password.png");
    int w3 = ui->passwordIcon_2->width();
    int h3 = ui->passwordIcon_2->height();
    ui->passwordIcon_2->setPixmap(pix3.scaled(w3,h3, Qt::KeepAspectRatio));

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    if (!db.openConnection()) {
        QMessageBox::critical(this, "Error", "Cannot connect to database!");
    }
}

SignupWindow::~SignupWindow() {
    db.closeConnection();
    delete ui;
}

void SignupWindow::on_signupButton_clicked()
{
    QString email = ui->emailEdit->text();
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    QString confirm = ui->confirmPasswordEdit->text();

    if (email.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "Error", "Passwords do not match!");
        return;
    }

    // 1. Check if a student with this email exists
    int studentId = db.findStudentByEmail(email);

    if (studentId == -1) {
        QMessageBox::warning(this, "Error",
                             "This email is not registered. "
                             "Please contact the administrator.");
        return;
    }

    // 2. Check if the user_id is already linked
    if (db.studentHasUser(studentId)) {
        QMessageBox::warning(this, "Error",
                             "This student already has an account!");
        return;
    }

   // 3. Check if the username is taken
    if (db.userExists(username)) {
        QMessageBox::warning(this, "Error", "User already exists!");
        return;
    }

    QString role = "student";

    // 4. Add user (users)
    int newUserId = db.addUserReturnId(username, password, role);

    if (newUserId == -1) {
        QMessageBox::critical(this, "Error", "Failed to create account!");
        return;
    }

    // 5. Write user_id into students
    if (!db.linkStudentWithUser(studentId, newUserId)) {
        QMessageBox::critical(this, "Error", "Failed to link student to account!");
        return;
    }

    QMessageBox::information(this, "Success", "Account created successfully!");
    emit signupSuccessful(role);

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
