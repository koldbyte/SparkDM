#ifndef NEWDOWNLOADINFODIALOG_H
#define NEWDOWNLOADINFODIALOG_H

#include <QDialog>
#include <backend/modeldownloads.h>
#include "backend/downloadsdbmanager.h"
namespace Ui {
class NewDownloadInfoDialog;
}

class NewDownloadInfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewDownloadInfoDialog(QWidget *parent = 0,QString UrlInput="");
    ~NewDownloadInfoDialog();
    void setLocalModel(modelDownloads *);
    modelDownloads *localmodel;

private slots:
    void on_browseSaveToButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_categoryBox_currentIndexChanged(int index);
    void on_pushButton_3_clicked();
    int insertANewDownload();


private:
    Ui::NewDownloadInfoDialog *ui;
};

#endif // NEWDOWNLOADINFODIALOG_H
