#ifndef MAP_SELECT_DLG_H
#define MAP_SELECT_DLG_H

#include <ros/ros.h>
#include <ros/time.h>
// rosbag tools
#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <ros/package.h>
#include <iostream>
#include <QDialog>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QDebug>

#include <QDomNode>
#include <QFile>   
// #include <QtXml\QtXml>
// #include <QtXml\QDomDocument> 
#include <map>
#include <vector>
#include <iterator>
#include <task_msgs/BusinessStatus.h>
#include "data_pool.h"

// #define foreach(variable, container)

using namespace std;
namespace Ui {
class MapSelectDlg;
}

class MapSelectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MapSelectDlg(QWidget *parent = 0);
    ~MapSelectDlg();

    void closeEvent(QCloseEvent *event);

private Q_SLOTS:
    void on_load_map_btn_clicked();

    void on_select_accept_btn_clicked();

    void on_load_default_map_btn_clicked();

private:
    Ui::MapSelectDlg *ui;
    QStandardItemModel *model_ = NULL;
    QStandardItemModel *map_list_model_ = NULL;

    rosbag::Bag  bag_;
    QDomDocument m_doc;
    QString select_map_id_ = "";
    QString default_map_id_ = "";
    QString map_xml_file_path_ = "";
    QString map_file_path_ = "";
    std::string vehicle_map_path_ = "";
    std::string select_map_path_ = "";

    std::string config_file_path = "";

    bool is_copy_map_success_ = false;

public:
    std::string current_map_id_ = "";

private:
	bool CopyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
	bool CopyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
	bool OpenXmlFile(QString FilePath);
	bool ModifyMapID(QString map_xml_file_path, QString map_id);
    void CopyMapfileFromVehicle();

    void GetAllFileFolder(QString dirPath);
    bool GetMapInfo(QString map_info_file_path);
    bool GetDefaultMapID(QString map_xml_file_path);
    bool CheckExistMapID(QString map_id);


    void OnLoadConfigParams();
    void OnSaveConfigParams();
};

#endif // MAP_SELECT_DLG_H
