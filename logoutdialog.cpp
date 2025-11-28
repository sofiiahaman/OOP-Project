#include "logoutdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsOpacityEffect>

LogoutDialog::LogoutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Logout");
    setFixedSize(320, 150);
    setModal(true);

    // ----- UI -----
    QLabel *label = new QLabel("Are you sure you want to logout?");
    label->setAlignment(Qt::AlignCenter);

    QPushButton *btnYes = new QPushButton("Logout");
    QPushButton *btnNo = new QPushButton("Cancel");

    btnYes->setStyleSheet("background-color: #d9534f; color: white; padding: 6px;");
    btnNo->setStyleSheet("padding: 6px;");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnYes);
    btnLayout->addWidget(btnNo);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(label);
    mainLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    // ----- Fade-in Animation -----
    auto *effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);

    fadeAnimation = new QPropertyAnimation(effect, "opacity");
    fadeAnimation->setDuration(300);       // 300 ms
    fadeAnimation->setStartValue(0.0);     // повністю прозоре
    fadeAnimation->setEndValue(1.0);       // нормальна непрозорість
    fadeAnimation->start();

    // ----- Buttons -----
    connect(btnYes, &QPushButton::clicked, this, [this]() {
        accept();   // повертає QDialog::Accepted
    });

    connect(btnNo, &QPushButton::clicked, this, [this]() {
        reject();   // повертає QDialog::Rejected
    });
}
