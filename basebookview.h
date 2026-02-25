#ifndef BASEBOOKVIEW_H
#define BASEBOOKVIEW_H

#include <QDialog>
#include <QSqlTableModel>
#include "database.h"

struct BookSearchParams {
    QString title;
    QString author;
    QString publisher;
    QString id;
    QString category;
    bool availableOnly;
};

class BaseBookView : public QDialog {
    Q_OBJECT

public:
    explicit BaseBookView(QWidget *parent = nullptr);
    virtual ~BaseBookView();

protected:
    void initTableModel(const QString &tableName);
    void applySearchFilter(const BookSearchParams &params);
    void setupTableAppearance();

    virtual void onGoBack() = 0;

    QSqlTableModel *model;
};

#endif // BASEBOOKVIEW_H
