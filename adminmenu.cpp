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
        ui->labelBookIssueCount->setText(QString::number(db.getIssuedBooksCount()));
        ui->labelBookReturnedCount->setText(QString::number(db.getReturnedBooksCount()));
        ui->labelBookNotReturnCount->setText(QString::number(db.getNotReturnBooksCount()));
    }

    // Якщо хочеш лише активні:
    // ui->labelBookIssueCount->setText(
    //     QString::number(db.getActiveIssuedBooksCount())
    // );

    db.closeConnection();
}


void AdminMenu::on_actionAdd_new_book_triggered()
{
    AddBooks *addBookDialog = new AddBooks(); // створюємо нове вікно
    addBookDialog->show();                    // відкриваємо його

    this->close();                            // закриваємо AdminMenu
}

void AdminMenu::on_actionView_books_triggered()
{
    ViewBooks *viewBooks = new ViewBooks();
    viewBooks->show();  // відкриває вікно перегляду книг

    this->close();      // закриває adminmenu
}

void AdminMenu::on_actionAdd_student_triggered()
{
    AddStudents *addStudents = new AddStudents();
    addStudents->show();   // відкриваємо нове вікно
    this->close();         // закриваємо adminmenu
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
    this->close();     // закриває adminmenu
}

void AdminMenu::on_actionReturn_books_triggered()
{
    ReturnBooks *rb = new ReturnBooks();
    rb->show();        // відкриває вікно
    this->close();     // закриває adminmenu
}

void AdminMenu::on_actionEdit_student_information_triggered()
{
    EditStudentInformation *editInfo = new EditStudentInformation();
    editInfo->show();        // відкриває вікно
    this->close();     // закриває adminmenu
}

void AdminMenu::on_actionLogout_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Logout",
                                  "Are you sure you want to logout?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();   // відкриває головне вікно
        this->close();        // закриває AdminMenu
    }
    else
    {
        // Якщо натиснули No – нічого не робимо
        return;
    }
}
