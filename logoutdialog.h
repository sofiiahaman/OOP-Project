#ifndef LOGOUTDIALOG_H
#define LOGOUTDIALOG_H

#include <QDialog>
#include <QPropertyAnimation>

class LogoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogoutDialog(QWidget *parent = nullptr);

private:
    QPropertyAnimation *fadeAnimation;
};

#endif // LOGOUTDIALOG_H
