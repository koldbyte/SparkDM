#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::dbMan = new DownloadsDBManager();
    checkFirstRun();
    readSettings();
    model = new modelDownloads(this,dbMan->db);
    ui->DownloadsTable->setModel(model);
    ui->categoriesTree->setCurrentIndex(ui->categoriesTree->indexAt(QPoint(0,0)));
    CreateSystemTrayIcon();
    StartListeningForAddUrl();
    ///hide some columns
    ui->DownloadsTable->hideColumn(0);
    ui->DownloadsTable->hideColumn(4);
    ui->DownloadsTable->hideColumn(6);
    ui->DownloadsTable->hideColumn(14);
    ui->DownloadsTable->resizeColumnsToContents();
    //ui->DownloadsTable->
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdd_New_triggered()
{
    //Get Url from clipboard
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *pMime = clipboard->mimeData();
    //QMessageBox::information(this,"Clipboard",clipboard->text(),QMessageBox::Ok);
    if(pMime->hasUrls())
      {
          QUrl url;
          foreach(url, pMime->urls())
          {
              // do something with the url
              AddDownloadDialog *addDownloadUi = new AddDownloadDialog(this);
              addDownloadUi->setUrl(url.toDisplayString());
              addDownloadUi->setLocalModel(model);
              addDownloadUi->show();
          }
      }else{
        AddDownloadDialog *addDownloadUi = new AddDownloadDialog(this);
          QUrl rl = QUrl::fromUserInput(clipboard->text());
          addDownloadUi->setUrl(rl.toDisplayString());
          addDownloadUi->setLocalModel(model);
          addDownloadUi->show();
      }
}

void MainWindow::on_actionQuit_triggered()
{
    int reply = QMessageBox::warning(this,"Confirm","Are you sure you want to quit?",QMessageBox::Yes,QMessageBox::No);
    if(reply==QMessageBox::Yes){
        writeSettings();
        qApp->quit();
    }
}

int MainWindow::CreateSystemTrayIcon(){
    QAction *ShowHideAction = new QAction("Show/Hide Interface",this);
    connect(ShowHideAction, SIGNAL(triggered()), this, SLOT(ShowHideInterface()));
    QAction *QuitAction = new QAction("Quit",this);
    connect(QuitAction, SIGNAL(triggered()), this, SLOT(on_actionQuit_triggered()));

    TrayIconMenu = new QMenu();
    TrayIconMenu->addAction(ui->actionAdd_New);
    TrayIconMenu->addAction(ui->actionPause_All);
    TrayIconMenu->addSeparator();
    TrayIconMenu->addAction(ui->actionOptions);
    TrayIconMenu->addSeparator();
    TrayIconMenu->addAction(ShowHideAction);
    TrayIconMenu->addAction(QuitAction);
    ////////Bug Workaround///
    QSet<QWidget*> beforeWidgets = QApplication::topLevelWidgets().toSet();
    //////
    TrayIcon = new SystemTrayIcon(this);
    TrayIcon->setParent(this);
    TrayIcon->setIcon(QIcon(":/icons/images/icon.png"));
    TrayIcon->setContextMenu(TrayIconMenu);
    TrayIcon->setVisible(true);
    ///////Bug Workaround https://bugreports.qt-project.org/browse/QTBUG-30079 ////
    QSet<QWidget*> postWidgets = QApplication::topLevelWidgets().toSet();
    postWidgets -= beforeWidgets;
    if( !postWidgets.isEmpty() )
    {
         QWidget* sysWidget = (*postWidgets.begin());
         sysWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::SplashScreen );
         sysWidget->show();
    }
    ///////
    TrayIcon->setToolTip(qApp->applicationName());
    connect(TrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(TrayIconActivated(QSystemTrayIcon::ActivationReason)));
   // TrayIcon->showMessage("Welcome to Download Manager","This is a nice download manager",QSystemTrayIcon::Information,5000);
    return 0;
}

int MainWindow::ShowHideInterface(){
    if(isHidden()){
        show();
    }else{
        hide();
    }
    return 0;
}

int MainWindow::TrayIconActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason){
        case QSystemTrayIcon::Trigger:
            if(isHidden()){
                if(TrayIcon->blinking){
                    TrayIcon->setBlink(false);
                    TrayIcon->setIcon(QIcon(":/icons/images/icon.png"));
                }
                show();
            }else{
                hide();
            }
            break;
        case QSystemTrayIcon::DoubleClick:
            break;
        case QSystemTrayIcon::MiddleClick:
            break;
        default:
            ;
    }
    return 0;
}

void MainWindow::readSettings()
{
    QSettings settings(this);
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings(){
    QSettings settings(this);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::setDefaultSettings(){
    QSettings settings(this);

    //general start
    settings.setValue("maxRunningDownloads",4);
    settings.setValue("ConnectionsPerDownload",4);
    settings.setValue("showDownloadsCompleteDialog",true);
    settings.setValue("EnableSounds",true);
    //general end

    //saveto start
    QDir defaultLocation = QDir::home();
    defaultLocation.cd("Downloads");
    settings.setValue("generalDirectory",defaultLocation.path());

    defaultLocation.cd("Compressed");
    settings.setValue("compressedDirectory",defaultLocation.path());

    defaultLocation.cd("..");
    defaultLocation.cd("Documents");

    settings.setValue("documentDirectory",defaultLocation.path());

    defaultLocation.cd("..");
    defaultLocation.cd("Music");

    settings.setValue("musicDirectory",defaultLocation.path());

    defaultLocation.cd("..");
    defaultLocation.cd("Programs");

    settings.setValue("programDirectory",defaultLocation.path());

    defaultLocation.cd("..");
    defaultLocation.cd("Video");

    settings.setValue("videoDirectory",defaultLocation.path());
    //saveto end

    //categories start
    QString compressedExtensions= "zip rar r0* r1* arj gz sit sitx sea ace bz2 7z tar";
    settings.setValue("CompressedFileExtensions",compressedExtensions);
    settings.setValue("DocumentsFileExtensions","doc pdf ppt pps odt");
    settings.setValue("MusicFileExtensions","mp3 wav wma mpa ram ra aac aif m4a");
    settings.setValue("ProgramsExtensions","exe msi deb bin rpm run ");
    settings.setValue("VideoFileExtensions","avi mpg mpe mpeg asf wmv mov qt rm mp4 flv m4v webm ogv ogg");
    //categories end

    //network start
    settings.beginGroup(QLatin1String("proxy"));
    settings.setValue(QLatin1String("enabled"), false);
    settings.setValue(QLatin1String("type"), 0);
    settings.setValue(QLatin1String("hostName"),"");
    settings.setValue(QLatin1String("port"), 0);
    settings.setValue(QLatin1String("userName"), "");
    settings.setValue(QLatin1String("password"), "");
    settings.endGroup();
    //network end
}

void MainWindow::checkFirstRun(){
    QSettings settings(this);
    bool firstRun = settings.value("firstRun").toBool();
    if(firstRun){
        QDir defaultLocation = QDir::home();
        if(!defaultLocation.cd("Downloads")){
            defaultLocation.mkdir("Downloads");
            defaultLocation.cd("Downloads");
        }

        if(!defaultLocation.exists("Compressed")){
            defaultLocation.mkdir("Compressed");
        }

        if(!defaultLocation.exists("Documents")){
            defaultLocation.mkdir("Documents");
        }

        if(!defaultLocation.exists("Music")){
            defaultLocation.mkdir("Music");
        }

        if(!defaultLocation.exists("Programs")){
            defaultLocation.mkdir("Programs");
        }
        if(!defaultLocation.exists("Video")){
            defaultLocation.mkdir("Video");
        }
        setDefaultSettings();
        dbMan->createDB();
        settings.setValue("firstRun",false);
    }
}

void MainWindow::on_actionOptions_triggered()
{
    OptionsDialog *optionsUi = new OptionsDialog;
    optionsUi->show();
}

void MainWindow::on_categoriesTree_itemSelectionChanged(){
    int statusCat=-1;
    int typeCat=-1;
    int queueCat=-1;
    QTreeWidgetItem *currentCol = ui->categoriesTree->currentItem();
    QTreeWidgetItem *currentColP = ui->categoriesTree->currentItem()->parent();
    if(currentColP){
        //user is in subcategory
        QString parentCat = currentColP->text(ui->categoriesTree->currentColumn());
        if(parentCat.compare("All downloads")==0){
             statusCat=-1;//any
        }else if(parentCat.compare("Unfinished")==0){
            statusCat=0;//only unfinished
        }else if(parentCat.compare("Finished")==0){
            statusCat = 1;
        }else if(parentCat.compare("Queues")==0){
            statusCat = 0;//unfinished
        }
        QString childCat = currentCol->text(ui->categoriesTree->currentColumn());
        typeCat =-1;//any
        queueCat =-1;
        if(childCat.compare("Compressed")==0){
            typeCat =1;//1 for compressed
        }else if(childCat.compare("Documents")==0){
            typeCat =2;//2 for documents
        }
        else if(childCat.compare("Music")==0){
            typeCat =3;//3 for music
        }
        else if(childCat.compare("Programs")==0){
            typeCat =4;//4 for programs
        }
        else if(childCat.compare("Video")==0){
            typeCat =5;//5 for video
        }
        else if(childCat.compare("Main Queue")==0){
            queueCat=0;//0 for main queue
            statusCat = 0;//unfinished
        }
        else if(childCat.compare("Queue #1")==0){
            queueCat=1;//1 for queue 1
            statusCat = 0;//unfinished
        }
        else if(childCat.compare("Queue #2")==0){
            queueCat=2;//2 for queue 2
            statusCat = 0;//unfinished
        }
        else if(childCat.compare("Queue #3")==0){
            queueCat=3;//3 for queue 3
            statusCat = 0;//unfinished
        }
        model->setFilterDownloads(statusCat,typeCat,queueCat);
    }else {
        //user selected a parent category
        QString parentCat = currentCol->text(ui->categoriesTree->currentColumn());
        if(parentCat.compare("All downloads")==0){
             statusCat=-1;//any
        }else if(parentCat.compare("Unfinished")==0){
            statusCat=0;//only unfinished
        }else if(parentCat.compare("Finished")==0){
            statusCat = 1;
        }else if(parentCat.compare("Queues")==0){
            statusCat = 0;//unfinished
        }
        model->setFilterDownloads(statusCat,typeCat,queueCat);
    }
    ui->DownloadsTable->hideColumn(0);
    ui->DownloadsTable->hideColumn(4);
    ui->DownloadsTable->hideColumn(6);
    ui->DownloadsTable->hideColumn(14);
    ui->DownloadsTable->resizeColumnsToContents();
}

int MainWindow::StartListeningForAddUrl(){
    udpSocket = new QUdpSocket();
    udpSocket->bind(QHostAddress::LocalHost,45454);
    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(ReadUrlAndProcess()));
    return 0;
}

int MainWindow::ReadUrlAndProcess(){
    QByteArray datagram;
    while(udpSocket->hasPendingDatagrams()){
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
//      qDebug(QString(datagram.data()).toAscii().data());
    }
    AddUrlToAddDialog(QString(datagram.data()));
    return 0;
}

int MainWindow::AddUrlToAddDialog(QString Url){
    NewDownloadInfoDialog *newDownloadUi = new NewDownloadInfoDialog(0,Url);
    newDownloadUi->setLocalModel(model);
    newDownloadUi->show();
    return 0;
}

int MainWindow::UpdateInterface(){

    ui->DownloadsTable->resizeColumnsToContents();
    selectionModel= ui->DownloadsTable->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;
    int row = -1;
    foreach (index, indexes) {
        row = index.row();
    }

    int finished = model->data(model->index(row,6)).toInt();
    if(finished==0){
        ui->actionDownload_Now->setEnabled(true);
    }else if(finished ==1){
        ui->actionDownload_Now->setEnabled(false);
        ui->actionStop_Download->setEnabled(false);
    }

    QString value = model->data(model->index(row,7)).toString();
    if(value.contains("running")){
        ui->actionDownload_Now->setEnabled(false);
        ui->actionStop_Download->setEnabled(true);
    }else if(value.contains("paused")){
       ui->actionDownload_Now->setEnabled(true);
        ui->actionStop_Download->setEnabled(false);
    }else if(value.contains("error")){
        ui->actionDownload_Now->setEnabled(true);
         ui->actionStop_Download->setEnabled(false);
    }else if(value.contains("completed")){
        ui->actionDownload_Now->setEnabled(false);
         ui->actionStop_Download->setEnabled(false);
    }else if(value.contains("added")){
        ui->actionDownload_Now->setEnabled(false);
        ui->actionStop_Download->setEnabled(true);
    }

    //update the downloadsTableView
    //this->on_categoriesTree_itemSelectionChanged();
    return 0;
}

void MainWindow::on_actionProperties_triggered()
{
    selectionModel= ui->DownloadsTable->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;
    int row = -1;
    foreach (index, indexes) {
        row = index.row();
        DownloadPropertyDialog *dpd = new DownloadPropertyDialog();
        dpd->setLocalModel(model);
        dpd->setIndex(row);
        dpd->show();
     }
}

void MainWindow::on_actionDownload_Now_triggered()
{
    //TODO
    selectionModel= ui->DownloadsTable->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;
    int row = -1;
    foreach (index, indexes) {
        row = index.row();
        //QString filepath = model->data(model->index(row,12)).toString();
        QString url = model->data(model->index(row,2)).toString();
        //Downloads *mManager = new Downloads(this);
        //connect(mManager,SIGNAL(downloadComplete(QString)),this,SLOT(ADownloadCompleted(QString)));
        //connect(mManager,SIGNAL(progress(int)),this,SLOT(progress(int)));
        //mManager->startDownload(QUrl(url));
    }
}


int MainWindow::ADownloadCompleted(QString message){
    TrayIcon->showMessage("Download Completed",message,QSystemTrayIcon::Information,5000);
    if(isHidden()){
        if(!TrayIcon->blinking)
            TrayIcon->setBlink(true);
    }
    return 0;
}

void MainWindow::on_actionRemove_triggered()
{
    selectionModel= ui->DownloadsTable->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;
    int row = -1;
    int reply = QMessageBox::question(this,"Confirm","Do you really want to delete these " +QString::number(indexes.count()) +" download(s)?",QMessageBox::Yes , QMessageBox::No);
    if(reply == QMessageBox::Yes){
        foreach (index, indexes) {
            row = index.row();
            model->deleteDownload(row);
        }
    }
}


QString formatSize(qulonglong size, bool persec)
{
    QString rval;

    if(size < 1024)
        rval = QString("%L1 B").arg(size);
    else if(size < 1024*1024)
        rval = QString("%L1 KB").arg(size/1024);
    else if(size < 1024*1024*1024)
        rval = QString("%L1 MB").arg(double(size)/1024.0/1024.0, 0, 'f', 1);
    else
        rval = QString("%L1 GB").arg(double(size)/1024.0/1024.0/1024.0, 0, 'f', 1);

    if(persec) rval += "/s";
    return rval;
}

QString formatTime(qulonglong inval)
{
    QString result;
    qulonglong days,hrs,mins,secs;
    days = inval/(60*60*24);
    inval %= 60*60*24;

    hrs = inval/(60*60);
    inval %= 60*60;

    mins = inval/60;
    secs = inval%60;

    if(days)
        result = QString("%1d ").arg(days);
    if(hrs)
        result += QString("%1h ").arg(hrs);
    if(mins)
        result += QString("%1m ").arg(mins);
    if(secs)
        result += QString("%1s").arg(secs);

    return result;
}

void MainWindow::on_actionHide_Categories_triggered()
{
    if(ui->actionHide_Categories->text().compare("Hide Categories")==0){
        ui->dockCategories->close();
        ui->actionHide_Categories->setText("Show Categories");
    }else{
        ui->dockCategories->showNormal();
        ui->actionHide_Categories->setText("Hide Categories");
    }
}

void MainWindow::on_categoriesTree_customContextMenuRequested(const QPoint &pos)
{
    int x = pos.x();
    int y= pos.y();
    QMessageBox::information(this,"pos",QString::number(x) + " " +QString::number(y),QMessageBox::Ok);
}

void MainWindow::on_dockWidgetContents_customContextMenuRequested(const QPoint &pos)
{
    int x = pos.x();
    int y= pos.y();
    QMessageBox::information(this,"pos22",QString::number(x) + " " +QString::number(y),QMessageBox::Ok);

}

void MainWindow::on_actionTo_text_file_triggered()
{
    //export
    QFileDialog filedialog(this);
    filedialog.setWindowTitle("Select the folder where you want to export.");
    //filedialog.setOption(QFileDialog::ShowDirsOnly);
    filedialog.setFileMode(QFileDialog::Directory);
    //filedialog.setFileMode(filedialog.Directory);
    QStringList seldir;
    if(filedialog.exec()){
        seldir = filedialog.selectedFiles();
        QDir::setCurrent(seldir.first());
        QFile f;
        f.setFileName("export.txt");
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&f);
        for(int i=0;i<model->rowCount();i++){
            out << model->data(model->index(i,2)).toString() << "\n";
        }
        f.close();
        QMessageBox::information(this,"Exported Successfully","The current download list was successfully exported to \n"+seldir.first()+QDir::separator()+"export.txt",QMessageBox::Ok);
    }
    else return;
}

void MainWindow::on_actionFrom_text_file_triggered()
{
    //import
    QFileDialog filedialog(this);
    filedialog.setWindowTitle("Select the .txt file containing urls.");
    filedialog.setNameFilter(tr("Text (*.txt)"));;
    QStringList selfile;
    if(filedialog.exec()){
        selfile = filedialog.selectedFiles().first().split(QDir::separator());
        QFile f;
        f.setFileName(selfile.last());
        //QMessageBox::information(this,"file",selfile.last(),QMessageBox::Ok);
        selfile.removeLast();
        QDir d = QDir(selfile.join(QDir::separator()));
        d.setCurrent(d.absolutePath());
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&f);
        QString url;
        int count=0;
        url=in.readLine();
        while(!url.isNull()){
            AddUrlToAddDialog(url);
            count++;
            url = in.readLine();
        }
        f.close();
        QMessageBox::information(this,"Imported Successfully","Found " + QString::number(count)+" urls ",QMessageBox::Ok);
    }
    else return;
}

void MainWindow::on_actionDelete_All_Completed_triggered()
{
    int reply = QMessageBox::warning(this,"Confirm","Are you sure you want to DELETE the completed downloads from the list?\nNote:Actual files will not be deleted in this process.",QMessageBox::Yes,QMessageBox::No);
    if(reply==QMessageBox::Yes){
    int count=0;
    for(int i=0;i < model->rowCount();i++){
        if(model->getFinishedStatus(i)==1){
            model->removeRow(i);
            count++;
        }
    }
    QMessageBox::information(this,"Removed Completed Downloads","Successfully removed "+ QString::number(count)+" downloads.",QMessageBox::Ok);

    }
}

void MainWindow::on_actionAbout_triggered()
{
   // About123 *about =new About123();
    //about->show();
}
