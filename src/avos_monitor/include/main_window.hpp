/**
 * @file /include/avos_monitor/main_window.hpp
 *
 * @brief Qt based gui for avos_monitor.
 *
 * @date November 2010
 **/
#ifndef cyrobot_monitor_MAIN_WINDOW_H
#define cyrobot_monitor_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

//#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include "addtopics.h"
#include "settings.h"
#include "qrviz.hpp"

#include "QProcess"
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QSoundEffect>
#include <QComboBox>
#include <QSpinBox>
#include <QVariant>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <map>
//rviz
#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>
#include <rviz/tool_manager.h>
#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>
#include <rviz/tool.h>
#include <ros/package.h>

#include "data_pool.h"
#include "task_manage_dlg.h"
#include "system_toolbar.h"
#include "switchbutton.h"
#include "RosTopicWidget.h"
#include "VehicleInformationWidget.h"

#include "avos_service_bridge.h"
#include "business_platform_api_msgs/BusinessCloudInfo.h"
#include "business_platform_api_msgs/AvosServiceReply.h"
#include "business_platform_api_msgs/BagrecordTopicInfo.h"


// using namespace avvs;
typedef enum{
    AUTO_DRIVER,
    TURN_LEFT_LIGHT,
    TURN_RIGHT_LIGHT,
    TURN_LEFT,
    TURN_RIGHT,
    LOW_BEAM,
    HIGH_BEAM
}VehicleControlType;

namespace Ui {
class MainWindow;
}

/*****************************************************************************
** Namespace
*****************************************************************************/
namespace avos_monitor {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(int argc, char** argv, QWidget *parent = nullptr);
    ~MainWindow();
    
    void ReadSettings(); // Load up qt program settings at startup
    void WriteSettings(); // Save qt program settings when closing

    void closeEvent(QCloseEvent *event); // Overloaded function
    void timerEvent(QTimerEvent *event);
    
    void showNoMasterMessage();
    void initRviz();
    void initUis();
    void initVideos();
    void initTopicList();
    void load_rviz_params();
    void set2DPos();
    QString getParamString(QString section, QString name);

    bool rviz_params_loaded = false;    
public slots:
    /******************************************
    ** Auto-connections (connectSlotsByName())
    *******************************************/
    void on_actionAbout_triggered();
    void on_checkbox_use_environment_stateChanged(int state);
    void slot_rosShutdown();
    void quick_cmds_check_change(int);
    void cmd_output();
    void cmd_error_output();
    void refreashTopicList();
    void RvizGetModel(QAbstractItemModel *model);
    /******************************************
    ** Manual connections
    *******************************************/
    void updateLoggingView(); // no idea why this can't connect automatically4
    void slot_tab_manage_currentChanged(int);
    void slot_tab_Widget_currentChanged(int);
    void slot_choose_topic(QTreeWidgetItem *choose, QString name);
    void slot_set_2D_Goal();
    void slot_set_2D_Pos();
    void slot_set_select();
    void slot_move_camera_btn();
    void on_set_rviz_view_btn_clicked();
    void on_return_to_zero_btn_clicked();
    //设置界面
    void slot_setting_frame();
    void quick_cmd_add();
    void quick_cmd_remove();
    //显示图像
    //显示图像
    void slot_show_image(int,QImage);
    //    void on_horizontalSlider_raw_valueChanged(int value);
private slots:
    void on_pushButton_add_topic_clicked();

    void on_pushButton_remove_topic_clicked();

    void on_pushButton_rename_topic_clicked();

    void on_treeView_rvizDisplayTree_clicked(const QModelIndex &index);

    void on_button_disconnect_clicked();
    /// \brief 导入rviz Display配置
    void on_pushButton_rvizReadDisplaySet_clicked();
    /// \brief 导出rviz Display配置
    void on_pushButton_rvizSaveDisplaySet_clicked();

    void on_button_connect_clicked();    

    void on_action_reboot_system_triggered();
    void on_record_bag_btn_clicked();
    void on_go_end_point_btn_clicked();

    void on_switch_button_gps(bool checked);    
    void on_switch_button_lidar(bool checked);    
    void on_switch_button_mill(bool checked);    
    void on_switch_button_camera(bool checked);    
    void on_switch_button_perception(bool checked);    
    void on_switch_button_localization(bool checked);    
    void on_switch_button_control(bool checked);    
    void on_switch_button_planning(bool checked);    
    void on_switch_button_canbus(bool checked);    
    void on_switch_button_task(bool checked);       
    void on_switch_button_control_all(bool checked);   

    void on_radio_button_automode_changed(bool value);  

    void on_turn_left_light_btn_clicked();
    void on_front_low_light_btn_clicked();
    void on_turn_right_light_btn_clicked();
    void on_turn_left_btn_clicked();
    void on_front_high_light_btn_clicked();
    void on_turn_right_btn_clicked(); 
    void on_save_display_setting_btn_clicked();

private:
    Ui::MainWindowDesign *ui;
    void initData();
    QString JudgeDisplayNewName(QString name);

    void connections();
    void add_quick_cmd(QString name,QString shell);

    void inform(QString);
    bool AskInform(QString);
    QString getUserName();

    bool CheckLidarStatus();
    bool CheckNavStatus();
    bool CheckMmStatus();
    void UpdateStatus();

    bool CheckAvosStatus();
    bool VehicleControl(VehicleControlType type, int cmd, QString &msg);
    bool BagRecordStart(QString & msg);

    int m_nTimerId = 0;
    bool rviz_view_changed_ = false;
    QNode qnode;
    QProcess *quick_cmd = nullptr;
    QProcess *close_remote_cmd = nullptr;
    QProcess *base_cmd = nullptr;
    QRviz *map_rviz_ = nullptr;
    QStandardItemModel* treeView_rviz_model = nullptr;
    AddTopics *addtopic_form = nullptr;
    //存放rviz treewidget当前显示的控件及控件的父亲的地址
    QMap <QWidget*,QTreeWidgetItem *> widget_to_parentItem_map;
    //存放状态栏的对应关系 display名 状态item
    QMap <QString,QTreeWidgetItem *> tree_rviz_stues;
    //存放display的当前值 item名，参数名称和值
    QMap <QTreeWidgetItem*,QMap<QString,QString>> tree_rviz_values;
    Settings *set = nullptr;
    QSoundEffect *media_player = nullptr;

    QAbstractItemModel* m_modelRvizDisplay;

    QMap<QString, QString> m_mapRvizDisplays;

    QString m_sRvizDisplayChooseName_;

    TaskManageDlg *task_manage_dlg;

    SwitchButton *switch_gps_btn;
    SwitchButton *switch_lidar_radar_btn;
    SwitchButton *switch_mill_radar_btn;
    SwitchButton *switch_camera_btn;
    SwitchButton *switch_perception_btn;
    SwitchButton *switch_localization_btn;
    SwitchButton *switch_control_btn;
    SwitchButton *switch_planning_btn;
    SwitchButton *switch_canbus_btn;
    SwitchButton *switch_task_btn;
    SwitchButton *switch_control_all_btn;

};
}// namespace avos_monitor

#endif // cyrobot_monitor_MAIN_WINDOW_H
