#include "dialogs/newdownloadinfodialog.h"
#include "dialogs/ui_newdownloadinfodialog.h"
#include "backend/downloadsdbmanager.h"
//#include "backend/downloads.h"
#include<QString>
#include<QFileDialog>
#include <QSettings>
#include<QMessageBox>
NewDownloadInfoDialog::NewDownloadInfoDialog(QWidget *parent,QString UrlInput) :
    QDialog(parent),
    ui(new Ui::NewDownloadInfoDialog)
{
    ui->setupUi(this);
    ui->urlinput->setText(UrlInput);
    ui->descriptionEdit->setFocus();
    QSettings settings(this);
    switch(ui->categoryBox->currentIndex()){
    case 0: ui->SaveToEdit->setText(settings.value("generalDirectory").toString());break;
    case 1: ui->SaveToEdit->setText(settings.value("compressedDirectory").toString());break;
    case 2: ui->SaveToEdit->setText(settings.value("documentDirectory").toString());break;
    case 3: ui->SaveToEdit->setText(settings.value("musicDirectory").toString());break;
    case 4: ui->SaveToEdit->setText(settings.value("programDirectory").toString());break;
    case 5: ui->SaveToEdit->setText(settings.value("videoDirectory").toString());break;
    }
}

NewDownloadInfoDialog::~NewDownloadInfoDialog()
{
    delete ui;
}

void NewDownloadInfoDialog::on_browseSaveToButton_clicked()
{
    //browse to change Saveto
    QFileDialog filedialog(this);
    filedialog.setFileMode(filedialog.Directory);
    QStringList seldir;
    if(filedialog.exec())
        seldir = filedialog.selectedFiles();
    ui->SaveToEdit->setText(seldir.first());
}

void NewDownloadInfoDialog::on_pushButton_4_clicked()
{
    this->destroy();
}

int NewDownloadInfoDialog::insertANewDownload(){
    QString url = ui->urlinput->text();
    QString filename = url;//TODO
    QString loc = ui->SaveToEdit->text();
    QString desc = ui->descriptionEdit->toPlainText();
    int cat = ui->categoryBox->currentIndex();
    QString ref="";
    int queue = 0;
    int pieces=10;
    int newrow = localmodel->insertDownload(filename,url,loc,desc,cat,ref,queue,pieces);
    return newrow;
}

void NewDownloadInfoDialog::on_pushButton_2_clicked()
{
    //download later
    insertANewDownload();
    this->destroy();
}

void NewDownloadInfoDialog::on_categoryBox_currentIndexChanged(int index)
{
    QSettings settings(this);
    switch(index){
    case 0: ui->SaveToEdit->setText(settings.value("generalDirectory").toString());break;
    case 1: ui->SaveToEdit->setText(settings.value("compressedDirectory").toString());break;
    case 2: ui->SaveToEdit->setText(settings.value("documentDirectory").toString());break;
    case 3: ui->SaveToEdit->setText(settings.value("musicDirectory").toString());break;
    case 4: ui->SaveToEdit->setText(settings.value("programDirectory").toString());break;
    case 5: ui->SaveToEdit->setText(settings.value("videoDirectory").toString());break;
    }
}

void NewDownloadInfoDialog::on_pushButton_3_clicked()
{
    //download now
    int newrow = insertANewDownload();
    if(newrow!=-1){
    //start download
        //TODO
    //this->parent()->
    //QString filepath = ui->SaveToEdit->text();
    /*Downloads *dl = new Downloads();
    dl->SetupDownloadData(localmodel,newrow,filepath);
    dl->StartDownload();
    connect(dl,SIGNAL(completed(QString)),this,SLOT(ADownloadCompleted(QString)));
    connect(dl,SIGNAL(updateinterface()),this,SLOT(UpdateInterface()));
    */
    QString url = localmodel->data(localmodel->index(newrow,2)).toString();
    //Downloads *mManager = new Downloads(this);
    //connect(mManager,SIGNAL(downloadComplete()),this,SLOT(finished()));
    //connect(mManager,SIGNAL(progress(int)),this,SLOT(progress(int)));
    //mManager->startDownload(QUrl(url));
    }
    this->destroy();

}

void NewDownloadInfoDialog::setLocalModel(modelDownloads *model){
    localmodel = model;
}
