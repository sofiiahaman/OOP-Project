#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QDialog>
#include "database.h"

namespace Ui {
class SignupWindow;
}

class SignupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SignupWindow(QWidget *parent = nullptr);
    ~SignupWindow();

signals:
    void signupSuccessful(QString role);  // завжди "student"

private slots:
    void on_signupButton_clicked();
    void on_showPasswordCheckBox_stateChanged(int state);
    // void on_backButton_clicked();
    void on_goBackButton_clicked();

private:
    Ui::SignupWindow *ui;
    Database db;
};

#endif // SIGNUPWINDOW_H
