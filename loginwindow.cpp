#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include "mainwindow.h"
#include "studentmenu.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
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

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    if (!db.openConnection()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database!");
    }
}

LoginWindow::~LoginWindow() {
    db.closeConnection();
    delete ui;
}

void LoginWindow::on_loginButton_clicked() {
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    if (db.checkLogin(username, password)) {
        QString role = db.getUserRole(username);

        if (role == "student") {
            // Open StudentMenu window
            StudentMenu *studentMenu = new StudentMenu();
            studentMenu->show();

            this->close();
        } else {
            // For other roles – standard handling
            emit loginSuccessful(role);
            this->close();
        }
    } else {
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
    MainWindow *addMainWindow = new MainWindow();
    addMainWindow->show();

    this->close();
}
