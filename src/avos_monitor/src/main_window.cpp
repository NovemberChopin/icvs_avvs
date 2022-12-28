/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui->
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "main_window.hpp"


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace avos_monitor {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent),
    ui(new Ui::MainWindowDesign),
    qnode(argc,argv)
{
    ui->setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    //QObject::connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application

    initUis();
    initData();
    //读取配置文件
    ReadSettings();
    // setWindowIcon(QIcon(":/images/robot.png"));
    ui->tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).
    //QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    
    /*********************
    ** Logging
    **********************/
    ui->view_logging->setModel(qnode.loggingModel());
    
    addtopic_form = new AddTopics();
    //绑定添加rviz话题信号
    connect(addtopic_form, SIGNAL(Topic_choose(QTreeWidgetItem *, QString)), this, SLOT(slot_choose_topic(QTreeWidgetItem *, QString)));
    
    /*********************
    ** 自动连接master
    **********************/
    if ( ui->checkbox_remember_settings->isChecked() ) {
        on_button_connect_clicked();
    }
    //链接connect
    connections();
}

//析构函数
MainWindow::~MainWindow()
{
    if( base_cmd)
    {
        delete base_cmd;
        base_cmd = nullptr;
    }
    if(map_rviz_)
    {
        delete map_rviz_;
        map_rviz_ = nullptr;
    }
}

//初始化UI
void MainWindow::initUis()
{
    ui->groupBox_3->setEnabled(false);
    ui->groupBox_4->setEnabled(false);

    ui->tabWidget->addTab(new SystemToolbar(),tr("车辆状态"));//在后面添加选项卡    
    ui->tab_manager->addTab(new VehicleInformationWidget(), "整车信息");
    // ui->tab_manager->addTab(new RosTopicWidget(), "ROS话题");

    ui->vehicle_info_vlayout->addWidget(new RosTopicWidget());

    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);

    ui->tab_manager->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);

    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);

    ui->tab_manager->setTabEnabled(1, false);
    ui->tab_manager->setTabEnabled(2, false);
    ui->tab_manager->setTabEnabled(3, false);
    
    ui->pushButton_remove_topic->setEnabled(false);
    ui->pushButton_rename_topic->setEnabled(false);

    //qucik treewidget
    ui->treeWidget_quick_cmd->setHeaderLabels(QStringList()<<"key"<<"values");
    ui->treeWidget_quick_cmd->setHeaderHidden(true);
    
    ui->button_disconnect->setEnabled(false);

    switch_control_all_btn = new SwitchButton();    
    switch_control_all_btn->setText("全部关闭", "全部开启");
    ui->gridLayout_avos_ctrl->addWidget(switch_control_all_btn, 0, 0);

    switch_gps_btn = new SwitchButton();    
    switch_gps_btn->setText("组合导航 关", "组合导航 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_gps_btn, 1, 0);

    switch_lidar_radar_btn = new SwitchButton();
    switch_lidar_radar_btn->setText("激光雷达 关", "激光雷达 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_lidar_radar_btn, 1, 1);

    switch_mill_radar_btn = new SwitchButton();
    switch_mill_radar_btn->setText("毫米波 关", "毫米波 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_mill_radar_btn, 2, 0);

    switch_camera_btn = new SwitchButton();
    switch_camera_btn->setText("摄像头 关", "摄像头 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_camera_btn, 2, 1);

    switch_perception_btn = new SwitchButton();
    switch_perception_btn->setText("感知 关", "感知 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_perception_btn, 3, 0);

    switch_localization_btn = new SwitchButton();
    switch_localization_btn->setText("定位 关", "定位 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_localization_btn, 3, 1);

    switch_control_btn = new SwitchButton();
    switch_control_btn->setText("控制 关", "控制 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_control_btn, 4, 0);

    switch_planning_btn = new SwitchButton();
    switch_planning_btn->setText("规划 关", "规划 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_planning_btn, 4, 1);

    switch_canbus_btn = new SwitchButton();
    switch_canbus_btn->setText("Canbus 关", "Canbus 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_canbus_btn, 5, 0);

    switch_task_btn = new SwitchButton();
    switch_task_btn->setText("业务中枢 关", "业务中枢 开");
    ui->gridLayout_avos_ctrl->addWidget(switch_task_btn, 5, 1);

}

void MainWindow::initData()
{
    m_mapRvizDisplays.insert("Axes", RVIZ_DISPLAY_AXES);
    m_mapRvizDisplays.insert("Camera", RVIZ_DISPLAY_CAMERA);
    m_mapRvizDisplays.insert("DepthCloud", RVIZ_DISPLAY_DEPTHCLOUD);
    m_mapRvizDisplays.insert("Effort", RVIZ_DISPLAY_EFFORT);
    m_mapRvizDisplays.insert("FluidPressure", RVIZ_DISPLAY_FLUIDPRESSURE);
    m_mapRvizDisplays.insert("Grid", RVIZ_DISPLAY_GRID);
    m_mapRvizDisplays.insert("GridCells", RVIZ_DISPLAY_GRIDCELLS);
    m_mapRvizDisplays.insert("Group", RVIZ_DISPLAY_GROUP);
    m_mapRvizDisplays.insert("Illuminance", RVIZ_DISPLAY_ILLUMINANCE);
    m_mapRvizDisplays.insert("Image", RVIZ_DISPLAY_IMAGE);
    m_mapRvizDisplays.insert("InterativerMarker", RVIZ_DISPLAY_INTERATIVEMARKER);
    m_mapRvizDisplays.insert("LaserScan", RVIZ_DISPLAY_LASERSCAN);
    m_mapRvizDisplays.insert("Map", RVIZ_DISPLAY_MAP);
    m_mapRvizDisplays.insert("Marker", RVIZ_DISPLAY_MARKER);
    m_mapRvizDisplays.insert("MarkerArray", RVIZ_DISPLAY_MARKERARRAY);
    m_mapRvizDisplays.insert("Odometry", RVIZ_DISPLAY_ODOMETRY);
    m_mapRvizDisplays.insert("Path", RVIZ_DISPLAY_PATH);
    m_mapRvizDisplays.insert("PointCloud", RVIZ_DISPLAY_POINTCLOUD);
    m_mapRvizDisplays.insert("PointCloud2", RVIZ_DISPLAY_POINTCLOUD2);
    m_mapRvizDisplays.insert("PointStamped", RVIZ_DISPLAY_POINTSTAMPED);
    m_mapRvizDisplays.insert("Polygon", RVIZ_DISPLAY_POLYGON);
    m_mapRvizDisplays.insert("Pose", RVIZ_DISPLAY_POSE);
    m_mapRvizDisplays.insert("PoseArray", RVIZ_DISPLAY_POSEARRAY);
    m_mapRvizDisplays.insert("PoseWithCovariance", RVIZ_DISPLAY_POSEWITHCOVARIANCE);
    m_mapRvizDisplays.insert("Range", RVIZ_DISPLAY_RANGE);
    m_mapRvizDisplays.insert("RelativeHumidity", RVIZ_DISPLAY_RELATIVEHUMIDITY);
    m_mapRvizDisplays.insert("RobotModel", RVIZ_DISPLAY_ROBOTMODEL);
    m_mapRvizDisplays.insert("TF", RVIZ_DISPLAY_TF);
    m_mapRvizDisplays.insert("Temperature", RVIZ_DISPLAY_TEMPERATURE);
    m_mapRvizDisplays.insert("WrenchStamped", RVIZ_DISPLAY_WRENCHSTAMPED);

    g_common_info_.business_status.vehicle_vin = getParamString("vehicle_setting","vehicle_vin").toStdString();
    g_common_info_.business_status.avos_version = getParamString("vehicle_setting","avos_version").toStdString();
}

void MainWindow::load_rviz_params()
{
    if (!rviz_params_loaded)
    {
        QString path = QString::fromStdString(ros::package::getPath("avos_monitor")) + "/config/avvs.yaml";
        map_rviz_->ReadDisplaySet(path);
        rviz_params_loaded = true;
    }
}
//订阅video话题
void MainWindow::initVideos()
{
   QSettings video_topic_setting("video_topic","avos_monitor");
   QStringList names=video_topic_setting.value("names").toStringList();
   QStringList topics=video_topic_setting.value("topics").toStringList();
   if(names.size()==5)
   {
       ui->label_v_name0->setText(names[0]);
       ui->label_v_name1->setText(names[1]);
       ui->label_v_name2->setText(names[2]);
       ui->label_v_name3->setText(names[3]);
       ui->label_v_name4->setText(names[4]);
   }
   if(topics.size()==5)
   {
       if(topics[0]!="")
        qnode.Sub_Image(topics[0],0);
       if(topics[1]!="")
        qnode.Sub_Image(topics[1],1);
       if(topics[2]!="")
        qnode.Sub_Image(topics[2],2);
       if(topics[3]!="")
        qnode.Sub_Image(topics[3],3);
       if(topics[4]!="")
        qnode.Sub_Image(topics[4],4);
   }
   //链接槽函数
   connect(&qnode,SIGNAL(Show_image(int,QImage)),this,SLOT(slot_show_image(int,QImage)));
}

void MainWindow::slot_show_image(int frame_id, QImage image)
{
    switch (frame_id)
    {
    case 0:
        ui->label_video0->setPixmap(QPixmap::fromImage(image).scaled(ui->label_video0->width(),ui->label_video0->height()));
        break;
    case 1:
        ui->label_video1->setPixmap(QPixmap::fromImage(image).scaled(ui->label_video1->width(),ui->label_video1->height()));
        break;
    case 2:
        ui->label_video2->setPixmap(QPixmap::fromImage(image).scaled(ui->label_video2->width(),ui->label_video2->height()));
        break;
    case 3:
        ui->label_video3->setPixmap(QPixmap::fromImage(image).scaled(ui->label_video3->width(),ui->label_video3->height()));
        break;
    case 4:
        ui->label_video4->setPixmap(QPixmap::fromImage(image).scaled(ui->label_video4->width(),ui->label_video4->height()));
        break;
    default:
        break;
    }
}


void MainWindow::initRviz()
{
    ui->label_rvizShow->hide();
    map_rviz_=new QRviz(ui->verticalLayout_build_map,"qrviz");
    connect(map_rviz_, &QRviz::ReturnModelSignal, this, &MainWindow::RvizGetModel);
    map_rviz_->GetDisplayTreeModel();
    QMap<QString, QVariant> namevalue;
    namevalue.insert("Line Style", "Billboards");
    namevalue.insert("Color", QColor(160, 160, 160));
    namevalue.insert("Plane Cell Count", 10);
    map_rviz_->DisplayInit(RVIZ_DISPLAY_GRID, "Grid", true, namevalue);
    map_rviz_->SetRvizView(TOP_DOWN_OR_THO);
    
    ui->pushButton_add_topic->setEnabled(true);
    ui->pushButton_rvizReadDisplaySet->setEnabled(true);
    ui->pushButton_rvizSaveDisplaySet->setEnabled(true);
}

void MainWindow::RvizGetModel(QAbstractItemModel *model)
{
    m_modelRvizDisplay = model;
    ui->treeView_rvizDisplayTree->setModel(model);
}

void MainWindow::connections()
{
    QObject::connect(&qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(slot_rosShutdown()));
    QObject::connect(&qnode, SIGNAL(Master_shutdown()), this, SLOT(slot_rosShutdown()));
    //绑定快捷按钮相关函数
    connect(ui->quick_cmd_add_btn,SIGNAL(clicked()),this,SLOT(quick_cmd_add()));
    connect(ui->quick_cmd_remove_btn,SIGNAL(clicked()),this,SLOT(quick_cmd_remove()));
    //设置界面
    connect(ui->action_2,SIGNAL(triggered(bool)),this,SLOT(slot_setting_frame()));

    // connect(ui->action_reboot_system,SIGNAL(triggered(bool)),this,SLOT(on_action_reboot_system_clicked()));
    //设置2D Pose
    connect(ui->set_pos_btn,SIGNAL(clicked()),this,SLOT(slot_set_2D_Pos()));
    //设置2D goal
    connect(ui->set_goal_btn,SIGNAL(clicked()),this,SLOT(slot_set_2D_Goal()));
    //设置MoveCamera
    connect(ui->move_camera_btn,SIGNAL(clicked()),this,SLOT(slot_move_camera_btn()));
    //设置Select
    connect(ui->set_select_btn,SIGNAL(clicked()),this,SLOT(slot_set_select()));
    //左工具栏tab索引改变
    connect(ui->tab_manager,SIGNAL(currentChanged(int)),this,SLOT(slot_tab_manage_currentChanged(int)));
    //右工具栏索引改变
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(slot_tab_Widget_currentChanged(int)));
    //刷新话题列表
    connect(ui->refreash_topic_btn,SIGNAL(clicked()),this,SLOT(refreashTopicList()));

    connect(switch_gps_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_gps(bool)));
    connect(switch_lidar_radar_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_lidar(bool)));
    connect(switch_mill_radar_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_mill(bool)));
    connect(switch_camera_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_camera(bool)));
    connect(switch_perception_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_perception(bool)));
    connect(switch_localization_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_localization(bool)));
    connect(switch_control_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_control(bool)));
    connect(switch_planning_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_planning(bool)));
    connect(switch_canbus_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_canbus(bool)));
    connect(switch_task_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_task(bool)));
    connect(switch_control_all_btn,SIGNAL(checkedChanged(bool)),this,SLOT(on_switch_button_control_all(bool)));

    connect(ui->autodriver_mode_rbtn,SIGNAL(clicked(bool)),this,SLOT(on_radio_button_automode_changed(bool)));
    connect(ui->idle_mode_rbtn,SIGNAL(clicked(bool)),this,SLOT(on_radio_button_automode_changed(bool)));
    connect(ui->maule_mode_rbtn,SIGNAL(clicked(bool)),this,SLOT(on_radio_button_automode_changed(bool)));
}
//设置界面
void MainWindow::slot_setting_frame()
{
    if(set != nullptr)
    {
        delete set;
        set=new Settings();
        set->setWindowModality(Qt::ApplicationModal);
        set->show();
    }
    else
    {
        set=new Settings();
        set->setWindowModality(Qt::ApplicationModal);
        set->show();
    }
    //绑定set确认按钮点击事件
}

//设置导航当前位置按钮的槽函数
void MainWindow::slot_set_2D_Pos()
{
    map_rviz_->Set_Pos();
    // ui->label_map_msg->setText("请在地图中选择机器人的初始位置");
}
void MainWindow::set2DPos()
{
    map_rviz_->Set_Pos();
}
//设置导航目标位置按钮的槽函数
void MainWindow::slot_set_2D_Goal()
{
    map_rviz_->Set_Goal();
//  ui->label_map_msg->setText("请在地图中选择机器人导航的目标位置");
}
void MainWindow::slot_move_camera_btn()
{
    map_rviz_->Set_MoveCamera();
    qDebug()<<"move camera";
}
void MainWindow::slot_set_select()
{
    map_rviz_->Set_Select();
}

void MainWindow::on_set_rviz_view_btn_clicked()
{
    if (rviz_view_changed_)
    {
        map_rviz_->SetRvizView(TOP_DOWN_OR_THO);
    }
    else
    {
        map_rviz_->SetRvizView(THIRD_PERSON_FOLLOWER);        
    }
    rviz_view_changed_ = !rviz_view_changed_;
}

void MainWindow::on_return_to_zero_btn_clicked()
{
    map_rviz_->ReturnToZero();
}

//选中要添加的话题的槽函数
void MainWindow::slot_choose_topic(QTreeWidgetItem *choose, QString name)
{
    QString ClassID = choose->text(0);
    // 检查重名
    name = JudgeDisplayNewName(name);
    
    qDebug() << "choose topic ClassID:" << ClassID << ", name:" << name;
    QMap<QString, QVariant> namevalue;
    namevalue.clear();
    map_rviz_->DisplayInit(m_mapRvizDisplays[ClassID], name, true, namevalue);
}

///
/// \brief 检查重名
/// \param name
/// \return 
///
QString MainWindow::JudgeDisplayNewName(QString name)
{
    if (m_modelRvizDisplay != nullptr)
    {
        bool same = true;
        while (same)
        {
            same = false;
            for (int i = 0; i < m_modelRvizDisplay->rowCount(); i++)
            {
                //m_sRvizDisplayChooseName = index.data().value<QString>();
                if (m_modelRvizDisplay->index(i, 0).data().value<QString>() == name)
                {
                    if (name.indexOf("_") != -1)
                    {
                        int num = name.section("_", -1, -1).toInt();
                        name = name.left(name.length() - name.section("_", -1, -1).length() - 1);
                        if (num <= 1)
                        {
                            num = 2;
                        }
                        else
                        {
                            num++;
                        }
                        name = name + "_" + QString::number(num);
                    }
                    else {
                      name = name + "_2";
                    }
                    same = true;
                    break;
                }
            }
        }
    }
    return name;
}

//左工具栏索引改变
void MainWindow::slot_tab_manage_currentChanged(int index)
{
    switch (index) {
    case 0:
        break;
    case 1:
        // ui->tabWidget->setCurrentIndex(1);
        break;
    case 2:
        break;

    }
}
//右工具栏索引改变
void MainWindow::slot_tab_Widget_currentChanged(int index)
{
    switch (index) {
    case 0:
        // ui->tab_manager->setCurrentIndex(3);
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        ui->tab_manager->setCurrentIndex(0);
        break;
    default:
        break;

    }
}


//快捷指令删除按钮
void MainWindow::quick_cmd_remove()
{
    QTreeWidgetItem *curr=ui->treeWidget_quick_cmd->currentItem();
    //没有选择节点
    if(curr == nullptr) return;
    //获取父节点
    QTreeWidgetItem* parent=curr->parent();
    //如果当前节点就为父节点
    if(parent == nullptr)
    {
        ui->treeWidget_quick_cmd->takeTopLevelItem(ui->treeWidget_quick_cmd->indexOfTopLevelItem(curr));
        delete curr;
    }
    else{
        ui->treeWidget_quick_cmd->takeTopLevelItem(ui->treeWidget_quick_cmd->indexOfTopLevelItem(parent));
        delete parent;
    }


}
//快捷指令添加按钮
void MainWindow::quick_cmd_add()
{
    QWidget *w=new QWidget;
    //阻塞其他窗体
    w->setWindowModality(Qt::ApplicationModal);
    QLabel *name=new QLabel;
    name->setText("名称:");
    QLabel *content=new QLabel;
    content->setText("脚本:");
    QLineEdit *name_val=new QLineEdit;
    QTextEdit *shell_val=new QTextEdit;
    QPushButton *ok_btn=new QPushButton;
    ok_btn->setText("ok");
    ok_btn->setIcon(QIcon("://images/ok.png"));
    QPushButton *cancel_btn=new QPushButton;
    cancel_btn->setText("cancel");
    cancel_btn->setIcon(QIcon("://images/false.png"));
    QHBoxLayout *lay1=new QHBoxLayout;
    lay1->addWidget(name);
    lay1->addWidget(name_val);
    QHBoxLayout *lay2=new QHBoxLayout;
    lay2->addWidget(content);
    lay2->addWidget(shell_val);
    QHBoxLayout *lay3=new QHBoxLayout;
    lay3->addWidget(ok_btn);
    lay3->addWidget(cancel_btn);
    QVBoxLayout *v1=new QVBoxLayout;
    v1->addLayout(lay1);
    v1->addLayout(lay2);
    v1->addLayout(lay3);

    w->setLayout(v1);
    w->show();

    connect(ok_btn,&QPushButton::clicked,[this,w,name_val,shell_val]
    {
        this->add_quick_cmd(name_val->text(),shell_val->toPlainText());
        w->close();
    });
}
//向treeWidget添加快捷指令
void MainWindow::add_quick_cmd(QString name,QString val)
{
    if(name=="") return;
    QTreeWidgetItem *head=new QTreeWidgetItem(QStringList()<<name);
    this->ui->treeWidget_quick_cmd->addTopLevelItem(head);
    QCheckBox *check=new QCheckBox;
    //记录父子关系
    this->widget_to_parentItem_map[check]=head;
    //连接checkbox选中的槽函数
    connect(check,SIGNAL(stateChanged(int)),this,SLOT(quick_cmds_check_change(int)));
    this->ui->treeWidget_quick_cmd->setItemWidget(head,1,check);
    QTreeWidgetItem *shell_content=new QTreeWidgetItem(QStringList()<<"shell");
    QTextEdit *shell_val=new QTextEdit;
    shell_val->setMaximumWidth(150);
    shell_val->setMaximumHeight(40);
    head->addChild(shell_content);
    shell_val->setText(val);
    this->ui->treeWidget_quick_cmd->setItemWidget(shell_content,1,shell_val);
}
//快捷指令按钮处理的函数
void MainWindow::quick_cmds_check_change(int state)
{

    QCheckBox* check = qobject_cast<QCheckBox*>(sender());
    QTreeWidgetItem *parent=widget_to_parentItem_map[check];
    QString bash = static_cast<QTextEdit *>(ui->treeWidget_quick_cmd->itemWidget(parent->child(0),1))->toPlainText();
    bool is_checked = state>1 ? true : false;
    if(is_checked)
    {
        quick_cmd = new QProcess;
        quick_cmd->start("bash");
        qDebug() << bash;
        quick_cmd->write(bash.toLocal8Bit()+'\n');
        connect(quick_cmd,SIGNAL(readyReadStandardOutput()),this,SLOT(cmd_output()));
        connect(quick_cmd,SIGNAL(readyReadStandardError()),this,SLOT(cmd_error_output()));
    }
    else{


    }

}
//执行一些命令的回显
void MainWindow::cmd_output()
{

    ui->cmd_output->append(quick_cmd->readAllStandardOutput());
}
//执行一些命令的错误回显
void MainWindow::cmd_error_output()
{
    ui->cmd_output->append("<font color=\"#FF0000\">"+quick_cmd->readAllStandardError()+"</font> ");
}



/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

void MainWindow::showNoMasterMessage() {
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}

void MainWindow::inform(QString strdata)
{
    QMessageBox m_r;
    m_r.setWindowTitle("提示");
    m_r.setText(strdata);
    m_r.exec();
}
bool MainWindow::AskInform(QString strdata)
{
    QMessageBox m_r;

    m_r.setWindowTitle("提示");
    m_r.setText(strdata);
    m_r.addButton(tr("确认"), QMessageBox::ActionRole);
    m_r.addButton(tr("取消"), QMessageBox::ActionRole);

    int isok = m_r.exec();
    if (isok == 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */

void MainWindow::initTopicList()
{
    ui->topic_listWidget->clear();
    ui->topic_listWidget->addItem(QString("%1   (%2)").arg("Name","Type"));
    QMap<QString,QString> topic_list= qnode.get_topic_list();
    for(QMap<QString,QString>::iterator iter=topic_list.begin();iter!=topic_list.end();iter++)
    {
       ui->topic_listWidget->addItem(QString("%1   (%2)").arg(iter.key(),iter.value()));
    }
}


QString MainWindow::getParamString(QString section, QString name)
{
    QSettings param_get(section,"avos_monitor");    
    return param_get.value(name,"").toString();
}

void MainWindow::refreashTopicList()
{
    initTopicList();
}
//当ros与master的连接断开时
void MainWindow::slot_rosShutdown()
{
    ui->label_robot_staue_img->setPixmap(QPixmap::fromImage(QImage("://images/offline.png")));
    ui->label_statue_text->setStyleSheet("color:red;");
    ui->label_statue_text->setText("离线");
    ui->button_connect->setEnabled(true);
    ui->line_edit_master->setReadOnly(false);
    ui->line_edit_host->setReadOnly(false);
}

void MainWindow::on_checkbox_use_environment_stateChanged(int state) {
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui->line_edit_master->setEnabled(enabled);
	ui->line_edit_host->setEnabled(enabled);
}

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/

/**
 * This function is signalled by the underlying model. When the model changes,
 * this will drop the cursor down to the last line in the QListview to ensure
 * the user can always see the latest log message.
 */
void MainWindow::updateLoggingView() {
        ui->view_logging->scrollToBottom();
}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void MainWindow::on_actionAbout_triggered() {
    //QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "avos_monitor");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.102:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.118")).toString();
    //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui->line_edit_master->setText(master_url);
    ui->line_edit_host->setText(host_url);
    bool remember = settings.value("remember_settings", false).toBool();
    ui->checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui->checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui->line_edit_master->setEnabled(false);
    	ui->line_edit_host->setEnabled(false);
    }

    //读取快捷指令的setting
    QSettings quick_setting("quick_setting","avos_monitor");
    QStringList ch_key=quick_setting.childKeys();
    for(auto c:ch_key)
    {
        add_quick_cmd(c,quick_setting.value(c,QString("")).toString());
    }

}

void MainWindow::WriteSettings() 
{
    QSettings settings("Qt-Ros Package", "avos_monitor");
    settings.setValue("master_url",ui->line_edit_master->text());
    settings.setValue("host_url",ui->line_edit_host->text());
    settings.setValue("use_environment_variables",QVariant(ui->checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    //settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui->checkbox_remember_settings->isChecked()));

    //存下快捷指令的setting
    QSettings quick_setting("quick_setting","avos_monitor");
    quick_setting.clear();
    for(int i=0;i<ui->treeWidget_quick_cmd->topLevelItemCount();i++)
    {
        QTreeWidgetItem *top=ui->treeWidget_quick_cmd->topLevelItem(i);
        QTextEdit *cmd_val= static_cast<QTextEdit *>(ui->treeWidget_quick_cmd->itemWidget(top->child(0),1));
        quick_setting.setValue(top->text(0),cmd_val->toPlainText());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    WriteSettings();
    QMainWindow::closeEvent(event);    
    killTimer(m_nTimerId);
}


void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerId)
    {
        UpdateStatus(); 
    }
}

///
/// \brief 增加topic
///
void avos_monitor::MainWindow::on_pushButton_add_topic_clicked()
{
    addtopic_form->show();
}

///
/// \brief 删除topic
///
void avos_monitor::MainWindow::on_pushButton_remove_topic_clicked()
{
    if (ui->treeView_rvizDisplayTree->currentIndex().row() >= 0)
    {
        m_sRvizDisplayChooseName_ = ui->treeView_rvizDisplayTree->currentIndex().data().value<QString>();
        map_rviz_->RemoveDisplay(m_sRvizDisplayChooseName_);
        if (ui->treeView_rvizDisplayTree->currentIndex().row() >= 0)
        {
            on_treeView_rvizDisplayTree_clicked(ui->treeView_rvizDisplayTree->currentIndex());
        }
        else
        {
            m_sRvizDisplayChooseName_.clear();
        }
    }
    else
    {
        inform("请选择Display后再执行此操作");
    }
}

///
/// \brief 重命名topic
///
void avos_monitor::MainWindow::on_pushButton_rename_topic_clicked()
{
    if (ui->treeView_rvizDisplayTree->currentIndex().row() < 0)
    {
        inform("请选择Display后再执行此操作");
        return ;
    }
    QString dlgTitle = "重命名";
    QString txtlabel = "请输入名字：";
    QString defaultInupt = m_sRvizDisplayChooseName_;
    QLineEdit::EchoMode echoMode = QLineEdit::Normal;
    bool ok = false;
    QString newname = QInputDialog::getText(this, dlgTitle, txtlabel, echoMode, defaultInupt, &ok);
    if (ok && !newname.isEmpty())
    {
        if (newname != defaultInupt)
        {
            QString nosamename = JudgeDisplayNewName(newname);
            map_rviz_->RenameDisplay(defaultInupt, nosamename);
            m_sRvizDisplayChooseName_ = nosamename;
            if (nosamename != newname)
            {
                inform("命名重复！命名已自动更改为" + nosamename);
            }
        }
    }
    else if (ok)
    {
        inform("输入内容为空，重命名失败");
    }
}

void avos_monitor::MainWindow::on_treeView_rvizDisplayTree_clicked(const QModelIndex &index)
{
    m_sRvizDisplayChooseName_ = index.data().value<QString>();
    if (index.parent().row() == -1)   // Display 的根节点
    {
        if (index.row() > 1)
        {
            ui->pushButton_remove_topic->setEnabled(true);
            ui->pushButton_rename_topic->setEnabled(true);
        }
        else
        {
            ui->pushButton_remove_topic->setEnabled(false);
            ui->pushButton_rename_topic->setEnabled(false);
        }
    }
    else
    {
        ui->pushButton_remove_topic->setEnabled(false);
        ui->pushButton_rename_topic->setEnabled(false);
    }
}

/// \brief 连接ROS
void MainWindow::on_button_connect_clicked()
{
    //如果使用环境变量
    if ( ui->checkbox_use_environment->isChecked() )
    {
        if ( !qnode.init() )
        {
            //showNoMasterMessage();
            QMessageBox::warning(nullptr, "失败", "连接车辆失败,自动驾驶系统未启动或网络未连接！", QMessageBox::Yes, QMessageBox::Yes);
            ui->label_robot_staue_img->setPixmap(QPixmap::fromImage(QImage("://images/offline.png")));
            ui->label_statue_text->setStyleSheet("color:red;");
            ui->label_statue_text->setText("离线");

            ui->tabWidget->setTabEnabled(1, false);
            ui->tabWidget->setTabEnabled(2, false);
            ui->tabWidget->setTabEnabled(3, false);

            ui->tab_manager->setTabEnabled(1, false);
            ui->tab_manager->setTabEnabled(2, false);
            ui->tab_manager->setTabEnabled(3, false);

            ui->groupBox_3->setEnabled(false);
            ui->groupBox_4->setEnabled(false);
            return ;
        }
    }
    //如果不使用环境变量
    else
    {
        if ( !qnode.init(ui->line_edit_master->text().toStdString(), ui->line_edit_host->text().toStdString()) )
        {
            QMessageBox::warning(nullptr, "失败", "连接车辆失败,自动驾驶系统未启动或网络未连接！", QMessageBox::Yes, QMessageBox::Yes);
            ui->label_robot_staue_img->setPixmap(QPixmap::fromImage(QImage("://images/offline.png")));
            ui->label_statue_text->setStyleSheet("color:red;");
            ui->label_statue_text->setText("离线");

            ui->tabWidget->setTabEnabled(1, false);
            ui->tabWidget->setTabEnabled(2, false);
            ui->tabWidget->setTabEnabled(3, false);

            ui->tab_manager->setTabEnabled(1, false);
            ui->tab_manager->setTabEnabled(2, false);
            ui->tab_manager->setTabEnabled(3, false);

            ui->groupBox_3->setEnabled(false);
            ui->groupBox_4->setEnabled(false);
            //showNoMasterMessage();
            return ;
        }
        else
        {
            ui->line_edit_master->setReadOnly(true);
            ui->line_edit_host->setReadOnly(true);
        }
    }
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->tabWidget->setTabEnabled(3, true);

    ui->tab_manager->setTabEnabled(1, true);
    ui->tab_manager->setTabEnabled(2, true);
    ui->tab_manager->setTabEnabled(3, true);

    ui->groupBox_3->setEnabled(true);
    ui->groupBox_4->setEnabled(true);
    //初始化rviz
    initRviz();    
    ui->button_connect->setEnabled(false);
    ui->label_robot_staue_img->setPixmap(QPixmap::fromImage(QImage("://images/online.png")));
    ui->label_statue_text->setStyleSheet("color:green;");
    ui->label_statue_text->setText("在线");
    ui->button_disconnect->setEnabled(true);
    //显示话题列表
    initTopicList();
    load_rviz_params();    
    m_nTimerId = startTimer(300);
}

//重启计算平台
void MainWindow::on_action_reboot_system_triggered()
{
    std::string shell_path_ = ros::package::getPath("avos_monitor") + "/resources/shell/";
    std::string cmd = "bash " + shell_path_ + "system_reboot.sh " + shell_path_;
    if(system(cmd.c_str()) == 0)
    {
        QMessageBox::warning(this, "信息提示", "计算平台重启失败，请尝试断电重启！");
    } 
    else
    {
        // QMessageBox::information(this, "信息提示", "计算平台正在重启，请耐心等待！");
        QMessageBox box(QMessageBox::Question, "信息提示","计算平台正在重启，请耐心等待！确认后将退出上位机软件！");
        SetMessageBox(box);
        auto ret = box.exec();
        if (ret == QMessageBox::Yes) {
            close();
        } else if (ret == QMessageBox::No) {
        } else {
        }
    }
}

void MainWindow::on_record_bag_btn_clicked()
{    
    QString msg = "";
    if(BagRecordStart(msg))
    {
        QMessageBox::information(this, "信息提示", "Bag包记录指令下发成功！");
    }
    else
    {
        QMessageBox::information(this, "信息提示", msg);        
    }  
}

void MainWindow::on_go_end_point_btn_clicked()
{
    PointGCCS output_pgccs;
    GetPathEndPoint(output_pgccs);
    qDebug() << "output_pgccs xg:" << output_pgccs.xg << "  output_pgccs yg:" << output_pgccs.yg << "  output_pgccs Angle:" << output_pgccs.angle;
}

/// \brief 断开ROS
void MainWindow::on_button_disconnect_clicked()
{
    qnode.disinit();
    map_rviz_->quit();
    delete map_rviz_;
    map_rviz_ = nullptr;
    ui->pushButton_add_topic->setEnabled(false);
    ui->pushButton_remove_topic->setEnabled(false);
    ui->pushButton_rename_topic->setEnabled(false);
    ui->pushButton_rvizReadDisplaySet->setEnabled(false);
    ui->pushButton_rvizSaveDisplaySet->setEnabled(false);

    ui->label_rvizShow->show();

    ui->button_disconnect->setEnabled(false);
    ui->button_connect->setEnabled(true);   

    ui->groupBox_3->setEnabled(false);
    ui->groupBox_4->setEnabled(false);

    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);

    ui->tab_manager->setTabEnabled(1, false);
    ui->tab_manager->setTabEnabled(2, false);
    ui->tab_manager->setTabEnabled(3, false);
    
}

void MainWindow::on_pushButton_rvizReadDisplaySet_clicked()
{
    if (map_rviz_ == nullptr)
    {
        return;
    }
    QString path = QFileDialog::getOpenFileName(this, "导入 Display 配置", "/home/" + getUserName() + "/", "YAML(*.yaml);;ALL(*.*)");
    if (!path.isEmpty())
    {
        map_rviz_->ReadDisplaySet(path);
    }
}

void MainWindow::on_pushButton_rvizSaveDisplaySet_clicked()
{
    if (map_rviz_ == nullptr)
    {
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, "导出 Display 配置", "/home/" + getUserName() + "/", "YAML(*.yaml)");
    
    if (!path.isEmpty())
    {
        if (path.section('/', -1, -1).indexOf('.') < 0)
        {
            path = path + ".yaml";
        }
        map_rviz_->OutDisplaySet(path);
    }
}

void MainWindow::on_save_display_setting_btn_clicked()
{
    if (map_rviz_ == nullptr)
    {
        return;
    }

    QString path = QString::fromStdString(ros::package::getPath("avos_monitor")) + "/config/avvs.yaml";

    if (!path.isEmpty())
    {
        map_rviz_->OutDisplaySet(path);
    }
}

QString MainWindow::getUserName()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString userName = userPath.section("/", -1, -1);
    return userName;
}



void MainWindow::on_switch_button_gps(bool checked)
{
    if (checked)
    {
        if (CheckNavStatus())
        {
            g_common_info_.autodriver_status.navigation = checked;
        }
        else
        {
            g_common_info_.autodriver_status.navigation = false;        
            QMessageBox::information(this, "信息提示", "打开组合导航失败，请查检连接或参数配置！");
            switch_gps_btn->setChecked(false);
        }
    }
    else
    {
        g_common_info_.autodriver_status.navigation = checked;
    }
    CheckAvosStatus();
}

void MainWindow::on_switch_button_lidar(bool checked)
{
    if (checked)
    {
        if (CheckLidarStatus())
        {
            g_common_info_.autodriver_status.lidar_32 = checked;
        }
        else
        {
            g_common_info_.autodriver_status.lidar_32 = false;        
            QMessageBox::information(this, "信息提示", "打开激光雷达失败，请查检连接或参数配置！");
            switch_lidar_radar_btn->setChecked(false);
        }
    }
    else
    {
        g_common_info_.autodriver_status.lidar_32 = checked;
    }    
    CheckAvosStatus();
}   

void MainWindow::on_switch_button_mill(bool checked)
{
    if (checked)
    {
        if (CheckMmStatus())
        {
            g_common_info_.autodriver_status.mm_lidar = checked;
        }
        else
        {
            g_common_info_.autodriver_status.mm_lidar = false;        
            QMessageBox::information(this, "信息提示", "打开毫米雷达失败，请查检连接或参数配置！");
            switch_mill_radar_btn->setChecked(false);
        }
    }
    else
    {
        g_common_info_.autodriver_status.mm_lidar = checked;
    }  
    CheckAvosStatus();
}    

void MainWindow::on_switch_button_camera(bool checked)
{
    if (checked)
    {
        //初始化视频订阅的显示
        initVideos();
    }
    else
    {
        qnode.SubImageShutdown();
    }

    g_common_info_.autodriver_status.camera = checked;
    CheckAvosStatus();
}    

void MainWindow::on_switch_button_perception(bool checked)
{
    g_common_info_.autodriver_status.perception = checked;
    CheckAvosStatus();
}    

void MainWindow::on_switch_button_localization(bool checked)
{
    g_common_info_.autodriver_status.localization = checked;
    CheckAvosStatus();
}    

void MainWindow::on_switch_button_control(bool checked)
{
    g_common_info_.autodriver_status.control = checked;
    CheckAvosStatus();
}    

void MainWindow::on_switch_button_planning(bool checked)
{
    g_common_info_.autodriver_status.planning = checked;
    CheckAvosStatus();
}    

void MainWindow::on_switch_button_canbus(bool checked)
{
    g_common_info_.autodriver_status.canbus = checked;
    CheckAvosStatus();
}    

void MainWindow::on_switch_button_task(bool checked)
{
    g_common_info_.autodriver_status.business = checked;
    CheckAvosStatus();
}   

void MainWindow::on_switch_button_control_all(bool checked)
{
    g_common_info_.autodriver_status.navigation = checked;
    g_common_info_.autodriver_status.lidar_32 = checked;
    g_common_info_.autodriver_status.mm_lidar = checked;
    g_common_info_.autodriver_status.camera = checked;
    g_common_info_.autodriver_status.perception = checked;
    g_common_info_.autodriver_status.localization = checked;
    g_common_info_.autodriver_status.control = checked;
    g_common_info_.autodriver_status.planning = checked;
    g_common_info_.autodriver_status.canbus = checked;
    g_common_info_.autodriver_status.business = checked;

    switch_gps_btn->setChecked(checked);
    switch_lidar_radar_btn->setChecked(checked);
    switch_mill_radar_btn->setChecked(checked);
    switch_camera_btn->setChecked(checked);
    switch_perception_btn->setChecked(checked);
    switch_localization_btn->setChecked(checked);
    switch_control_btn->setChecked(checked);
    switch_planning_btn->setChecked(checked);
    switch_canbus_btn->setChecked(checked);
    switch_task_btn->setChecked(checked);

    CheckAvosStatus();
}  

void MainWindow::on_radio_button_automode_changed(bool value)
{
    QString msg = "";
    if (ui->idle_mode_rbtn->isChecked())
    {
        qDebug() << "idle_mode_rbtn";
        if (VehicleControl(AUTO_DRIVER, 0, msg))
        {
            QMessageBox::information(this, "信息提示", "待机模式切换成功！");
        }
        else
        {
            QMessageBox::information(this, "信息提示", msg);
        }
    }
    else if (ui->autodriver_mode_rbtn->isChecked())
    {
        qDebug() << "autodriver_mode_rbtn";
        if (VehicleControl(AUTO_DRIVER, 1, msg))
        {
            QMessageBox::information(this, "信息提示", "自动驾驶模式切换成功！");
        }
        else
        {
            QMessageBox::information(this, "信息提示", msg);
        }
    }
    else if (ui->maule_mode_rbtn->isChecked())
    {
        qDebug() << "maule_mode_rbtn";
        if (VehicleControl(AUTO_DRIVER, 0, msg))
        {
            QMessageBox::information(this, "信息提示", "人工驾驶模式切换成功！");
        }
        else
        {
            QMessageBox::information(this, "信息提示", msg);
        }
    }
}

void MainWindow::on_turn_left_light_btn_clicked()
{
    qDebug() << "on_turn_left_light_btn_clicked";
    QString msg = "";

    if (g_common_info_.sensor_status.turn_left_light_status != 1)
    {
        if (VehicleControl(TURN_LEFT_LIGHT, 1, msg))
        {
            g_common_info_.sensor_status.turn_left_light_status = 1;
            ui->turn_left_light_btn->setText("关左转灯");
        }
        else
        {
            g_common_info_.sensor_status.turn_left_light_status = 0;
            ui->turn_left_light_btn->setText("开左转灯");
            QMessageBox::information(this, "信息提示", msg);
        }
    }
    else
    {       
        if (VehicleControl(TURN_LEFT_LIGHT, 0, msg))
        {
            g_common_info_.sensor_status.turn_left_light_status = 0;
            ui->turn_left_light_btn->setText("开左转灯");
        }
        else
        {
            g_common_info_.sensor_status.turn_left_light_status = 1;
            ui->turn_left_light_btn->setText("关左转灯");
            QMessageBox::information(this, "信息提示", msg);
        }
    }
}
void MainWindow::on_turn_right_light_btn_clicked()
{
    qDebug() << "on_turn_right_light_btn_clicked";
    QString msg = "";

    if (g_common_info_.sensor_status.turn_right_light_status != 1)
    {
        if (VehicleControl(TURN_RIGHT_LIGHT, 1, msg))
        {
            g_common_info_.sensor_status.turn_right_light_status = 1;
            ui->turn_right_light_btn->setText("关右转灯");
        }
        else
        {
            g_common_info_.sensor_status.turn_right_light_status = 0;
            ui->turn_right_light_btn->setText("开右转灯");
            QMessageBox::information(this, "信息提示", msg);
        }
    }
    else
    {       
        if (VehicleControl(TURN_RIGHT_LIGHT, 0, msg))
        {
            g_common_info_.sensor_status.turn_right_light_status = 0;
            ui->turn_right_light_btn->setText("开右转灯");
        }
        else
        {
            g_common_info_.sensor_status.turn_right_light_status = 1;
            ui->turn_right_light_btn->setText("关右转灯");
            QMessageBox::information(this, "信息提示", msg);
        }
    }
}
void MainWindow::on_front_low_light_btn_clicked()
{
    qDebug() << "on_front_low_light_btn_clicked";
    QString msg = "";

    if (g_common_info_.sensor_status.head_lamp_low_beam_status != 1)
    {
        if (VehicleControl(LOW_BEAM, 1, msg))
        {
            g_common_info_.sensor_status.head_lamp_low_beam_status = 1;
            ui->front_low_light_btn->setText("关近光灯");
        }
        else
        {
            g_common_info_.sensor_status.head_lamp_low_beam_status = 0;
            ui->front_low_light_btn->setText("开近光灯");
            QMessageBox::information(this, "信息提示", msg);
        }
    }
    else
    {       
        if (VehicleControl(LOW_BEAM, 0, msg))
        {
            g_common_info_.sensor_status.head_lamp_low_beam_status = 0;
            ui->front_low_light_btn->setText("开近光灯");
        }
        else
        {
            g_common_info_.sensor_status.head_lamp_low_beam_status = 1;
            ui->front_low_light_btn->setText("关近光灯");
            QMessageBox::information(this, "信息提示", msg);
        }
    }
}
void MainWindow::on_front_high_light_btn_clicked()
{
    qDebug() << "on_front_high_light_btn_clicked";
    QString msg = "";

    if (g_common_info_.sensor_status.head_lamp_high_beam_status != 1)
    {
        if (VehicleControl(HIGH_BEAM, 1, msg))
        {
            g_common_info_.sensor_status.head_lamp_high_beam_status = 1;
            ui->front_high_light_btn->setText("关远光灯");
        }
        else
        {
            g_common_info_.sensor_status.head_lamp_high_beam_status = 0;
            ui->front_high_light_btn->setText("开远光灯");
            QMessageBox::warning(this, "信息提示", msg);
        }
    }
    else
    {       
        if (VehicleControl(HIGH_BEAM, 0, msg))
        {
            g_common_info_.sensor_status.head_lamp_high_beam_status = 0;
            ui->front_high_light_btn->setText("开远光灯");
        }
        else
        {
            g_common_info_.sensor_status.head_lamp_high_beam_status = 1;
            ui->front_high_light_btn->setText("关远光灯");
            QMessageBox::warning(this, "信息提示", msg);
        }
    }
}
void MainWindow::on_turn_left_btn_clicked()
{
    qDebug() << "on_turn_left_btn_clicked";
    QString msg = "";

    if (ui->turn_angle_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "操作不当，请输入转向角度！");
        return;
    }

    int turn_angle = ui->turn_angle_edit->text().toInt();

    if (turn_angle > 0)
    {
        QMessageBox::warning(this, "信息提示", "操作不当，左转向应输入负值！");
        return;
    }

    if (turn_angle < -500)
    {
        QMessageBox::warning(this, "信息提示", "操作不当，左转向角度不能小于-500！");
        return;
    }

    if (VehicleControl(TURN_LEFT, turn_angle, msg))
    {
        QMessageBox::information(this, "信息提示", "转向指令下发成功！");
    }
    else
    {
        QMessageBox::information(this, "信息提示", msg);
    }
}
void MainWindow::on_turn_right_btn_clicked()
{
    qDebug() << "on_turn_right_btn_clicked";
    QString msg = "";

    if (ui->turn_angle_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "操作不当，请输入转向角度！");
        return;
    }

    int turn_angle = ui->turn_angle_edit->text().toInt();

    if (turn_angle < 0)
    {
        QMessageBox::warning(this, "信息提示", "操作不当，右转向应输入正值！");
        return;
    }

    if (turn_angle > 500)
    {
        QMessageBox::warning(this, "信息提示", "操作不当，右转向角度不能超过500！");
        return;
    }

    if (VehicleControl(TURN_RIGHT, turn_angle, msg))
    {
        QMessageBox::information(this, "信息提示", "转向指令下发成功！");
    }
    else
    {
        QMessageBox::information(this, "信息提示", msg);
    }
} 

bool MainWindow::CheckLidarStatus()
{
    if (getParamString("sensor_param","lidar_ip") != "192.168.1.200")
    {
        return false;
    }

    if (getParamString("sensor_param","msop_port") != "2368")
    {
        return false;
    }

    if (getParamString("sensor_param","difop_port") != "2369")
    {
        return false;
    }
    return true;
}

bool MainWindow::CheckNavStatus()
{
    if (getParamString("sensor_param","navigation_port") != "/dev/ttyTHS4")
    {
        return false;
    }

    if (getParamString("sensor_param","navigation_butr") != "230400")
    {
        return false;
    }
    return true;
}

bool MainWindow::CheckMmStatus()
{
    if (getParamString("sensor_param","mm_fl_state_id") != "601")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_fl_table_id") != "603")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_fm_state_id") != "611")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_fm_table_id") != "613")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_fr_state_id") != "631")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_fr_table_id") != "633")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_bl_state_id") != "651")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_bl_table_id") != "653")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_br_state_id") != "671")
    {
        return false;
    }

    if (getParamString("sensor_param","mm_br_table_id") != "673")
    {
        return false;
    }
    return true;
}

bool MainWindow::CheckAvosStatus()
{
    if (g_common_info_.autodriver_status.navigation && 
        g_common_info_.autodriver_status.lidar_32 &&
        g_common_info_.autodriver_status.mm_lidar &&
        g_common_info_.autodriver_status.camera &&
        g_common_info_.autodriver_status.perception &&
        g_common_info_.autodriver_status.localization &&
        g_common_info_.autodriver_status.control &&
        g_common_info_.autodriver_status.planning &&
        g_common_info_.autodriver_status.canbus &&
        g_common_info_.autodriver_status.business)
    {
        g_common_info_.business_status.avos_status = 1;
        return true;
    }

    g_common_info_.business_status.avos_status = 0;
    return false;
}

// uint8   turn_light_left_ctrl #  左转向灯控制      "0：关闭 1：开启       "
// uint8   turn_light_right_ctrl # 右转向灯控制      "0：关闭 1：开启       "
// uint8   high_beam #远光灯     "0：关闭 1：开启       "
// uint8   low_beam #近光灯     "0：关闭 1：开启       "
// int32   turn_angle #转向角
// uint8   epsxbw_enable   #方向盘使能 0:停止方向盘线控控制 1:方向盘线控控制
// uint8   enter_autodriver #驾驶模式切换: 0:退出自动驾驶模式 1:进入自动驾驶模式
// ENTER_AUTO_DRIVER,
// TURN_LEFT_LIGHT,
// TURN_RIGHT_LIGHT,
// TURN_LEFT,
// TURN_RIGHT,
// LOW_BEAM,
// HIGH_BEAM
bool MainWindow::VehicleControl(VehicleControlType type, int cmd, QString &msg)
{    
    business_platform_api_msgs::BusinessCloudInfo input;
    business_platform_api_msgs::AvosServiceReply output;

    switch(type)
    {
        case AUTO_DRIVER:
        {
            input.turn_light_left_ctrl = 0;
            input.turn_light_right_ctrl = 0;
            input.high_beam = 0;
            input.low_beam = 0;
            input.turn_angle = 0;
            input.epsxbw_enable = 1;
            input.enter_autodriver = cmd;
            break;
        }
        case TURN_LEFT_LIGHT:
        {
            input.turn_light_left_ctrl = cmd;
            input.turn_light_right_ctrl = 0;
            input.high_beam = 0;
            input.low_beam = 0;
            input.turn_angle = 0;
            input.epsxbw_enable = 1;
            input.enter_autodriver = 1;
            break;
        }
        case TURN_RIGHT_LIGHT:
        {
            input.turn_light_left_ctrl = 0;
            input.turn_light_right_ctrl = cmd;
            input.high_beam = 0;
            input.low_beam = 0;
            input.turn_angle = 0;
            input.epsxbw_enable = 1;
            input.enter_autodriver = 1;
            break;
        }
        case TURN_LEFT:
        {
            input.turn_light_left_ctrl = 0;
            input.turn_light_right_ctrl = 0;
            input.high_beam = 0;
            input.low_beam = 0;
            input.turn_angle = cmd;
            input.epsxbw_enable = 1;
            input.enter_autodriver = 1;
            break;
        }
        case TURN_RIGHT:
        {
            input.turn_light_left_ctrl = 0;
            input.turn_light_right_ctrl = 0;
            input.high_beam = 0;
            input.low_beam = 0;
            input.turn_angle = cmd;
            input.epsxbw_enable = 1;
            input.enter_autodriver = 1;
            break;
        }
        case LOW_BEAM:
        {
            input.turn_light_left_ctrl = 0;
            input.turn_light_right_ctrl = 0;
            input.high_beam = 0;
            input.low_beam = cmd;
            input.turn_angle = 0;
            input.epsxbw_enable = 1;
            input.enter_autodriver = 1;
            break;
        }
        case HIGH_BEAM:
        {
            input.turn_light_left_ctrl = 0;
            input.turn_light_right_ctrl = 0;
            input.high_beam = cmd;
            input.low_beam = 0;
            input.turn_angle = 0;
            input.epsxbw_enable = 1;
            input.enter_autodriver = 1;
            break;
        }
        default:
        break;
    }
    avos::service::ServiceClientBridge service_client("/business_platform/service");
    int value = service_client.CallFunc<business_platform_api_msgs::BusinessCloudInfo, business_platform_api_msgs::AvosServiceReply>(6, 3, input, output );

    if (value == -1)
    {
        msg = "调用车辆控制服务失败，请查检连接！";
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
            msg = QString("指令执行失败(%1)!").arg(QString::fromStdString(output.addition_message));
            return false;
        }
    }
    return false;
}


bool MainWindow::BagRecordStart(QString & msg)
{
    avos::service::ServiceClientBridge service_client("/business_platform/service");
    std_msgs::UInt8 input;
    business_platform_api_msgs::BagrecordTopicInfo output;

    input.data = 0;
    int value = service_client.CallFunc<std_msgs::UInt8, business_platform_api_msgs::BagrecordTopicInfo>(5, 1, input, output);

    if (value == -1)
    {
        msg = "调用bag记录服务失败，请查检连接！";
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
            msg = QString("记录Bag执行失败(%1)!").arg(QString::fromStdString(output.addition_message));
            return false;
        }
    }
}


void MainWindow::UpdateStatus()
{
    static QString txt; 
    static AvvsCommonInfo tmp_info;

    if (g_common_info_.sensor_gps.status != tmp_info.sensor_gps.status)
    {
        txt = GetGpsStateInfo(g_common_info_.sensor_gps.status); 
        ui->label_gps_status->setText(txt);
        tmp_info.sensor_gps.status = g_common_info_.sensor_gps.status; 
    }

    if (g_common_info_.sensor_gps.lon != tmp_info.sensor_gps.lon)
    {
        txt = QString::number(g_common_info_.sensor_gps.lon,'f',9);  
        ui->label_longtitude->setText(txt); 
        tmp_info.sensor_gps.lon = g_common_info_.sensor_gps.lon; 
    }

    if (g_common_info_.sensor_gps.lat != tmp_info.sensor_gps.lat)
    {
        txt = QString::number(g_common_info_.sensor_gps.lat,'f',9); 
        ui->label_latitude->setText(txt);   
        tmp_info.sensor_gps.lat = g_common_info_.sensor_gps.lat; 
    }

    if (g_common_info_.business_status.avos_version != tmp_info.business_status.avos_version)
    {
        txt = QString::fromStdString(g_common_info_.business_status.avos_version);
        ui->label_version_info->setText(txt);   
        tmp_info.business_status.avos_version = g_common_info_.business_status.avos_version;   
    }

    if (g_common_info_.business_status.vehicle_vin != tmp_info.business_status.vehicle_vin)
    {
        txt = QString::fromStdString(g_common_info_.business_status.vehicle_vin);
        ui->label_vehicle_vin->setText(txt);  
        tmp_info.business_status.vehicle_vin = g_common_info_.business_status.vehicle_vin;   
    }

    if (g_common_info_.business_status.map_id != tmp_info.business_status.map_id)
    {
        txt = QString::fromStdString(g_common_info_.business_status.map_id);
        ui->label_map_id->setText(txt);  
        tmp_info.business_status.map_id = g_common_info_.business_status.map_id;   
    }

    if (g_common_info_.sensor_gps.heading != tmp_info.sensor_gps.heading)
    {
        txt = QString::number(g_common_info_.sensor_gps.heading,'f',6); 
        ui->label_heading->setText(txt);  
        tmp_info.sensor_gps.heading = g_common_info_.sensor_gps.heading;   
    }

    if (g_common_info_.sensor_gps.is_heading_valid != tmp_info.sensor_gps.is_heading_valid)
    {
        // txt = QString("%1").arg(g_common_info_.sensor_gps.is_heading_valid);
        if (g_common_info_.sensor_gps.is_heading_valid == 1)
        {
            ui->label_heading_valid->setText("航向有效:");  
        }
        else
        {
            ui->label_heading_valid->setText("航向无效:");              
        }
        tmp_info.sensor_gps.is_heading_valid = g_common_info_.sensor_gps.is_heading_valid;   
    }

    if (g_common_info_.sensor_gps.satenum != tmp_info.sensor_gps.satenum)
    {
        txt = QString("%1").arg(g_common_info_.sensor_gps.satenum);
        ui->label_gps_satenum->setText(txt);  
        tmp_info.sensor_gps.satenum = g_common_info_.sensor_gps.satenum;   
    }
}


}  // namespace avos_monitor