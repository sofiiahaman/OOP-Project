#include "adminmenu.h"
#include "ui_adminmenu.h"

#include "addbooks.h"
#include "viewbooks.h"
#include "addstudents.h"
#include "viewstudentinformation.h"
#include "issuebooks.h"
#include "returnbooks.h"
#include "editstudentinformation.h"
#include "mainwindow.h"
#include "logoutdialog.h"

#include "./repositories/StatisticsRepository.h"
#include "database.h"

#include <QMessageBox>

AdminMenu::AdminMenu(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::AdminMenu)
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

    if (!db.openConnection()) {
        QMessageBox::critical(this, "Error", "Cannot connect to database!");
        return;
    }

    StatisticsRepository statsRepo(&db);

    ui->labelBooksCount->setText(QString::number(statsRepo.getBooksCount()));
    ui->labelAuthorsCount->setText(QString::number(statsRepo.getAuthorsCount()));
    ui->labelCategoriesCount->setText(QString::number(statsRepo.getCategoriesCount()));
    ui->labelPublishersCount->setText(QString::number(statsRepo.getPublishersCount()));
    ui->labelStudentCount->setText(QString::number(statsRepo.getStudentsCount()));

    ui->labelBookAvailableCount->setText(QString::number(statsRepo.getAvailableBooksCount()));
    ui->labelBookIssueCount->setText(QString::number(statsRepo.getIssuedBooksCount()));
    ui->labelBookReturnedCount->setText(QString::number(statsRepo.getReturnedBooksCount()));
    ui->labelBookNotReturnCount->setText(QString::number(statsRepo.getNotReturnedBooksCount()));

    db.closeConnection();
}

void AdminMenu::on_actionAdd_new_book_triggered()
{
    AddBooks *addBookDialog = new AddBooks();
    addBookDialog->show();
    this->close();
}

void AdminMenu::on_actionView_books_triggered()
{
    ViewBooks *viewBooks = new ViewBooks();
    viewBooks->show();
    this->close();
}

void AdminMenu::on_actionAdd_student_triggered()
{
    AddStudents *addStudents = new AddStudents();
    addStudents->show();
    this->close();
}

void AdminMenu::on_actionView_student_information_triggered()
{
    ViewStudentInformation *viewInfo = new ViewStudentInformation();
    viewInfo->show();
    this->close();
}

void AdminMenu::on_actionIssue_books_triggered()
{
    IssueBooks *issueBooks = new IssueBooks();
    issueBooks->show();
    this->close();
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
