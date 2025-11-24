#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "studentmenu.h"
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
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    QString confirm = ui->confirmPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "Error", "Passwords do not match!");
        return;
    }

    // Тільки студент може реєструватися
    QString role = "student";

    if (db.userExists(username)) {
        QMessageBox::warning(this, "Error", "User already exists!");
        return;
    }

    if (db.addUser(username, password, role)) {
        emit signupSuccessful(role);  // = "student"
        this->close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create account!");
    }
}

void SignupWindow::on_showPasswordCheckBox_stateChanged(int state)
{
    QLineEdit::EchoMode mode;

    if (state == Qt::Checked)
        mode = QLineEdit::Normal;            // показує текст
    else
        mode = QLineEdit::Password;          // приховує текст

    ui->passwordEdit->setEchoMode(mode);
    ui->confirmPasswordEdit->setEchoMode(mode);
}

void SignupWindow::on_goBackButton_clicked()
{
    MainWindow *addMainWindow = new MainWindow();
    addMainWindow->show();

    this->close();
}
