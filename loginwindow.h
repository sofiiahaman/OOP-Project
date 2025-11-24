#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "database.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_loginButton_clicked();
    void on_showPasswordCheckBox_stateChanged(int state);
    void on_goBackButton_clicked();

signals:
    void loginSuccessful(const QString &role);

private:
    Ui::LoginWindow *ui;
    Database db;
};

#endif // LOGINWINDOW_H
