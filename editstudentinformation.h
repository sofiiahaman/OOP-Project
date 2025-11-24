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
    // Завантаження студентів у таблицю
    void loadStudents(const QString &name);

    // Вибір студента з таблиці
    void onStudentSelected(const QModelIndex &index);

    // Збереження змін
    void saveChanges();

    // Очищення полів
    void clearFields();

    void on_goBackButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::EditStudentInformation *ui;
    int currentStudentId;
};

#endif // EDITSTUDENTINFORMATION_H
