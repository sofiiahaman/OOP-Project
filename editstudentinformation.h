#ifndef EDITSTUDENTINFORMATION_H
#define EDITSTUDENTINFORMATION_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class EditStudentInformation;
}

class EditStudentInformation : public QDialog
{
    Q_OBJECT

public:
    explicit EditStudentInformation(QWidget *parent = nullptr);
    ~EditStudentInformation();

private slots:
    // Load students into the table
    void loadStudents(const QString &name);

    // Select a student from the table
    void onStudentSelected(const QModelIndex &index);

    // Save updates
    void saveChanges();

    // Clear input fields
    void clearFields();

    void on_goBackButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::EditStudentInformation *ui;
    int currentStudentId;
};

#endif // EDITSTUDENTINFORMATION_H
