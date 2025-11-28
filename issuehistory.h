#ifndef ISSUEHISTORY_H
#define ISSUEHISTORY_H

#include <QDialog>

namespace Ui {
class IssueHistory;
}

class IssueHistory : public QDialog
{
    Q_OBJECT

public:
    explicit IssueHistory(QWidget *parent = nullptr);
    ~IssueHistory();

private:
    Ui::IssueHistory *ui;
};

#endif // ISSUEHISTORY_H
