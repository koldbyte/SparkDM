#include "backend/modeldownloads.h"
#include<QtCore>
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlError>
modelDownloads::modelDownloads(QObject *parent,QSqlDatabase db)
:QSqlTableModel(parent,db)
{
    setTable("downloadsTable");
    setEditStrategy(modelDownloads::OnManualSubmit);
    select();
    setHeaders();
}

/*int modelDownloads::rowCount(const QModelIndex &parent) const
{
    return QSqlTableModel::rowCount(parent);
}

int modelDownloads::columnCount(const QModelIndex &parent ) const{
    return QSqlTableModel::columnCount(parent);
}
*/
/*QVariant modelDownloads::data(const QModelIndex &index, int role) const  {
    //if (!index.isValid())
    //        return QVariant();

    int row = index.row();
    int col = index.column();

    switch(role)
    {
    case Qt::DisplayRole:
        if(col==3){
            QDateTime date = QDateTime::fromMSecsSinceEpoch(1000 * itemData(index).value(3).toInt());
            return date.toLocalTime();
        }
        //else
        //return index.data(role).toString();
        //return "KB";
        break;
    case Qt::FontRole:
            if (row == 0 && col == 0) //change font only for cell(0,0)
            {
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
            break;
        case Qt::BackgroundRole:

            if (col == 2)  //change background only for cell(1,2)
            {
                QBrush redBackground(Qt::red);
                return redBackground;
            }
            break;
        case Qt::TextAlignmentRole:

            if (row == 1 && col == 1) //change text alignment only for cell(1,1)
            {
                return Qt::AlignRight + Qt::AlignVCenter;
            }
            break;
        case Qt::CheckStateRole:

            if (row == 1 && col == 0) //add a checkbox to cell(1,0)
            {
                return Qt::Checked;
            }
        }
        return QVariant();
}
*/
///////////////////////////////////
void modelDownloads::setFilterCustom(QString key, QString value){
    QString filter = key + "=" + value;
    setFilter(filter);
    select();
}

void modelDownloads::setFilterCustom(QString key, int value){
    QString filter = key + " = " + QString::number(value);
    setFilter(filter);
    select();
}

void modelDownloads::setFinishedStatus(int value){
    setFilterCustom("finished",value);
}

void modelDownloads::setType(int value){
    setFilterCustom("type",value);
}

void modelDownloads::setQueue(int value){
    setFilterCustom("queue",value);
}

void modelDownloads::setFilterDownloads(int status=-1,int type = -1, int queue=-1 ){
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
    setFilter(filters);
    select();
}

void modelDownloads::setHeaders(){

    setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("File Name"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("URL"));
    setHeaderData(3, Qt::Horizontal, QObject::tr("Date Added"));
    setHeaderData(4, Qt::Horizontal, QObject::tr("Queue"));
    setHeaderData(5, Qt::Horizontal, QObject::tr("Size"));
    setHeaderData(6, Qt::Horizontal, QObject::tr("Finished"));
    setHeaderData(7, Qt::Horizontal, QObject::tr("Status"));
    setHeaderData(8, Qt::Horizontal, QObject::tr("Time Left"));
    setHeaderData(9, Qt::Horizontal, QObject::tr("Transfer Rate"));
    setHeaderData(10, Qt::Horizontal, QObject::tr("Last Try Date"));
    setHeaderData(11, Qt::Horizontal, QObject::tr("Description"));
    setHeaderData(12, Qt::Horizontal, QObject::tr("Save to"));
    setHeaderData(13, Qt::Horizontal, QObject::tr("Referer"));
    setHeaderData(14, Qt::Horizontal, QObject::tr("Type"));
    setHeaderData(15, Qt::Horizontal, QObject::tr("Elapsed Time"));
    setHeaderData(16, Qt::Horizontal, QObject::tr("Pieces"));
    //removeColumn(12);

}

int modelDownloads::insertDownload(QString filename,QString url,QString loc,QString desc,int type=0,QString ref="",int queue=0,int pieces=10){
    qint64 dateAdded = QDateTime::currentMSecsSinceEpoch() / 1000;
    int size = 0; // make zero size by default
    int finished=0; // set status to unfinished
    QString status = "";
    int timeleft=0;// trivial data set to zero
    int transferRate=0;//trivial data set to zero
    int lastTry= QDateTime::currentMSecsSinceEpoch() / 1000; // TODO :: it should be string
    int elapsedTime =0;
    int newrow = 0;
    if(!insertRow(newrow)){
        QMessageBox::critical(0,"failed","rowfailed to insert",QMessageBox::Ok);
        return -1;
    }
    //setData(index(newrow,0),filename);
    setData(index(newrow,1),filename);
    setData(index(newrow,2),url);
    setData(index(newrow,3),QVariant::fromValue(dateAdded));
    setData(index(newrow,4),queue);
    setData(index(newrow,5),size);
    setData(index(newrow,6),finished);
    setData(index(newrow,7),status);
    setData(index(newrow,8),timeleft);
    setData(index(newrow,9),transferRate);
    setData(index(newrow,10),lastTry);
    setData(index(newrow,11),desc);
    setData(index(newrow,12),loc);
    setData(index(newrow,13),ref);
    setData(index(newrow,14),type);
    setData(index(newrow,15),elapsedTime);
    setData(index(newrow,16),pieces);
    if(!submitAll()){
        QSqlError err = lastError();
        QMessageBox::critical(0,"failed","failed to insert-"+ err.text(),QMessageBox::Ok);
        return -1;
    }
    return newrow;
}

void modelDownloads::deleteDownload(int row){
    if(!removeRow(row))
      QMessageBox::critical(0,"failed","failed to delete" + QString::number(row) + lastError().text(),QMessageBox::Ok);
    else {
        if(!submitAll())
            QMessageBox::critical(0,"failed","failed to submit delete-" + lastError().text() + QString::number(row),QMessageBox::Ok);
    }
}


Qt::ItemFlags modelDownloads::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSqlTableModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}



//////START CRUD OPERATIONS////
QString modelDownloads::getFileName(int row){
    return data(index(row,1)).toString();
}

void modelDownloads::setFileName(int row, QString Filename){
    setData(index(row,1),Filename);
   submitAll();
}

QString modelDownloads::getURL(int row){
    return data(index(row,2)).toString();
}

void modelDownloads::setURL(int row, QString URL){
    setData(index(row,2),URL);
    submitAll();
}

int modelDownloads::getDateAdded(int row){
    return data(index(row,3)).toInt();
}

void modelDownloads::setDateAdded(int row, int dateAdded){
    setData(index(row,3),dateAdded);
    submitAll();
}

int modelDownloads::getQueue(int row){
    return data(index(row,4)).toInt();
}

void modelDownloads::setQueue(int row,int queue){
    setData(index(row,4),queue);
    submitAll();
}

int modelDownloads::getSize(int row){
return data(index(row,5)).toInt();
}

void modelDownloads::setSize(int row,int size){
    setData(index(row,5),size);
    submitAll();
}

int modelDownloads::getFinishedStatus(int row){
return data(index(row,6)).toInt();
}

void modelDownloads::setFinishedStatus(int row,int finishedStatus){
    setData(index(row,6),finishedStatus);
    submitAll();
}

QString modelDownloads::getStatus(int row){
return data(index(row,7)).toString();
}

void modelDownloads::setStatus(int row,QString status){
    setData(index(row,7),status);
    submitAll();
}

int modelDownloads::getTimeleft(int row){
return data(index(row,8)).toInt();
}

void modelDownloads::setTimeLeft(int row,int timeleft){
    setData(index(row,8),timeleft);
    submitAll();
}

int modelDownloads::getTransferRate(int row){
return data(index(row,9)).toInt();
}

void modelDownloads::setTransferRate(int row,int transferRate){
    setData(index(row,9),transferRate);
    submitAll();
}

int modelDownloads::getLastTry(int row){
return data(index(row,10)).toInt();
}

void modelDownloads::setLastTry(int row,int lastry){
    setData(index(row,10),lastry);
    submitAll();
}

QString modelDownloads::getDescription(int row){
return data(index(row,11)).toString();
}

void modelDownloads::setDescription(int row,QString desc){
    setData(index(row,11),desc);
    submitAll();
}

QString modelDownloads::getSaveLocation(int row){
return data(index(row,12)).toString();
}

void modelDownloads::setSaveLocation(int row,QString saveLocation){
    setData(index(row,12),saveLocation);
    submitAll();
}

QString modelDownloads::getReferer(int row){
return data(index(row,13)).toString();
}

void modelDownloads::setReferer(int row,QString referer){
    setData(index(row,13),referer);
    submitAll();
}

int modelDownloads::getType(int row){
return data(index(row,14)).toInt();
}

void modelDownloads::setType(int row,int type){
    setData(index(row,14),type);
    submitAll();
}

int modelDownloads::getTimeElapsed(int row){
    return data(index(row,15)).toInt();
}

void modelDownloads::setTimeElapsed(int row,int timeElapsed){
    setData(index(row,15),timeElapsed);
    submitAll();
}

int modelDownloads::getPieces(int row){
return data(index(row,16)).toInt();
}

void modelDownloads::setPieces(int row,int pieces){
    setData(index(row,16),pieces);
    submitAll();
}
