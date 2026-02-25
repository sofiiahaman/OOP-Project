#ifndef VIEWBOOKSSTUDENT_H
#define VIEWBOOKSSTUDENT_H

#include "basebookview.h" // Inherit shared logic

namespace Ui {
class ViewBooksStudent;
}

class ViewBooksStudent : public BaseBookView
{
    Q_OBJECT

public:
    explicit ViewBooksStudent(QWidget *parent = nullptr);
    ~ViewBooksStudent();

protected:
    // Implementation of student-specific navigation
    void onGoBack() override;

private slots:
    void on_goBackButton_clicked();
    void on_searchButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::ViewBooksStudent *ui;
    // The model and loadBooks were removed — they are now in the base class
};

#endif // VIEWBOOKSSTUDENT_H
