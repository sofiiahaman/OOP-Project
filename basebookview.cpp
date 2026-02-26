#include "basebookview.h"
#include "services/LibraryService.h"
#include <QHeaderView>
#include <QMessageBox>

BaseBookView::BaseBookView(QWidget *parent) : QDialog(parent), model(nullptr) {}

BaseBookView::~BaseBookView() {
    delete model;
}

void BaseBookView::initTableModel(const QString &tableName) {
    if (!model) {
        model = new QSqlTableModel(this);
        model->setTable(tableName);
    }
}

void BaseBookView::setupTableAppearance() {
    if (!model) return;

    // Спільні назви колонок (DRY усунено)
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Назва книги");
    model->setHeaderData(2, Qt::Horizontal, "Автор");
    model->setHeaderData(3, Qt::Horizontal, "Видавництво");
    model->setHeaderData(4, Qt::Horizontal, "Жанр");
    model->setHeaderData(5, Qt::Horizontal, "Доступна");

    model->select();
}

void BaseBookView::applySearchFilter(const BookSearchParams &params) {
    Database db;
    LibraryService service(db);

    QString filter = service.buildFilterString(params);

    model->setFilter(filter);
    model->select();
}
