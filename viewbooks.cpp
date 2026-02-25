#include "viewbooks.h"
#include "ui_viewbooks.h"
#include "adminmenu.h"
#include <QHeaderView>

ViewBooks::ViewBooks(QWidget *parent)
    : BaseBookView(parent) // Call the base class constructor
    , ui(new Ui::ViewBooks)
{
    ui->setupUi(this);

    // 1. Configure icons
    ui->goBackButton->setIcon(QIcon(":/icons/icons/left-arrow.png"));

    // 2. Initialize the model via the base class
    initTableModel("books");

    // 3. Connect the model to the table and configure its appearance
    ui->tableView->setModel(model);
    setupTableAppearance();

    // 4. Configure table parameters specific to this window
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Load initial data
    model->select();
}

ViewBooks::~ViewBooks()
{
    delete ui;
}

/* ========================
 * Implementation of the virtual navigation method
 * ======================== */
void ViewBooks::onGoBack()
{
    AdminMenu *adminMenu = new AdminMenu();
    adminMenu->show();
    this->close();
}

/* ========================
 * UI event handling
 * ======================== */

void ViewBooks::on_goBackButton_clicked()
{
    onGoBack();
}

void ViewBooks::on_searchButton_clicked()
{
    // Collect search parameters into a structure (defined in basebookview.h)
    BookSearchParams params;
    params.title = ui->searchBookLineEdit->text().trimmed();
    params.author = ui->authorLineEdit->text().trimmed();
    params.publisher = ui->publisherLineEdit->text().trimmed();
    params.id = ui->idLineEdit->text().trimmed();
    params.category = ui->categoryComboBox->currentText();
    params.availableOnly = ui->availableCheckBox->isChecked();

    // Call the unified filtering method from the base class
    applySearchFilter(params);
}

void ViewBooks::on_clearButton_clicked()
{
    // Clear UI fields
    ui->searchBookLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->publisherLineEdit->clear();
    ui->idLineEdit->clear();
    ui->categoryComboBox->setCurrentIndex(0);
    ui->availableCheckBox->setChecked(false);

    // Reset the filter via the base class
    applySearchFilter(BookSearchParams());
}
