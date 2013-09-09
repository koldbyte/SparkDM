#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTreeWidgetItem>
#include <QSystemTrayIcon>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QUdpSocket>
#include <QtCore>
#include <QTimer>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QClipboard>
#include <QFileDialog>
#include "dialogs/newdownloadinfodialog.h"
#include "dialogs/optionsdialog.h"
#include "systemtrayicon.h"
#include "dialogs/downloadpropertydialog.h"
//#include "backend/downloads.h"
#include "dialogs/adddownloaddialog.h"
#include "backend/downloadsdbmanager.h"


namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    SystemTrayIcon *TrayIcon;
    int CreateSystemTrayIcon();
    QMenu *TrayIconMenu;
    int AddUrlToAddDialog(QString);
    int StartListeningForAddUrl();
    AddDownloadDialog *adddownloaddialog;
    QUdpSocket *udpSocket;
    QItemSelectionModel *selectionModel;
    //QStandardItemModel *model;
    modelDownloads *model;
    QSortFilterProxyModel *proxyModel;
    DownloadsDBManager *dbMan;

public slots:
        int UpdateInterface();
        int ADownloadCompleted(QString);

private slots:
    void on_actionAdd_New_triggered();
    void on_actionQuit_triggered();
    void on_actionOptions_triggered();
    void on_categoriesTree_itemSelectionChanged();
    int TrayIconActivated(QSystemTrayIcon::ActivationReason);
    int ShowHideInterface();
    int ReadUrlAndProcess();

    void on_actionProperties_triggered();

    void on_actionDownload_Now_triggered();

    //void on_DownloadsTable_clicked(const QModelIndex &index);

    void on_actionRemove_triggered();

    void on_actionHide_Categories_triggered();

    void on_categoriesTree_customContextMenuRequested(const QPoint &pos);

    void on_dockWidgetContents_customContextMenuRequested(const QPoint &pos);

    void on_actionTo_text_file_triggered();

    void on_actionFrom_text_file_triggered();

    void on_actionDelete_All_Completed_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;


    void checkFirstRun();
    void setDefaultSettings();
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
