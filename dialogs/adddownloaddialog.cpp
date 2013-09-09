#include "dialogs/adddownloaddialog.h"
#include "dialogs/ui_adddownloaddialog.h"
#include "dialogs/newdownloadinfodialog.h"

AddDownloadDialog::AddDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDownloadDialog)
{
    ui->setupUi(this);
}

AddDownloadDialog::~AddDownloadDialog()
{
    delete ui;
}
void AddDownloadDialog::setUrl(QString url)
{
    ui->URLinput->setText(url);
}

void AddDownloadDialog::on_pushButton_2_clicked()
{
    destroy(this);
}

void AddDownloadDialog::on_pushButton_clicked()
{
    NewDownloadInfoDialog *newDownloadUi = new NewDownloadInfoDialog(this,ui->URLinput->text());
    newDownloadUi->show();
    newDownloadUi->setLocalModel(localmodel);
    destroy(this);
}

void AddDownloadDialog::setLocalModel(modelDownloads *model){
    localmodel = model;
}
