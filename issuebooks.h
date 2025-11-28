#ifndef ISSUEBOOKS_H
#define ISSUEBOOKS_H

#include <QDialog>

namespace Ui {
class IssueBooks;
}

class IssueBooks : public QDialog
{
    Q_OBJECT

public:
    explicit IssueBooks(QWidget *parent = nullptr);
    ~IssueBooks();

private slots:
    void on_issueBookButton_clicked();
    void on_cancelButton_clicked();
    void on_goBackButton_clicked();

private:
    Ui::IssueBooks *ui;
    void clearFields();
    void loadStudentHints();
    void fillStudentEmail(const QString &name);
    void loadBookHints();
    void fillBookId(const QString &title);

    QMap<QString, QString> studentEmails;
    QMap<QString, int> bookIds;
};

#endif // ISSUEBOOKS_H
