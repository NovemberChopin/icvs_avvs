#include "../include/settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings),
    service_client("/planning/service")
{
    ui->setupUi(this);
    QSettings video_topic_setting("video_topic","avos_monitor");
    QStringList names=video_topic_setting.value("names").toStringList();
    QStringList topics=video_topic_setting.value("topics").toStringList();
    if(names.size()==5)
    {
        ui->video0_name_set->setText(names[0]);
        ui->video0_name_set_2->setText(names[1]);
        ui->video0_name_set_3->setText(names[2]);
        ui->video0_name_set_4->setText(names[3]);
        ui->video0_name_set_5->setText(names[4]);
    }
    if(topics.size()==5)
    {
        ui->video0_topic_set->setText(topics[0]);
        ui->video0_topic_set_2->setText(topics[1]);
        ui->video0_topic_set_3->setText(topics[2]);
        ui->video0_topic_set_4->setText(topics[3]);
        ui->video0_topic_set_5->setText(topics[4]);
    }

    QSettings main_setting("topic_setting","avos_monitor");
    // ui->lineEdit_odm->setText(main_setting.value("topic_odom","raw_odom").toString());
    // ui->lineEdit_power->setText(main_setting.value("topic_power","power").toString());
    // ui->lineEdit_power_min->setText(main_setting.value("power_min","10").toString());
    // ui->lineEdit_power_max->setText(main_setting.value("power_max","12").toString());
//    ui->lineEdit_odom->setText(main_setting.value("odom_topic","amcl_pose").toString());


// float32 user_max_speed//速度限制，８－１５
// float32 user_follow_time//跟车时间，单位为秒；自车与前车可能碰撞的时间，最小２秒
// float32 user_follow_dis//跟车距离，自车到前车的距离限制－以上最小碰撞距离
// float32 user_stop_dis//跟停距离，如果前车停车，自车会跟停，与前车的距限制>5
// float32 user_collision_buff//碰撞检测，就是避障距离，最小的横向偏差
// float32 user_pullover_dis//距离终点多少米开始执行靠边车，最小４０
// bool enable_avoid//是否避障，ture:可以避障，false:不避障，遇障碍会停车，避障消失会自动启动
    QSettings autodriver_setting("autodriver_setting","avos_monitor");
    ui->user_max_speed_edit->setText(autodriver_setting.value("user_max_speed","15").toString());
    ui->user_follow_dis_edit->setText(autodriver_setting.value("user_follow_dis","10").toString());
    ui->user_follow_time_edit->setText(autodriver_setting.value("user_follow_time","2").toString());
    ui->user_collision_buff_edit->setText(autodriver_setting.value("user_collision_buff","0.3").toString());
    ui->user_stop_dis_edit->setText(autodriver_setting.value("user_stop_dis","5").toString());
    ui->user_pullover_dis_edit->setText(autodriver_setting.value("user_pullover_dis","40").toString());
    ui->enable_avoid_rbtn->setChecked(autodriver_setting.value("enable_avoid","1").toBool());


    QSettings vehicle_setting("vehicle_setting","avos_monitor");
    ui->vehicle_length_edit->setText(vehicle_setting.value("vehicle_length","4.3").toString());
    ui->vehicle_width_edit->setText(vehicle_setting.value("vehicle_width","1.8").toString());
    ui->vehicle_heigth_edit->setText(vehicle_setting.value("vehicle_heigth","1.7").toString());
    ui->vehicle_alxe_edit->setText(vehicle_setting.value("vehicle_alxe","1.35").toString());
    ui->front_axle_edit->setText(vehicle_setting.value("front_axle","0.8").toString());
    ui->back_alxe_edit->setText(vehicle_setting.value("back_alxe","2.15").toString());
    ui->vehicle_vin_edit->setText(vehicle_setting.value("vehicle_vin","IDPIVP3010008B0008").toString());
    ui->vehicle_version_edit->setText(vehicle_setting.value("avos_version","V0.0.0.0").toString());


    QSettings sensor_param("sensor_param","avos_monitor");
    ui->lidar_ip_edit->setText(sensor_param.value("lidar_ip","192.168.1.200").toString());
    ui->msop_port_edit->setText(sensor_param.value("msop_port","2368").toString());
    ui->difop_port_edit->setText(sensor_param.value("difop_port","2369").toString());

    ui->navigation_port_edit->setText(sensor_param.value("navigation_port","/dev/ttyTHS4").toString());
    ui->navigation_butr_edit->setText(sensor_param.value("navigation_butr","230400").toString());

// 601  603   mmw0
// 611  613   mmw1
// 631  633   mmw3
// 651  653   mmw5
// 671  673   mmw7
    ui->mm_fl_state_id_edit->setText(sensor_param.value("mm_fl_state_id","601").toString());
    ui->mm_fl_table_id_edit->setText(sensor_param.value("mm_fl_table_id","603").toString());

    ui->mm_fm_state_id_edit->setText(sensor_param.value("mm_fm_state_id","611").toString());
    ui->mm_fm_table_id_edit->setText(sensor_param.value("mm_fm_table_id","613").toString());

    ui->mm_fr_state_id_edit->setText(sensor_param.value("mm_fr_state_id","631").toString());
    ui->mm_fr_table_id_edit->setText(sensor_param.value("mm_fr_table_id","633").toString());

    ui->mm_bl_state_id_edit->setText(sensor_param.value("mm_bl_state_id","651").toString());
    ui->mm_bl_table_id_edit->setText(sensor_param.value("mm_bl_table_id","653").toString());

    ui->mm_br_state_id_edit->setText(sensor_param.value("mm_br_state_id","671").toString());
    ui->mm_br_table_id_edit->setText(sensor_param.value("mm_br_table_id","673").toString());



    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slot_ok_btn_click()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(slot_cancel_btn_click()));
    connect(ui->autodriver_param_apply_btn,SIGNAL(clicked()),this,SLOT(slot_apply_param_click()));
    connect(ui->autodriver_param_reset_btn,SIGNAL(clicked()),this,SLOT(slot_reset_param_click()));
}


Settings::~Settings()
{
    delete ui;
}


void Settings::slot_ok_btn_click()
{
    QSettings main_setting("topic_setting","avos_monitor");
    // main_setting.setValue("topic_odom",ui->lineEdit_odm->text());
    // main_setting.setValue("topic_power",ui->lineEdit_power->text());
    // main_setting.setValue("power_min",ui->lineEdit_power_min->text());
    // main_setting.setValue("power_max",ui->lineEdit_power_max->text());

    QSettings autodriver_setting("autodriver_setting","avos_monitor");
    autodriver_setting.setValue("user_max_speed",ui->user_max_speed_edit->text());
    autodriver_setting.setValue("user_follow_dis",ui->user_follow_dis_edit->text());
    autodriver_setting.setValue("user_follow_time",ui->user_follow_time_edit->text());
    autodriver_setting.setValue("user_collision_buff",ui->user_collision_buff_edit->text());
    autodriver_setting.setValue("user_stop_dis",ui->user_stop_dis_edit->text());
    autodriver_setting.setValue("user_pullover_dis",ui->user_pullover_dis_edit->text());
    autodriver_setting.setValue("enable_avoid",ui->enable_avoid_rbtn->isChecked());

    QSettings vehicle_setting("vehicle_setting","avos_monitor");
    vehicle_setting.setValue("vehicle_length",ui->vehicle_length_edit->text());
    vehicle_setting.setValue("vehicle_width",ui->vehicle_width_edit->text());
    vehicle_setting.setValue("vehicle_heigth",ui->vehicle_heigth_edit->text());
    vehicle_setting.setValue("vehicle_alxe",ui->vehicle_alxe_edit->text());
    vehicle_setting.setValue("front_axle",ui->front_axle_edit->text());
    vehicle_setting.setValue("back_alxe",ui->back_alxe_edit->text());
    vehicle_setting.setValue("vehicle_vin",ui->vehicle_vin_edit->text());
    vehicle_setting.setValue("avos_version",ui->vehicle_version_edit->text());

    QSettings sensor_param("sensor_param","avos_monitor");

    sensor_param.setValue("lidar_ip",ui->lidar_ip_edit->text());
    sensor_param.setValue("msop_port",ui->msop_port_edit->text());
    sensor_param.setValue("difop_port",ui->difop_port_edit->text());

    sensor_param.setValue("navigation_port",ui->navigation_port_edit->text());
    sensor_param.setValue("navigation_butr",ui->navigation_butr_edit->text());

    sensor_param.setValue("mm_fl_state_id",ui->mm_fl_state_id_edit->text());
    sensor_param.setValue("mm_fl_table_id",ui->mm_fl_table_id_edit->text());

    sensor_param.setValue("mm_fm_state_id",ui->mm_fm_state_id_edit->text());
    sensor_param.setValue("mm_fm_table_id",ui->mm_fm_table_id_edit->text());

    sensor_param.setValue("mm_fr_state_id",ui->mm_fr_state_id_edit->text());
    sensor_param.setValue("mm_fr_table_id",ui->mm_fr_table_id_edit->text());

    sensor_param.setValue("mm_bl_state_id",ui->mm_bl_state_id_edit->text());
    sensor_param.setValue("mm_bl_table_id",ui->mm_bl_table_id_edit->text());
    
    sensor_param.setValue("mm_br_state_id",ui->mm_br_state_id_edit->text());
    sensor_param.setValue("mm_br_table_id",ui->mm_br_table_id_edit->text());


    QSettings video_topic_setting("video_topic","avos_monitor");
    QStringList name_data;
    QStringList topic_data;
    QStringList format_data;
    name_data.append(ui->video0_name_set->text());
    name_data.append(ui->video0_name_set_2->text());
    name_data.append(ui->video0_name_set_3->text());
    name_data.append(ui->video0_name_set_4->text());
    name_data.append(ui->video0_name_set_5->text());

    topic_data.append(ui->video0_topic_set->text());
    topic_data.append(ui->video0_topic_set_2->text());
    topic_data.append(ui->video0_topic_set_3->text());
    topic_data.append(ui->video0_topic_set_4->text());
    topic_data.append(ui->video0_topic_set_5->text());

    video_topic_setting.setValue("names",name_data);
    video_topic_setting.setValue("topics",topic_data);
    QMessageBox::critical(NULL, "保存成功！", "保存成功，部分功能需重启后生效！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    this->close();
}

void Settings::slot_cancel_btn_click()
{
    this->close();
}

void Settings::slot_apply_param_click()
{
    if(OnSendAutodriverParams())
    {
        QMessageBox::information(this, "信息提示", "自动驾驶参数设置成功！");        
    }
}

void Settings::slot_reset_param_click()
{
    ui->user_max_speed_edit->setText("15");
    ui->user_follow_dis_edit->setText("10");
    ui->user_follow_time_edit->setText("2");
    ui->user_collision_buff_edit->setText("0.3");
    ui->user_stop_dis_edit->setText("5");
    ui->user_pullover_dis_edit->setText("40");
    ui->enable_avoid_rbtn->setChecked(true); 
}

void Settings::on_update_avos_version_btn_clicked()
{
    g_common_info_.business_status.avos_version = ui->vehicle_version_edit->text().toStdString();
}

void Settings::on_modify_vehicle_vin_btn_clicked()
{
    g_common_info_.business_status.vehicle_vin = ui->vehicle_vin_edit->text().toStdString();
}

bool Settings::OnSaveAutodriverParams(QString file_path)//保存参数到配置文件
{
    return false;
}

// float32 user_max_speed//速度限制，８－１５
// float32 user_follow_time//跟车时间，单位为秒；自车与前车可能碰撞的时间，最小２秒
// float32 user_follow_dis//跟车距离，自车到前车的距离限制－以上最小碰撞距离
// float32 user_stop_dis//跟停距离，如果前车停车，自车会跟停，与前车的距限制
// float32 user_collision_buff//碰撞检测，就是避障距离，最小的横向偏差
// float32 user_pullover_dis//距离终点多少米开始执行靠边车，最小４０
// bool enable_avoid//是否避障，ture:可以避障，false:不避障，遇障碍会停车，避障消失会自动启动
bool Settings::OnSendAutodriverParams()//配置文件下发到车端固定位置
{
    planning_api_msgs::BehaviorLimitCmd input;
    planning_api_msgs::PlanningResult output;

    input.user_max_speed = ui->user_max_speed_edit->text().toFloat();;
    input.user_follow_time = ui->user_follow_time_edit->text().toFloat();;
    input.user_follow_dis = ui->user_follow_dis_edit->text().toFloat();;
    input.user_stop_dis = ui->user_stop_dis_edit->text().toFloat();;
    input.user_collision_buff = ui->user_collision_buff_edit->text().toFloat();;
    input.user_pullover_dis = ui->user_pullover_dis_edit->text().toFloat();;
    input.enable_avoid = ui->enable_avoid_rbtn->isChecked();


    int value = service_client.CallFunc<planning_api_msgs::BehaviorLimitCmd, planning_api_msgs::PlanningResult>(1, 2, input, output );

    if (value == -1)
    {
        QMessageBox::information(this, "信息提示", "调用设置服务失败，请查检连接！");
        return false;
    }
    else
    {
        if (output.result == 0)
        {
            return true;
        }
        else if (output.result == -1)
        {
            QMessageBox::information(this, "信息提示", QString("参数设置失败(%1)!").arg(QString::fromStdString(output.addition_message)));
            return false;
        }
    }

    return false;
}