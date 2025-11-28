#include "issuehistory.h"
#include "ui_issuehistory.h"

IssueHistory::IssueHistory(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IssueHistory)
{
    ui->setupUi(this);
}

IssueHistory::~IssueHistory()
{
    delete ui;
}
