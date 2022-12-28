#ifndef MAP_EDIT_DLG_H
#define MAP_EDIT_DLG_H

#include <ros/ros.h>
#include <ros/package.h>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include <atomic>
#include "data_pool.h"
#include "ivlocmsg/ivsensorgps.h"
#include "common/base/log/include/log.h"
#include "common/common_header/struct_serialization.h"
#include "common/base/rviz/include/publish_pointcloud.h"
#include "common/base/rviz/include/publish_text.h"
#include "map/vectormap/include/geotool.h"

#include <std_msgs/ColorRGBA.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

struct XjMapProtocol
{
    double lon;
    double lat;
    double heading;
    double xg;
    double yg;
    double angle;
    int status;
    int point_type;
    double x;
    double y;

    XjMapProtocol() {
        lon = -1.0;
        lat = -1.0;
        heading = -1.0;
        xg = -1.0;
        yg = -1.0;
        angle = -1.0;
        status = -1;
        point_type = 0;
    }

    XjMapProtocol(double _lon, double _lat, double _heading,
                  double _xg, double _yg, double _angle, double _status, int _point_type) {
        lon = _lon;
        lat = _lat;
        heading = _heading;
        xg = _xg;
        yg = _yg;
        angle = _angle;
        status = _status;
        point_type = _point_type;
    }

    bool IsValid() {
        if (lon > 0 && lat > 0 && xg > 0 && yg > 0) {
            return true;
        }
        return false;
    }

    void Reset() {
        lon = -1.0;
        lat = -1.0;
        heading = -1.0;
        xg = -1.0;
        yg = -1.0;
        angle = -1.0;
        status = -1;
        point_type = 0;
    }

    void SetData(double _lon, double _lat, double _heading,
                 double _xg, double _yg, double _angle, double _status, int _point_type) {
        lon = _lon;
        lat = _lat;
        heading = _heading;
        xg = _xg;
        yg = _yg;
        angle = _angle;
        status = _status;
        point_type = _point_type;
    }


    void Serialization(stringstream &ss) {
        REGISTER_SERIALIZATION_ATTRIBUTE(lon, ',');
        REGISTER_SERIALIZATION_ATTRIBUTE(lat, ',');
        REGISTER_SERIALIZATION_ATTRIBUTE(heading, ',');
        REGISTER_SERIALIZATION_ATTRIBUTE(xg, ',');
        REGISTER_SERIALIZATION_ATTRIBUTE(yg, ',');
        REGISTER_SERIALIZATION_ATTRIBUTE(angle, ',');
        REGISTER_SERIALIZATION_ATTRIBUTE(status, ',');
        REGISTER_SERIALIZATION_ATTRIBUTE(point_type, ',');
        REGISTER_ENDLINE();
    }

    void DeSerialization(stringstream &ss) {
        REGISTER_DESERIALIZATION_ATTRIBUTE(lon, ',');
        REGISTER_DESERIALIZATION_ATTRIBUTE(lat, ',');
        REGISTER_DESERIALIZATION_ATTRIBUTE(heading, ',');
        REGISTER_DESERIALIZATION_ATTRIBUTE(xg, ',');
        REGISTER_DESERIALIZATION_ATTRIBUTE(yg, ',');
        REGISTER_DESERIALIZATION_ATTRIBUTE(angle, ',');
        REGISTER_DESERIALIZATION_ATTRIBUTE(status, ',');
        REGISTER_DESERIALIZATION_ATTRIBUTE(point_type, ',');
    }

    int SpliceData(const XjMapProtocol &target_data, std::vector<XjMapProtocol> &inner_target) {
        inner_target.clear();
        double inner_dis = hypot(target_data.xg - xg, target_data.yg - yg);
        if (inner_dis > 20) {
            AERROR << "cannot plice target over 20m";
            return -1;
        }
        if (inner_dis < 0.1) {
            return 0;
        }
        int max_counter = inner_dis / 0.05;

        for (int i = 1; i < max_counter; i++) {
            XjMapProtocol temp_data;
            temp_data.lon = lon + (target_data.lon - lon) * i / max_counter;
            temp_data.lat = lat + (target_data.lat - lat) * i / max_counter;
            temp_data.heading = heading + (target_data.heading - heading) * i / max_counter;
            temp_data.xg = xg + (target_data.xg - xg) * i / max_counter;
            temp_data.yg = yg + (target_data.yg - yg) * i / max_counter;
            temp_data.angle = angle + (target_data.angle - angle) * i / max_counter;
            temp_data.status = 1;
            temp_data.point_type = point_type;
            inner_target.push_back(temp_data);
        }
        return 0;
    }
};

namespace Ui {
class MapEditDlg;
}

class MapEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MapEditDlg(QWidget *parent = 0);
    ~MapEditDlg();

protected:
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void UpdateGpsInfo();
    void CallbackSensorgps(const ivlocmsg::ivsensorgps::ConstPtr &msg);
    void SaveMap();
    void OnLoadConfigParams();
    void OnSaveConfigParams();
    void OnScpMapToVehicle();

    bool ExportMap(std::string cmd , std::string &error);
    
private slots:
    void on_select_save_path_btn_clicked();

    void on_start_acq_map_btn_clicked();

    void on_add_function_btn_clicked();

    void on_select_import_data_btn_clicked();

    void on_road_attribut_combox_currentIndexChanged(int index);

    void on_function_combox_currentIndexChanged(int index);

    void on_export_map_btn_clicked();

    void on_select_map_bag_btn_clicked();

    void on_download_map_to_vehicle_btn_clicked();

    void on_start_recieve_gps_btn_clicked();

    void on_vehicle_map_path_edit_textChanged(const QString &arg1);

    void on_select_export_path_btn_clicked();

private:
    Ui::MapEditDlg *ui;

    QString map_bag_file_path_ = "";
    QString gps_txt_file_path_ = "";
    QString export_map_path_ = "";
    QString save_map_file_path_ = "";
    std::string vehicle_map_path_ = "";
    std::string shell_path_ = "";
    std_msgs::ColorRGBA point_color_;

    QString map_name_ = "";

    bool is_start_recieve_gps_ = false;
    bool is_start_acq_map_ = false;
    int  acq_map_status_ = 0;
    int  function_type_ = 0;
    bool add_function_ = false;
    int  point_count_ = 0;
    bool is_download_map_ = false;

    int m_nTimerId = 0;
    double gps_topic_hz_ = 0; 
    double gps_topic_start_time_ = 0;

    ros::NodeHandle nh_;
    ros::Subscriber sub_sensorgps_;

    SensorGPS sensor_gps_status_;

    avos::vectormap::GeoTool geo_tool_;
    avos::vectormap::PointGCCS last_gccs;
    std::mutex data_update_mutex;

    XjMapProtocol map_protocol;
    std::vector<XjMapProtocol> lane_data;    

    float left_extend_ = 2.0;
    float right_extand_ = 2.0;
    int   road_attribut_ = 2;
};

#endif // MAP_EDIT_DLG_H
