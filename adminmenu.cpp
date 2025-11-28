#include "adminmenu.h"
#include "ui_adminmenu.h"
#include "addbooks.h"
#include "viewbooks.h"
#include "addstudents.h"
#include "viewstudentinformation.h"
#include "issuebooks.h"
#include "returnbooks.h"
#include "editstudentinformation.h"
#include "database.h"
#include "mainwindow.h"
#include "logoutdialog.h"
#include <QMessageBox>

AdminMenu::AdminMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminMenu)
{
    ui->setupUi(this);

    loadDashboardStats();
}

AdminMenu::~AdminMenu()
{
    delete ui;
}

void AdminMenu::loadDashboardStats()
{
    Database db;
    if(db.openConnection()) {

        ui->labelBooksCount->setText(QString::number(db.getBooksCount()));
        ui->labelAuthorsCount->setText(QString::number(db.getAuthorsCount()));
        ui->labelCategoriesCount->setText(QString::number(db.getCategoriesCount()));
        ui->labelPublishersCount->setText(QString::number(db.getPublishersCount()));
        ui->labelStudentCount->setText(QString::number(db.getStudentsCount()));
        ui->labelBookAvailableCount->setText(QString::number(db.getAvailableBooksCount()));
        ui->labelBookIssueCount->setText(QString::number(db.getIssuedBooksCount()));
        ui->labelBookReturnedCount->setText(QString::number(db.getReturnedBooksCount()));
        ui->labelBookNotReturnCount->setText(QString::number(db.getNotReturnBooksCount()));
    }

    db.closeConnection();
}


void AdminMenu::on_actionAdd_new_book_triggered()
{
    AddBooks *addBookDialog = new AddBooks(); // create a new window
    addBookDialog->show();                    // open it

    this->close();                            // close AdminMenu
}

void AdminMenu::on_actionView_books_triggered()
{
    ViewBooks *viewBooks = new ViewBooks();
    viewBooks->show();  // open the window for viewing books

    this->close();      // close adminmenu
}

void AdminMenu::on_actionAdd_student_triggered()
{
    AddStudents *addStudents = new AddStudents();
    addStudents->show();   // open a new window
    this->close();         // close adminmenu
}

void AdminMenu::on_actionView_student_information_triggered()
{
    ViewStudentInformation *viewInfo = new ViewStudentInformation();
    viewInfo->show();
    this->close();        // close adminmenu
}

void AdminMenu::on_actionIssue_books_triggered()
{
    IssueBooks *issueBooks = new IssueBooks();
    issueBooks->show();
    this->close();     // close adminmenu
}

void AdminMenu::on_actionReturn_books_triggered()
{
    ReturnBooks *rb = new ReturnBooks();
    rb->show();
    this->close();
}

void AdminMenu::on_actionEdit_student_information_triggered()
{
    EditStudentInformation *editInfo = new EditStudentInformation();
    editInfo->show();
    this->close();
}

void AdminMenu::on_actionLogout_triggered()
{
    LogoutDialog dlg(this);

    if (dlg.exec() == QDialog::Accepted)
    {
        MainWindow *mainwin = new MainWindow();
        mainwin->show();
        this->close();
    }
}
