#include "backend/downloadsdbmanager.h"
#include "backend/modeldownloads.h"
#include <QtSql>
#include <QtSql/QtSql>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <backend/modeldownloads.h>

DownloadsDBManager::DownloadsDBManager()
{
    db =  QSqlDatabase::addDatabase("QSQLITE");
    openDB();
}
DownloadsDBManager::~DownloadsDBManager()
{
    if(db.isOpen())
        db.close();
    db.removeDatabase("QSQLITE");
}

bool DownloadsDBManager::openDB()
{
    if(!db.isOpen()){
    // Find QSLite driver
    //if(db.driverName().compare("QSQLITE")!=0)
    //db = QSqlDatabase::addDatabase("QSQLITE");
#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("downloadsDB");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
#else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName("downloadsDB");
#endif

    // Open databasee
    return db.open();
    }
    else return true;
}

void DownloadsDBManager::createDB(){
    //QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE");

    if (this->openDB()) {
        QSqlQuery query(db);
        QString createDBquery = "CREATE TABLE IF NOT EXISTS downloadsTable("
                "id INT PRIMARY KEY,"
                "filename VARCHAR(255) NOT NULL,"
                "url VARCHAR(1024) NOT NULL,"
                "dateAdded int,"
                "queue int,"
                "size int,"
                "finished int,"
                "status varchar(100),"
                "timeleft int,"
                "transferRate int,"
                "lastTry int,"
                "description text,"
                "saveTo varchar(1024),"
                "referer varchar(1024),"
                "type int,"
                "elapsedTime int,"
                "pieces int"
                ")";
        if (query.exec(createDBquery)){
            qDebug() << "DB created Successfully";
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot Create database"),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "This program needs SQLite support. Please read "
                                       "the Qt SQL driver documentation for information how "
                                       "to build it.\n\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

void DownloadsDBManager::closeDB( ){
    db.close();
}

void DownloadsDBManager::setHeaders(modelDownloads *model){

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Download ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("File Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("URL"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Added"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Queue"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Size"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("finished"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Status"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Time Left"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Transfer Rate"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Last Try Date"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(12, Qt::Horizontal, QObject::tr("Save to"));
    model->setHeaderData(13, Qt::Horizontal, QObject::tr("Referer"));
    model->setHeaderData(14, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(15, Qt::Horizontal, QObject::tr("Elapsed Time"));
    model->setHeaderData(16, Qt::Horizontal, QObject::tr("Pieces"));
    //model->removeColumn(12);

}

modelDownloads *DownloadsDBManager::queryDownloads(int status,int type,int queue){
    //QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE");

    if (!this->openDB()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "This program needs SQLite support. Please read "
                                       "the Qt SQL driver documentation for information how "
                                       "to build it.\n\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }

    modelDownloads *model = new modelDownloads(0,db);
    //modelDownloads *model = new modelDownloads::modelDownloads(0,db);
    model->setTable("downloadsTable");
    model->setEditStrategy(modelDownloads::OnManualSubmit);
    QString filters1="",filters2="",filters3="";
    if(status!=-1)
        filters1.append("finished="+QString::number(status));
    if(type!=-1)
        filters2.append("type="+QString::number(type));
    if(queue!=-1)
        filters3.append("queue="+QString::number(queue));
    QString filters="";
    if(filters1!=""){
        filters.append(filters1);
    }
    if(filters2!=""){
        if(filters!=""){
            filters.append(" AND ").append(filters2);
        }else{
            filters.append(filters2);
        }
    }
    if(filters3!=""){
        if(filters!=""){
            filters.append(" AND ").append(filters3);
        }else{
            filters.append(filters3);
        }
    }
    model->setFilter(filters);
    model->select();
    //model->data(model->index(1,1)).toString().append("bhaskar");
    //model->data(model->index(0,3)).toDateTime().toString();
    QDateTime d;
    for(int col=0;col<model->columnCount();col++){
    model->setData(model->index(col,3),d.fromMSecsSinceEpoch(model->data(model->index(col,3)).toInt()).toString());
    model->setData(model->index(col,10),d.fromMSecsSinceEpoch(model->data(model->index(col,10)).toInt()).toString());
}
    setHeaders(model);
    ///this->closeDB();
    return model;
}

int DownloadsDBManager::insertDownload(QString filename,QString url,QString loc,QString desc,int cat=0,QString ref="",int queue=0,int pieces=10){

        int dateAdded = QDateTime::currentMSecsSinceEpoch();
        int size = 0; // make zero size by default
        int finished=0; // set status to unfinished
        QString status = "";
        int timeleft=0;// trivial data set to zero
        int transferRate=0;//trivial data set to zero
        int lastTry= QDateTime::currentMSecsSinceEpoch(); // TODO :: it should be string
        int elapsedTime =0;
        //All data collected ....Now insert the download

        //QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE");
        if (openDB()) {
            QSqlQuery query(db);
            query.prepare("SELECT max(id) from downloadsTable;");
            query.exec();
            query.first();
            QString id = query.value(0).toString();
            QString insertDownloadquery = "INSERT INTO downloadsTable VALUES("
                    +QString::number(id.toInt() +1) +","
                    "'"+filename+"',"
                    "'"+url+"',"
                    +QString::number(dateAdded)+","
                    +QString::number(queue)+","
                    +QString::number(size)+","
                    +QString::number(finished)+","
                    "'"+status+"',"
                    +QString::number(timeleft)+","
                    +QString::number(transferRate)+","
                    +QString::number(lastTry)+","
                    "'"+desc+"',"
                    "'"+loc+"',"
                    "'"+ref+"',"
                    +QString::number(cat)+","
                    +QString::number(elapsedTime)+","
                    +QString::number(pieces)+
                      ");";
            if (query.exec(insertDownloadquery)){
                //qDebug() << "Download Added Successfully";
                this->closeDB();
                return(query.lastInsertId().toInt());
            } else {
                QMessageBox::critical(0, qApp->tr("Cannot Insert Download...Something went wrong.."),
                                      query.lastError().text()+"\n id == "+id, QMessageBox::Cancel);

            }
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                  qApp->tr("Unable to establish a database connection.\n"
                                           "Click Cancel to exit."), QMessageBox::Cancel);
        }
        this->closeDB();
        return -1;
}

void DownloadsDBManager::deleteDownload(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
            QMessageBox::information(0, qApp->tr("Download Deleted."),
                                  "Successfully removed the download from the list", QMessageBox::Cancel);
            //TODO
            //REMOVE FROM FILESYSTEM
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot delete the queue of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

//////START CRUD OPERATIONS////
QString DownloadsDBManager::getFileName(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT filename from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            QString filename = query.value(0).toString();
            return filename;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get Filename."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return "undetermined";
}

void DownloadsDBManager::setFileName(int id, QString Filename){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET filename = '"+Filename+"' WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the filename of the file."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

QString DownloadsDBManager::getURL(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT url from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            QString url = query.value(0).toString();
            return url;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get url."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return "unknown";
}

void DownloadsDBManager::setURL(int id, QString URL){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET url = '"+URL+"' WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the url of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getDateAdded(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT dateAdded from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int dateAdded = query.value(0).toInt();
            return dateAdded;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the date when the download was added."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return 0;
}

void DownloadsDBManager::setDateAdded(int id, int dateAdded){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET dateAdded = "+QString::number(dateAdded)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the date when the download was added."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getQueue(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT queue from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int queue = query.value(0).toInt();
            return queue;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the queue number."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return -1;
}

void DownloadsDBManager::setQueue(int id,int queue){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET queue = "+QString::number(queue)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the queue of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getSize(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT size from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int size = query.value(0).toInt();
            return size;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the size of the file."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return 0;
}

void DownloadsDBManager::setSize(int id,int size){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET size = "+QString::number(size)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the size of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getFinishedStatus(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT finished from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int fin = query.value(0).toInt();
            return fin;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the finished status."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return -1;
}

void DownloadsDBManager::setFinishedStatus(int id,int finishedStatus){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET finished = "+QString::number(finishedStatus)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the finished status of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

QString DownloadsDBManager::getStatus(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT status from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            QString status = query.value(0).toString();
            return status;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get status."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return "unknown";
}

void DownloadsDBManager::setStatus(int id,QString status){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET status = '"+status+"' WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the status of the download."),
                                  query.lastError().text()+query.lastQuery(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getTimeleft(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT timeleft from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int timeleft = query.value(0).toInt();
            return timeleft;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the time left."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return -1;
}

void DownloadsDBManager::setTimeLeft(int id,int timeleft){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET timeleft = "+QString::number(timeleft)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the time left for the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getTransferRate(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT transferRate from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int rate = query.value(0).toInt();
            return rate;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get transfer rate."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return 0;
}

void DownloadsDBManager::setTransferRate(int id,int transferRate){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET transferRate = "+QString::number(transferRate)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the transfer rate of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getLastTry(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT lastTry from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int last = query.value(0).toInt();
            return last;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the last try date."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return 0;
}

void DownloadsDBManager::setLastTry(int id,int lastry){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET lastTry = "+QString::number(lastry)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the date of last try of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

QString DownloadsDBManager::getDescription(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT description from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            QString desc = query.value(0).toString();
            return desc;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get description."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return "no description";
}

void DownloadsDBManager::setDescription(int id,QString desc){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET description = "+desc+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the description of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

QString DownloadsDBManager::getSaveLocation(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT saveTo from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            QString saveto = query.value(0).toString();
            return saveto;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get saved location."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return "";
}

void DownloadsDBManager::setSaveLocation(int id,QString saveLocation){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET saveTo = '"+saveLocation+"' WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the save location of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

QString DownloadsDBManager::getReferer(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT referer from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            QString ref = query.value(0).toString();
            return ref;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get referer."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return "";
}

void DownloadsDBManager::setReferer(int id,QString referer){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET referer = "+referer+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the referer of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getType(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT type from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int type = query.value(0).toInt();
            return type;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the type of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return 0;
}

void DownloadsDBManager::setType(int id,int type){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET type = "+QString::number(type)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the type of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getTimeElapsed(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT elapsedTime from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int elapsed = query.value(0).toInt();
            return elapsed;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the time elapsed."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return 0;
}

void DownloadsDBManager::setTimeElapsed(int id,int timeElapsed){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET elapsedTime = "+QString::number(timeElapsed)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the elapsed time of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}

int DownloadsDBManager::getPieces(int id){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("SELECT pieces from downloadsTable WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            query.first();
            int p = query.value(0).toInt();
            return p;
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot get the size of the file."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
    return 0;
}

void DownloadsDBManager::setPieces(int id,int pieces){
    if (this->openDB()) {
        QSqlQuery query(db);
        query.prepare("UPDATE downloadsTable SET pieces = "+QString::number(pieces)+" WHERE id =" +QString::number(id)+ ";");
        if (query.exec()){
            //success
        } else {
            QMessageBox::critical(0, qApp->tr("Cannot set the number of pieces of the download."),
                                  query.lastError().text(), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ///this->closeDB();
}
