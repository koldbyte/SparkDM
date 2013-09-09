#ifndef MODELDOWNLOADS_H
#define MODELDOWNLOADS_H
#include <QSqlTableModel>
#include<QSqlDatabase>
class modelDownloads : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit modelDownloads(QObject *parent = 0,QSqlDatabase db=QSqlDatabase());
    void setFilterCustom(QString key, int value);
    void setFilterCustom(QString key, QString value);
    void setFinishedStatus(int value);
    void setType(int value);
    void setQueue(int value);
    void setFilterDownloads(int status,int type, int queue );
    void setHeaders();

    int insertDownload(QString filename,QString url,QString loc,QString desc,int type,QString ref,int queue,int pieces);
    void deleteDownload(int row);
    //modelDownloads(QObject *parent = 0,QSqlDatabase *db);
    //int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    //int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

Qt::ItemFlags flags(const QModelIndex &index) const;

    QString getFileName(int row);
    void setFileName(int row, QString Filename);
    QString getURL(int row);
    void setURL(int row, QString URL);
    int getDateAdded(int row);
    void setDateAdded(int row, int dateAdded);
    int getQueue(int row);
    void setQueue(int row,int queue);
    int getSize(int row);
    void setSize(int row,int size);
    int getFinishedStatus(int row);
    void setFinishedStatus(int row,int finishedStatus);
    QString getStatus(int row);
    void setStatus(int row,QString status);
    int getTimeleft(int row);
    void setTimeLeft(int row,int timeleft);
    int getTransferRate(int row);
    void setTransferRate(int row,int transferRate);
    int getLastTry(int row);
    void setLastTry(int row,int lastry);
    QString getDescription(int row);
    void setDescription(int row,QString desc);
    QString getSaveLocation(int row);
    void setSaveLocation(int row,QString saveLocation);
    QString getReferer(int row);
    void setReferer(int row,QString referer);
    int getType(int row);
    void setType(int row,int type);
    int getTimeElapsed(int row);
    void setTimeElapsed(int row,int timeElapsed);
    int getPieces(int row);
    void setPieces(int row,int pieces);

signals:

public slots:

};

#endif // MODELDOWNLOADS_H
