#ifndef SALOMEAUTOMATICMESHPROGRESSDIALOG_H
#define SALOMEAUTOMATICMESHPROGRESSDIALOG_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class SalomeAutomaticMeshProgressDialog;
}

class SalomeAutomaticMeshProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SalomeAutomaticMeshProgressDialog(QWidget *parent = 0);
    ~SalomeAutomaticMeshProgressDialog();
    QTextEdit * getTextEditor();

private:
    Ui::SalomeAutomaticMeshProgressDialog *ui;
};

#endif // SALOMEAUTOMATICMESHPROGRESSDIALOG_H
