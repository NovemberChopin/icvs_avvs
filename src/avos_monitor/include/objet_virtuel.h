#ifndef OBJET_VIRTUEL_H
#define OBJET_VIRTUEL_H
#include <ros/ros.h>
#include <ros/package.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtMath>

enum BOARD_SET_TYPE  
{  
    SPEED,   //速度，km/h,0-220 
    DRIVING_MILEAGE,      //续航里程，r/min,0-8000
    BATTERY_LIGHT,   //0:无，1：低电量提醒  
    BRAKE_LIGHT,  
    TURN,  //转向灯，-1：左转，1：右转； 2：双闪
    LIGHT_STATUS,  //1：近光灯，2：远光灯，
    WARNING,  //0：无警告，不显示；1:轻微警告 绿，2：一般警告，黄；3：严重警告，红
    BELT_STATUS,      //安全带，1：未扣提醒，0：不提示
    SIGNAL,    //4G 信号强度，共4格
    LOCATION_STATUS, //定位状态,０:定位成功;１:定位一般;２:定位失败;３:IMU初始化中;４:激光初始化中;５:地图加载中;
    SHIFT_POSITION,    //档位：0:无效;1:R档;2:N档;3:D档
    TEMPERATURE,         //温度显示
    BATTERY_VALUE,
    VEHICLE_STATUS,
    DOOR_STATUS,
    OVER_TEMP,
    LIDAR_STATUS,    //激光雷达：1开0关
    AEB_ENABLE,       //AEB生效
    TARGET_TORQUE,
    TOTAL_DISTANCE,
    ON_LINE_STATUS,
    ULTRASONIC_STATUS
};  

class objet_virtuel : public QGraphicsItem
{
public:
    objet_virtuel(QGraphicsItem *parent=nullptr);

    float getValue() const;
    void setValue(float value);

    float getValue(BOARD_SET_TYPE type) const;
    void setValue(BOARD_SET_TYPE type, float value);

    int getValue(BOARD_SET_TYPE type, int index) const;
    void setValue(BOARD_SET_TYPE type, int index, int value);

    int getValueMax() const;

    QString styleTexte;

protected:
    float value;//la vitesse actualisée
    int valueMax;//la vitesse maximum
    int location_status = 0;
    int braking_value = 0;
    int battery_value = 100;
    int vehicle_status = 0;
    int signal_value = 0;
    int belt_status = 0;
    int door_status = 0;
    int over_temperature = 0;
    int light_status = 0;
    int lidar_status = 0;
    int aeb_enable = 0;
    int driving_mileage = 0;
    int total_distance = 0;
    int on_line_status = 0;

    uint8_t utl_status[8];
};

#endif // OBJET_VIRTUEL_H
