#ifndef BASEBOOKVIEW_H
#define BASEBOOKVIEW_H

#include <QDialog>
#include <QSqlTableModel>
#include "database.h"

/**
 * @struct BookSearchParams
 * @brief Represents search and filtering parameters for books.
 *
 * This structure encapsulates all possible filtering fields
 * used when searching or filtering books in the system.
 */
struct BookSearchParams {
    QString title;        /**< Book title filter */
    QString author;       /**< Author name filter */
    QString publisher;    /**< Publisher name filter */
    QString id;           /**< Book ID filter */
    QString category;     /**< Book category filter */
    bool availableOnly;   /**< If true, only available books are included */
};

/**
 * @class BaseBookView
 * @brief Abstract base dialog for displaying and filtering book data.
 *
 * The BaseBookView class provides common functionality for book-related
 * views that display data using a QSqlTableModel. It includes logic for:
 * - Initializing the table model
 * - Applying dynamic search filters
 * - Configuring table appearance
 *
 * Derived classes must implement navigation behavior via onGoBack().
 */
class BaseBookView : public QDialog {
    Q_OBJECT

public:

    /**
     * @brief Constructs the BaseBookView dialog.
     *
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit BaseBookView(QWidget *parent = nullptr);

    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived classes.
     */
    virtual ~BaseBookView();

protected:

    /**
     * @brief Initializes the table model for a specific database table.
     *
     * @param tableName The name of the database table to bind to the model.
     *
     * Configures the QSqlTableModel to use the given table
     * and prepares it for display.
     */
    void initTableModel(const QString &tableName);

    /**
     * @brief Applies search filters to the table model.
     *
     * @param params Structure containing filtering criteria.
     *
     * Builds and applies a dynamic filter expression
     * based on the provided search parameters.
     */
    void applySearchFilter(const BookSearchParams &params);

    /**
     * @brief Configures the visual appearance of the table view.
     *
     * Adjusts column resizing, selection behavior,
     * and other display properties.
     */
    void setupTableAppearance();

    /**
     * @brief Handles navigation back to the previous view.
     *
     * This is a pure virtual function that must be implemented
     * by derived classes to define custom back-navigation behavior.
     */
    virtual void onGoBack() = 0;

    /**
     * @brief Pointer to the SQL table model used for displaying book data.
     */
    QSqlTableModel *model;
};

#endif // BASEBOOKVIEW_H
