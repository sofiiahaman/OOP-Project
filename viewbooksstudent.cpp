#include "viewbooksstudent.h"
#include "ui_viewbooksstudent.h"
#include "studentmenu.h"
#include <QHeaderView>

ViewBooksStudent::ViewBooksStudent(QWidget *parent)
    : BaseBookView(parent)
    , ui(new Ui::ViewBooksStudent)
{
    ui->setupUi(this);

    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    // Initialize the shared model
    initTableModel("books");
    ui->tableView->setModel(model);

    // Configure headers and appearance (from the base class)
    setupTableAppearance();

    // Display-specific settings
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model->select();
}

ViewBooksStudent::~ViewBooksStudent()
{
    delete ui;
}

// Implementation of back navigation for the student
void ViewBooksStudent::onGoBack()
{
    StudentMenu *studentMenu = new StudentMenu();
    studentMenu->show();
    this->close();
}

void ViewBooksStudent::on_goBackButton_clicked()
{
    onGoBack();
}

void ViewBooksStudent::on_searchButton_clicked()
{
    // Use a structure to collect search data
    BookSearchParams params;
    params.title = ui->searchBookLineEdit->text().trimmed();
    params.author = ui->authorLineEdit->text().trimmed();
    params.publisher = ui->publisherLineEdit->text().trimmed();
    params.id = ui->idLineEdit->text().trimmed();
    params.category = ui->categoryComboBox->currentText();
    params.availableOnly = ui->availableCheckBox->isChecked();

    // Call the unified filter
    applySearchFilter(params);
}

void ViewBooksStudent::on_clearButton_clicked()
{
    ui->searchBookLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->publisherLineEdit->clear();
    ui->idLineEdit->clear();
    ui->categoryComboBox->setCurrentIndex(0);
    ui->availableCheckBox->setChecked(false);

    // Reset to the full list using empty parameters
    applySearchFilter(BookSearchParams());
}
