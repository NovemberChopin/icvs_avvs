#ifndef TASK_MANAGE_DLG_H
#define TASK_MANAGE_DLG_H

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStylePainter>
#include <QDebug>
#include "data_pool.h"
#include "avos_service_bridge.h"
#include "map/vectormap/include/geotool.h"
#include "business_platform_api_msgs/AutomaticTaskInput.h"
#include "business_platform_api_msgs/AvosServiceReply.h"
#include "business_platform_api_msgs/AutomaticTaskStatus.h"


namespace Ui {
class TaskManageDlg;
}

class TaskManageDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TaskManageDlg(QWidget *parent = 0);
    ~TaskManageDlg();

    void InitTableView();

protected:
	bool eventFilter(QObject* o, QEvent* e);
    void closeEvent(QCloseEvent *event);

private Q_SLOTS:
    void on_task_start_btn_clicked();

    void on_task_pause_btn_clicked();

    void on_task_stop_btn_clicked();

    void on_add_point_btn_clicked();

    void on_remove_point_btn_clicked();

    void on_target_attribute_combox_currentIndexChanged(const QString &arg1);

private:
	bool CheckPointExist(const TaskPoint &point);

	bool RemovePointFromVector(const TaskPoint &point);
	//cmd:0：default;1：准备自动驾驶; 2：开始任务; 3：结束任务; 4：暂停任务; 5：继续任务，msg为执行失败时返回的信息
	bool TaskExecute(int cmd, QString & msg);
	//msg为执行失败时返回的信息，int返回值：-1：任务执行失败； 0：任务执行成功； 1：任务运行中；2：任务暂停中；3：任务准备失败
	int  GetTaskStatus(QString & msg);

	void CallbackNavPos(const geometry_msgs::PoseStamped::ConstPtr &msg);

    bool CheckPoint();
private:
    Ui::TaskManageDlg *ui;

    QStandardItemModel *point_model_ = NULL;

    std::vector<TaskPoint> vfunction_point_;

    avos::service::ServiceClientBridge service_client;

    int task_status_ = 0;

    ros::Subscriber sub_nav_pose_;

	avos::vectormap::GeoTool geo_tool_;
};

#endif // TASK_MANAGE_DLG_H
