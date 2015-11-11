#ifndef MULTIZONESELECTFILESDIALOG_H
#define MULTIZONESELECTFILESDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class MultizoneSelectFilesDialog;
}

class MultizoneSelectFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultizoneSelectFilesDialog(QWidget *parent = 0);
    ~MultizoneSelectFilesDialog();

    QString getConfigFilePath();

    QString getMeshFilePath();

    bool hasSelectedFiles();
    void setProjectPath(QString _path);

private slots:
    void on_importedConfigNameToolButton_clicked();

    void on_meshFileToolButton_clicked();

    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::MultizoneSelectFilesDialog *ui;

    QString _configFilePath;
    QString _meshFilePath;
    QString _projectPath;

    QString _fullConfigFilePath;
    QString _fullMeshFilePath;

    bool _hasSelectedFiles;
};

#endif // MULTIZONESELECTFILESDIALOG_H
