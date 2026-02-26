#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QMessageBox>
#include <QPixmap>

#include "mainwindow.h"
#include "studentmenu.h"

#include "database.h"
#include "./repositories/UserRepository.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    QPixmap pix1(":/icons/icons/user.png");
    ui->userIcon->setPixmap(
        pix1.scaled(ui->userIcon->width(),
                    ui->userIcon->height(),
                    Qt::KeepAspectRatio));

    QPixmap pix2(":/icons/icons/password.png");
    ui->passwordIcon->setPixmap(
        pix2.scaled(ui->passwordIcon->width(),
                    ui->passwordIcon->height(),
                    Qt::KeepAspectRatio));

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter username and password!");
        return;
    }

    Database database;
    UserRepository repo(database);

    if (repo.checkLogin(username, password)) {

        QString role = repo.getUserRole(username);

        if (role == "student") {

#ifndef UNIT_TEST
            StudentMenu *studentMenu = new StudentMenu();
            studentMenu->show();
#endif

            this->close();
        }
        else {
            emit loginSuccessful(role);
            this->close();
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Incorrect username or password!");
    }
}


void LoginWindow::on_showPasswordCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked)
        ui->passwordEdit->setEchoMode(QLineEdit::Normal);
    else
        ui->passwordEdit->setEchoMode(QLineEdit::Password);
}

void LoginWindow::on_goBackButton_clicked()
{
#ifndef UNIT_TEST
    MainWindow *addMainWindow = new MainWindow();
    addMainWindow->show();
#endif

    this->close();
}

