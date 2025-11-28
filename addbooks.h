#ifndef ADDBOOKS_H
#define ADDBOOKS_H

#include <QDialog>

namespace Ui {
class AddBooks;
}

class AddBooks : public QDialog
{
    Q_OBJECT

public:
    explicit AddBooks(QWidget *parent = nullptr);
    ~AddBooks();

private slots:
    void on_goBackButton_clicked();
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddBooks *ui;
    void loadCategoryHints();
};

#endif // ADDBOOKS_H
