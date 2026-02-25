#include "basebookview.h"
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
    QStringList filters;

    if (!params.title.isEmpty())
        filters << QString("title LIKE '%%1%'").arg(params.title);
    if (!params.author.isEmpty())
        filters << QString("author LIKE '%%1%'").arg(params.author);
    if (!params.publisher.isEmpty())
        filters << QString("publisher LIKE '%%1%'").arg(params.publisher);
    if (!params.id.isEmpty())
        filters << QString("id = '%1'").arg(params.id);
    if (!params.category.isEmpty() && params.category != "Choose category")
        filters << QString("category = '%1'").arg(params.category);
    if (params.availableOnly)
        filters << "available = 1";

    model->setFilter(filters.join(" AND "));
    model->select();
}
