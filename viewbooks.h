#ifndef VIEWBOOKS_H
#define VIEWBOOKS_H

#include "basebookview.h" // Inherit logic from the base class

namespace Ui {
class ViewBooks;
}

class ViewBooks : public BaseBookView
{
    Q_OBJECT

public:
    explicit ViewBooks(QWidget *parent = nullptr);
    ~ViewBooks();

protected:
    // Implementation of administrator-specific navigation
    void onGoBack() override;

private slots:
    // UI event handlers
    void on_goBackButton_clicked();
    void on_searchButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::ViewBooks *ui;

    // The model pointer and loadBooks method were removed,
    // since they are now located in BaseBookView
};

#endif // VIEWBOOKS_H
