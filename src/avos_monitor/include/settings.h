#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QMessageBox>
#include "data_pool.h"
#include "avos_service_bridge.h"
#include "planning_api_msgs/BehaviorLimitCmd.h"
#include "planning_api_msgs/PlanningResult.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
public slots:
    void slot_ok_btn_click();
    void slot_cancel_btn_click();
    void slot_apply_param_click();
    void slot_reset_param_click();
    void on_update_avos_version_btn_clicked();
    void on_modify_vehicle_vin_btn_clicked();
private:
    Ui::Settings *ui;
    avos::service::ServiceClientBridge service_client;

private:
	bool OnSaveAutodriverParams(QString file_path);//保存参数到配置文件
	bool OnSendAutodriverParams();//配置文件下发到车端固定位置
};

#endif // SETTINGS_H
