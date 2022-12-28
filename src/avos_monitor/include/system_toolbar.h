#ifndef SYSTEM_TOOLBAR_H
#define SYSTEM_TOOLBAR_H

#include <ros/ros.h>
#include <ros/package.h>
#include <iostream>
#include <string.h>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>
#include <QTimerEvent>

#include "imu/SensorImu.h"
#include "planning_msgs/Trajectory.h"
#include "mapengine_msgs/Navigation.h"
#include "common/base/rviz/include/publish_base_map.h"
#include "common/base/rviz/include/publish_cube.h"
#include "common/base/rviz/include/publish_pointcloud.h"
#include "common/base/rviz/include/publish_sphere.h"
#include "common/base/rviz/include/publish_text.h"
#include "map/map_loader/include/map_loader.h"
#include "map/map_loader/include/show_vectormap.h"

#include "prediction_msgs/PredictionObjects.h"

#include "data_pool.h"
#include "map_edit_dlg.h"
#include "task_manage_dlg.h"
#include "can_debug_dlg.h"
#include "map_select_dlg.h"
#include "hugo_scene.h"
#include "settings.h"

#include <std_msgs/ColorRGBA.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include "tf2/utils.h"
#include <tf/transform_broadcaster.h>

#include "common_msgs/FaultVec.h"
#include "canbus_msgs/CanFault.h"
#include "monitor_msgs/MonitorDebug.h"
#include "control_msgs/control_sys.h"
#include "ivcanbus/ivcansensor.h"
#include "imu/SensorImu.h"
#include "control_msgs/chassis_status.h"
#include "perception_msgs/TrafficLightDetection.h"
#include "perception_msgs/RadarObjectList.h"
#include "perception_msgs/RadarObject.h"
#include "business_platform/BusinessStatus.h"
#include "mainstream_msgs/VehiclePerception.h"
#include "ivlocmsg/ivsensorgps.h"
#include "control_msgs/LoncontrolDebug.h"

using namespace avos::map;
using namespace avos::common;
// using namespace avvs;
// using namespace avos::map;

using avos::map::Mapinfo; 
using avos::map::MapLoader;

class SystemToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit SystemToolbar(QWidget *parent = 0);
    ~SystemToolbar();

protected:
    void timerEvent(QTimerEvent *event);

private:
    QTableWidget    *show_stat_table;
    QTableWidget    *sensor_table_;
    QGraphicsView   *graphicsView;
    scene_globale   *dashboard;  

    QPushButton *pTaskManageBtn;
    QPushButton *pSettingBtn;
    QPushButton *pStartDebugBtn;
    QPushButton *pMapManageBtn;
    QPushButton *pBagPlayBtn;
    QPushButton *pCanDebugBtn;
    QPushButton *pMapEditBtn;

    MapSelectDlg            *map_select_dlg_ = nullptr;
    TaskManageDlg           *task_manage_dlg_ = nullptr;
    CanDebugDlg             *can_debug_dlg_ = nullptr;
    MapEditDlg              *map_edit_dlg_ = nullptr;
    Settings 				*settings = nullptr;

    std::string config_file_path = "";

    common_msgs::FaultVec           car_faults_msg_;         //结点消息

    ros::Subscriber sub_tpcarfaults;
    ros::Subscriber sub_tpmonitordb103;
    ros::Subscriber sub_tpcontrolsys;
    ros::Subscriber sub_tpimu;
    ros::Subscriber sub_tpcansensor;
    ros::Subscriber sub_chassis_status;
    ros::Subscriber sub_traffic_light;
    ros::Subscriber sub_tpbusiness_status;
    ros::Subscriber sub_lon_control_debug;//lon_control_debug

    ros::Subscriber sub_fl_radar;
    ros::Subscriber sub_fr_radar;
    ros::Subscriber sub_rl_radar;

    int m_nTimerId = 0;
    int m_nTimerShow = 0;
    bool is_first;
    bool is_debug_status_ = false;

    QColor text_color_;
    QColor fault_level_color_0_;
    QColor fault_level_color_1_;
    QColor fault_level_color_2_;
    QColor gray_color_;  

private:	
    void OnCreateButton();
    void InitTableView(QTableWidget* table);
    void InitSensorTable(QTableWidget* table);
    void InitRosSub();
    void ExitDebug();
    void UpdateStatus();
    void UpdateDashboardStatus();
    void SetValue(BOARD_SET_TYPE type, float value);


    void CallbackCarfaults(const common_msgs::FaultVec::ConstPtr &msg);
    void CallbackVehicleFault(const canbus_msgs::CanFault::ConstPtr &msg);
    void CallbackControlSys(const control_msgs::control_sys::ConstPtr &msg);
    void CallbackSensorIMU(const imu::SensorImu::ConstPtr &msg);
    void CallbackCanSensor(const ivcanbus::ivcansensor::ConstPtr &msg);
    void CallbackClassisState(const control_msgs::chassis_status::ConstPtr &msg);
    void CallbackTrafficLight(const perception_msgs::TrafficLightDetection::ConstPtr &msg);//tptrafficlight
    void CallbackBusiness(const business_platform::BusinessStatus::ConstPtr &msg);//tpbusiness_status
    void CallbackSensorgps(const ivlocmsg::ivsensorgps::ConstPtr &msg);
    void CallbackLonDebug(const control_msgs::LoncontrolDebug::ConstPtr &msg);

    void CallbackFLRadar(const perception_msgs::RadarObjectList::ConstPtr &msg);
    void CallbackFRRadar(const perception_msgs::RadarObjectList::ConstPtr &msg);
    void CallbackRLRadar(const perception_msgs::RadarObjectList::ConstPtr &msg);

public Q_SLOTS:
    void SettingButton();
    void OnbagRecordButton();
    void OnDebugButton();
    void OnBagReplayButton();
    void OnTaskManageButton();
    void OnCanDebugButton();
    void OnMapEditButton();    
    void OnMapManageButton();

private:
    void InitShowCar(ros::NodeHandle nh);
    void DisplayCar();
    uint8_t repeatTime;
    visualization_msgs::Marker car_;
    ros::Publisher pub_car_;

    float length;
    float width;
    float height;
    std::string car_source_path;

private:
    PublishCube<std::vector<CubeSphereObject>> gobjectfusion_cube;
    PublishText<std::vector<CubeSphereObject>> gobjectfusion_text;
    PublishSphere<std::vector<CubeSphereObject>> gobjectfusion_debug;
    ObjectPointCloud<std::vector<perception_msgs::ObstacleCell>> gstatic_cell;
    ObjectPointCloud<std::vector<perception_msgs::ObstacleCell>> gobject_cell;
    ObjectPointCloud<std::vector<planning_msgs::TrajectoryPoint>> gtrajectory;
    ObjectPointCloud<std::vector<mapengine_msgs::ReferencePoint>> mapengine_path;
    ObjectPointCloud<std::vector<mapengine_msgs::ReferencePoint>> left_mapengine_path;
    ObjectPointCloud<std::vector<mapengine_msgs::ReferencePoint>> right_mapengine_path;
    ObjectPointCloud<std::vector<mapengine_msgs::ReferencePoint>> rev_mapengine_path;
    PublishText<std::vector<planning_msgs::TrajectoryPoint>> gtrajectory_text;
    ObjectPointCloud<std::vector<PointVCS>> show_vector_map_;
    PublishBasemap publish_image;
    ObjectPointCloud<std::vector<pcl::PointXYZL>> gcar_layout;
    double xg_, yg_, heading_;
    visualization_msgs::MarkerArray obstacle_bbox;
    std::vector<mapengine_msgs::Navigation> mapengine_list_;
    double pathplan_time_ = 0.0;
    visualization_msgs::MarkerArray prediction_trajectory_array_;

    ros::Subscriber sub_imu_;
    ros::Subscriber sub_pathplan_;
    ros::Subscriber sub_perception_;
    ros::Subscriber sub_perception_debug_;
    ros::Subscriber sub_mapegine_;
    ros::Subscriber sub_prediction_;
    ros::Subscriber sub_sensorgps_;

    ros::Publisher pub_mapengine_marker;
    ros::Publisher pub_left_mapengine_marker;
    ros::Publisher pub_right_mapengine_marker;
    ros::Publisher pub_rev_mapengine_marker;
    ros::Publisher pub_trajectory_marker;
    ros::Publisher pub_trajectory_text_marker;
    ros::Publisher pub_object_marker;
    ros::Publisher pub_object_bbox;
    ros::Publisher pub_objectcell;
    ros::Publisher pub_staticcell;
    ros::Publisher pub_base_map;
    ros::Publisher pub_vector_map_;
    ros::Publisher pub_object_text_marker;
    ros::Publisher pub_object_debug_marker_;
    ros::Publisher pub_car_cloud_;
    ros::Publisher pub_prediction_trajectory_;
    ros::Publisher pub_path_marker_;

    visualization_msgs::Marker path_marker_;
    std::string global_frame_;
    PointVCS vsc_point_;
    PointGCCS car_pgccs_;
    avos::vectormap::GeoTool geo_tool_;

    std_msgs::ColorRGBA getColor(float speed);
    void GetCarCells();
    void CallbackMapengine(const mapengine_msgs::Navigation::ConstPtr &msg);
    void CallbackTrajectory(const planning_msgs::Trajectory::ConstPtr&  msg);
    void CallbackPerceptionDebug(const perception_msgs::PerceptionObjects::ConstPtr &msg) ;
    geometry_msgs::Point ConvertPoint(const Eigen::Vector3f& point);
    visualization_msgs::MarkerArray VizObjectBBox( const std::vector<CubeSphereObject>& obstacles);
    void CallbackPerception(const mainstream_msgs::VehiclePerception::ConstPtr &msg);
    void MakeLane(string str_namespace , visualization_msgs::Marker &output, LaneParameter param);
    void MakePredictLines(std::string str_namespace, const prediction_msgs::PredictionObjects::ConstPtr &objects,
                          visualization_msgs::MarkerArray &output_lines);
    void CallbackPredictionObjects(const prediction_msgs::PredictionObjects::ConstPtr &objects);
    void TimeMatching();
    void PublisherRvizThread();

    bool LoadBaseMap();
    bool LoadVectorMap();
    void ShowBaseMap(void);
    void ShowVectormap(void);
};

#endif // SYSTEM_TOOLBAR_H


// QTabWidget::pane
// {
//     border-width:1px;border-color:rgb(48, 104, 151);
//     border-style: outset;background-color: rgb(132, 171, 208);
//     background: transparent;
// } 
// QTabWidget::tab-bar
// {
//     border-width:0px;
// }
// QTabBar::tab
// {
//     border-bottom-color: #C2C7CB;
//     border-top-left-radius: 0px;
//     border-top-right-radius: 0px;
//     max-width: 75px; min-width:75px; min-height:25px;
//     font:14px Times New Roman;
//     padding: 0px;
// }
// QTabBar::scroller
// {
//     width:25;
//     border:0;
//     padding: 0px;
// }
// QTabBar QToolButton::right-arrow
// {
//     background-color:rgb(132, 171, 208);
//     border-width: 0;
//     background-image: url(:/images/tab/rightbtn.png);
// }
// QTabBar QToolButton::right-arrow:hover 
// {
//     background-color:rgb(132, 171, 208);
//     border-width: 0;
//     background-image: url(:/images/tab/hoverrightbtn.png);
// }
// QTabBar QToolButton::right-arrow:disabled
// {
//     background-color:rgb(132, 171, 208);
//     border-width: 0;
//     background-image: url(:/images/tab/grayrightbtn.png);
// }
// QTabBar QToolButton::left-arrow
// {
//     background-color:rgb(132, 171, 208);
//     border-width: 0;
//     background-image: url(:/images/tab/leftbtn.png);
// }
// QTabBar QToolButton::left-arrow:hover
// {
//     background-color:rgb(132, 171, 208);
//     border-width: 0;
//     background-image: url(:/images/tab/hoverleftbtn.png);
// }
// QTabBar QToolButton::left-arrow:disabled
// {
//     background-color:rgb(132, 171, 208);
//     border-width: 0;
//     background-image: url(:/images/tab/grayleftbtn.png);
// }
// QTabBar::tab:first:selected
// {
//     margin-left: 30; margin-right: 0;
//     color: white;
//     border-image: url(:/images/tab/sel3.png);
// }
// QTabBar::tab:first:!selected
// {
//     color: black;
//     margin-left: 30; margin-right: 0;
//     border-image: url(:/images/tab/normal3.png);
// }
// QTabBar::tab:first:hover:!selected {
//     color: black;
//     margin-left: 30; margin-right: 0;
//     border-image: url(:/images/tab/hover3.png);
// }
// QTabBar::tab:middle:selected
// {
//     margin-top: 0; margin-left: -15; margin-right: 8;
//     color: white;
//     border-image: url(:/images/tab/sel3.png);
// }
// QTabBar::tab:middle:!selected {
//     color: black;
//     margin-top: 0; margin-left: -15; margin-right: 8;
//     border-image: url(:/images/tab/normal3.png);
// }
// QTabBar::tab:middle:hover:!selected 
// {
//     color: black;
//     margin-top: 0; margin-left: -15; margin-right: 8;
//     border-image: url(:/images/tab/hover3.png);
// }
// QTabBar::tab:last:selected
// {
//     margin-top: 0px; margin-left: 0; margin-right: 0;
//     color: white;
//     border-image: url();
// }
// QTabBar::tab:last:!selected
// {
//     color: black;
//     margin-top: 0; margin-left: 0; margin-right: 0;
//     border-image: url();
// }
// QTabBar::tab:last:hover:!selected
// {
//     color: black;
//     margin-top: 0; margin-left: 0; margin-right: 0;
//     border-image: url();
// }
