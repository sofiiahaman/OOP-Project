#ifndef VIEWSTUDENTINFORMATION_H
#define VIEWSTUDENTINFORMATION_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class ViewStudentInformation;
}

class ViewStudentInformation : public QDialog
{
    Q_OBJECT

public:
    explicit ViewStudentInformation(QWidget *parent = nullptr);
    ~ViewStudentInformation();

private slots:
    void on_refreshButton_clicked();
    void on_goBackButton_clicked();

private:
    Ui::ViewStudentInformation *ui;
    QSqlTableModel *model;

    void loadStudentData(const QString &filter = QString());
};

#endif // VIEWSTUDENTINFORMATION_H
