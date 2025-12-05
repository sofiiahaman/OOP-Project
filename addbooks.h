#ifndef ADDBOOKS_H
#define ADDBOOKS_H

#include <QDialog>
#include "ui_addbooks.h"

namespace Ui {
class AddBooks;
}

class QLineEdit;
class QPushButton;

class AddBooks : public QDialog
{
    Q_OBJECT

public:
    explicit AddBooks(QWidget *parent = nullptr);
    explicit AddBooks(bool testMode, QWidget *parent = nullptr);
    ~AddBooks();

    // Methods for injecting fields (used only in tests)
    void setBookNameLineEdit(QLineEdit *edit);
    void setBookAuthorLineEdit(QLineEdit *edit);
    void setBookPublisherLineEdit(QLineEdit *edit);
    void setBookCategoryLineEdit(QLineEdit *edit);

    // Getters so that tests can access the buttons / fields
    QLineEdit* getBookNameLineEdit() const;
    QLineEdit* getBookAuthorLineEdit() const;
    QLineEdit* getBookPublisherLineEdit() const;
    QLineEdit* getBookCategoryLineEdit() const;

    QPushButton* getSaveButton() const;
    QPushButton* getCancelButton() const;
    QPushButton* getGoBackButton() const;

private slots:
    void on_goBackButton_clicked();
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddBooks *ui;
    void loadCategoryHints();
};

#endif // ADDBOOKS_H
