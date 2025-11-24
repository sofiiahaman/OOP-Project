#ifndef ADDSTUDENTS_H
#define ADDSTUDENTS_H

#include <QDialog>
#include "database.h"

namespace Ui {
class AddStudents;
}

class AddStudents : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudents(QWidget *parent = nullptr);
    ~AddStudents();

private slots:
    void on_goBackButton_2_clicked();
    void on_saveButton_2_clicked();
    void on_cancelButton_2_clicked();

private:
    Ui::AddStudents *ui;
};

#endif // ADDSTUDENTS_H
