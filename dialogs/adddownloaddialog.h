#ifndef ADDDOWNLOADDIALOG_H
#define ADDDOWNLOADDIALOG_H

#include <QDialog>
#include <backend/modeldownloads.h>
#include "backend/downloadsdbmanager.h"
namespace Ui {
class AddDownloadDialog;
}

class AddDownloadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddDownloadDialog(QWidget *parent = 0);
    ~AddDownloadDialog();
    void setUrl(QString url);
    modelDownloads *localmodel;
    void setLocalModel(modelDownloads *);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::AddDownloadDialog *ui;
};

#endif // ADDDOWNLOADDIALOG_H
