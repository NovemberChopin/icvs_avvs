#include "system_toolbar.h"


QColor background_color = QColor(255, 255, 255);
using namespace std;

const int icon_width = 25;
const int icon_height = 25;



float ObjectDebugGetGFunc(void* param) {
    CubeSphereObject *object = (CubeSphereObject *)param;
    PerceptionDebugAddition* addition = (PerceptionDebugAddition*)(object->addition_message);
    // std::cout << std::hex << "source:" << addition->source << "|" << (int)(addition->source & SOURCE_MASK) << std::endl;
    if ((int)(addition->source & SOURCE_MASK) == (int)VISION_SOURCE)  return ((VISION_COLOR >> 8) & 0xff ) / 255.0;
    if ((int)(addition->source & SOURCE_MASK) == (int)RADAR_SOURCE)  return  ((RADAR_COLOR >> 8) & 0xff ) / 255.0;
    if ((int)(addition->source & SOURCE_MASK) == (int)LIDAR_SOURCE)  return ((LIDAR_COLOR >> 8) & 0xff ) / 255.0;
    // std::cout << "end" << std::endl;
    return 0.0;
}

float ObjectDebugGetRFunc(void* param) {
    CubeSphereObject *object = (CubeSphereObject *)param;
    PerceptionDebugAddition* addition = (PerceptionDebugAddition*)(object->addition_message);
    if ((int)(addition->source & SOURCE_MASK) == (int)VISION_SOURCE) return ((VISION_COLOR >> 16) & 0xff ) / 255.0;
    if ((int)(addition->source & SOURCE_MASK) == (int)RADAR_SOURCE) return ((RADAR_COLOR >> 16) & 0xff ) / 255.0;
    if ((int)(addition->source & SOURCE_MASK) == (int)LIDAR_SOURCE) return ((LIDAR_COLOR >> 16) & 0xff ) / 255.0;
    return 0.0;
}

float ObjectDebugGetBFunc(void* param) {
    CubeSphereObject *object = (CubeSphereObject *)param;
    PerceptionDebugAddition* addition = (PerceptionDebugAddition*)(object->addition_message);
    if ((int)(addition->source & SOURCE_MASK) == (int)VISION_SOURCE) return ((VISION_COLOR >> 0) & 0xff ) / 255.0;
    if ((int)(addition->source & SOURCE_MASK) == (int)RADAR_SOURCE) return ((RADAR_COLOR >> 0) & 0xff ) / 255.0;
    if ((int)(addition->source & SOURCE_MASK) == (int)LIDAR_SOURCE) return ((LIDAR_COLOR >> 0) & 0xff ) / 255.0;
    return 0.0;
}

float LaneGetGFunc(void* param) {
    return 0.0;
}
float LaneGetBFunc(void* param) {
    return 0.0;
}
float LaneGetRFunc(void* param) {
    return 1.0;
}

float TrajectoryGetGFunc(void* param) {
    planning_msgs::TrajectoryPoint *point = (planning_msgs::TrajectoryPoint *)param;
    if (fabs(point->velocity) <= 0.001 ) return 1.0;
    if (point->velocity < 0)    return 0.0;
    if (point->velocity >= UP_SPEED) return 0.0;
    if (point->velocity < UP_SPEED / 2) return 1.0;
    if (point->velocity < UP_SPEED) return (2 - (point->velocity) / (UP_SPEED / 2));
    return 0.0;
}

float TrajectoryGetRFunc(void* param) {
    planning_msgs::TrajectoryPoint *point = (planning_msgs::TrajectoryPoint *)param;
    if (fabs(point->velocity) <= 0.001 ) return 1.0;
    if (point->velocity < 0)    return 0.0;
    if (point->velocity >= UP_SPEED) return 1.0;
    if (point->velocity < UP_SPEED / 2) return point->velocity / (UP_SPEED / 2);
    if (point->velocity < UP_SPEED) return 1.0;
    return 0.0;
}

float TrajectoryGetBFunc(void* param) {
    planning_msgs::TrajectoryPoint *point = (planning_msgs::TrajectoryPoint *)param;
    if (fabs(point->velocity) <= 0.001 ) return 1.0;
    if (point->velocity < 0)    return 0.0;
    if (point->velocity >= UP_SPEED) return 0.0;
    if (point->velocity < UP_SPEED / 2) return 0.0;
    if (point->velocity < UP_SPEED) return 0.0;
    return 0.0;
}

std::string ObjectToStringFunc(void* param) {
    CubeSphereObject *object = (CubeSphereObject *)param;
    PerceptionAddition *addition = (PerceptionAddition *)(object->addition_message);
    std::stringstream out;
    out << std::setprecision(2) << "id:" << addition->id << endl;
    out << std::setprecision(2) << "x|y:" << object->x << "|" << object->y << endl;
    // out << std::setprecision(2) << "vxrel|vyrel:" << addition->vxrel << "|" << addition->vyrel << endl;
    // out << std::setprecision(2) << "speed:" << addition->speed << endl;
    out << std::setprecision(2) << "type|speed:" << addition->type << "|" << addition->speed << endl;
    return out.str();
}

std::string TrajectoryToStringFunc(void* param) {
    planning_msgs::TrajectoryPoint *point = (planning_msgs::TrajectoryPoint *)param;
    std::stringstream out;
    out << std::setprecision(2) << "init_v:" << point->velocity << endl;
    return out.str();
}

// 构造函数，初始化变量
SystemToolbar::SystemToolbar( QWidget* parent ) :
    QWidget(parent)
{
    config_file_path = ros::package::getPath("avos_monitor")+"/config/tool_bar.yaml";
    OnCreateButton();
    dashboard = new hugo_scene();
    graphicsView = new QGraphicsView();
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->horizontalScrollBar()->blockSignals(true);
    graphicsView->verticalScrollBar()->blockSignals(true);
    graphicsView->setFixedSize(460,350);
    graphicsView->setScene(dashboard);

    text_color_ = QColor(255,255,255);
    fault_level_color_0_ = QColor(76, 148, 13);
    fault_level_color_1_ = QColor(217, 148, 0);
    fault_level_color_2_ = QColor(190, 0, 0);
    gray_color_ = QColor(105, 105, 105);
    is_first = true;

    show_stat_table = new QTableWidget(10,4);
    InitTableView(show_stat_table);

    sensor_table_ = new QTableWidget(12,4);
    InitSensorTable(sensor_table_);

    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(pStartDebugBtn);
    pHLayout->addWidget(pMapEditBtn);
    pHLayout->addWidget(pMapManageBtn);
    pHLayout->addWidget(pTaskManageBtn);
    pHLayout->addWidget(pCanDebugBtn);
    pHLayout->addWidget(pSettingBtn);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(graphicsView, 0, 0);
    mainLayout->addLayout(pHLayout, 1, 0);
    mainLayout->addWidget(sensor_table_, 2, 0);
    mainLayout->addWidget(show_stat_table, 3, 0);
    setLayout(mainLayout);

    g_common_info_.business_status.intensity4g = 4;

    fc_radar = new QTimer(this);    // 前中毫米波雷达错误显示定时器
    fl_radar = new QTimer(this);    // 前左
    fr_radar = new QTimer(this);    // 前右
    nav_timer = new QTimer(this);
    lidar_timer = new QTimer(this);
    connect(fc_radar, &QTimer::timeout, this, &SystemToolbar::handle_fc_radar_timeout);
    connect(fl_radar, &QTimer::timeout, this, &SystemToolbar::handle_fl_radar_timeout);
    connect(fr_radar, &QTimer::timeout, this, &SystemToolbar::handle_fr_radar_timeout);
    connect(nav_timer, &QTimer::timeout, this, &SystemToolbar::handle_nav_timeout);
    connect(lidar_timer, &QTimer::timeout, this, &SystemToolbar::handle_lidar_timerout);
    
}

void SystemToolbar::handle_fl_radar_timeout() {
    if (fl_radar->isActive()) {
        fl_radar->stop();
        g_common_info_.sensor_status.mill_radar[0] = 1;     // 前左
        std::cout << "fl_radar timer stop" << std::endl;
    }
}


void SystemToolbar::handle_fc_radar_timeout() {
    if (fc_radar->isActive()) {
        fc_radar->stop();
        g_common_info_.sensor_status.mill_radar[1] = 1;     // 前中
        std::cout << "fc_radar timer stop" << std::endl;
    }
}


void SystemToolbar::handle_fr_radar_timeout() {
    if (fr_radar->isActive()) {
        fr_radar->stop();
        g_common_info_.sensor_status.mill_radar[2] = 1;     // 前右
        std::cout << "fr_radar timer stop" << std::endl;
    }
}

void SystemToolbar::handle_nav_timeout() {
    if (nav_timer->isActive()) {
        nav_timer->stop();
        g_common_info_.sensor_status.nav_status = 1;        // 组合惯导 状态设置为正常
        std::cout << "nav_timer stop" << std::endl;
    }
}

void SystemToolbar::handle_lidar_timerout() {
    if (lidar_timer->isActive()) {
        lidar_timer->stop();
        g_common_info_.sensor_status.lidar_status = 1;
    }
}


void SystemToolbar::InitRosSub()
{
    ros::NodeHandle nh;
    nh.param<std::string>("gloabal_frame", global_frame_, "base_link");

    sub_tpcarfaults         = nh.subscribe("/tpcarfaults", 100, &SystemToolbar::CallbackCarfaults, this);
    // sub_tpmonitordb103      = nh.subscribe("/tpcanfault", 100, &SystemToolbar::CallbackVehicleFault, this);
    sub_tpcontrolsys        = nh.subscribe("/control_sys", 100, &SystemToolbar::CallbackControlSys, this);
    sub_tpimu               = nh.subscribe("/tpimu", 100, &SystemToolbar::CallbackSensorIMU, this);
    sub_tpcansensor         = nh.subscribe("/tpcansensor", 100, &SystemToolbar::CallbackCanSensor, this);
    sub_chassis_status      = nh.subscribe("/chassic_status_info", 100, &SystemToolbar::CallbackClassisState, this);
    sub_traffic_light       = nh.subscribe("/tptrafficlight", 100, &SystemToolbar::CallbackTrafficLight, this);
    sub_tpbusiness_status   = nh.subscribe("/tpbusiness_status", 100, &SystemToolbar::CallbackBusiness, this);
    sub_sensorgps_          = nh.subscribe("/ivsensorgps", 100, &SystemToolbar::CallbackSensorgps, this);
    sub_lon_control_debug   = nh.subscribe("/lon_control_debug", 100, &SystemToolbar::CallbackLonDebug, this);

    sub_pathplan_           = nh.subscribe("/tppathplan", 10, &SystemToolbar::CallbackTrajectory, this);
    sub_perception_         = nh.subscribe("/tpperception", 10, &SystemToolbar::CallbackPerception, this);
    sub_perception_debug_   = nh.subscribe("/tpperceptiondebug", 10, &SystemToolbar::CallbackPerceptionDebug, this);
    sub_mapegine_           = nh.subscribe("/mapengine/tpnavigation", 10, &SystemToolbar::CallbackMapengine, this);
    sub_prediction_         = nh.subscribe("/tpprediction", 10, &SystemToolbar::CallbackPredictionObjects, this);

    // 使用 fault_info 来判断系统状态
    sub_fault_info = nh.subscribe("/fault_info", 1, &SystemToolbar::CallbackFaultInfo, this);

    // 毫米波雷达
    sub_fl_radar = nh.subscribe("/fl_radar_objects", 1, &SystemToolbar::CallbackFLRadar, this);
    sub_fr_radar = nh.subscribe("/fr_radar_objects", 1, &SystemToolbar::CallbackFRRadar, this);
    sub_rl_radar = nh.subscribe("/rl_radar_objects", 1, &SystemToolbar::CallbackRLRadar, this);

    pub_path_marker_            = nh.advertise<visualization_msgs::Marker>("path_visualize", 1);
    pub_mapengine_marker        = nh.advertise<PathplanPointCloud> ( "mapengine_path", 1, true );
    pub_left_mapengine_marker   = nh.advertise<PathplanPointCloud> ( "left_mapengine_path", 1, true );
    pub_right_mapengine_marker  = nh.advertise<PathplanPointCloud> ( "right_mapengine_path", 1, true );
    pub_rev_mapengine_marker    = nh.advertise<PathplanPointCloud> ( "rev_mapengine_path", 1, true );
    pub_trajectory_marker       = nh.advertise<PathplanPointCloud> ( "trajectory_marker_line", 1, true );
    pub_trajectory_text_marker  = nh.advertise<visualization_msgs::MarkerArray> ( "trajectory_marker_text", 1, true );
    pub_object_marker           = nh.advertise<visualization_msgs::MarkerArray> ( "objectfusion_marker", 1, true );
    pub_object_bbox             = nh.advertise<visualization_msgs::MarkerArray>( "objectfusion_bbox", 1);
    pub_objectcell              = nh.advertise<MapPointCloudRGB> ( "perception_object_cell", 1, true );
    pub_staticcell              = nh.advertise<MapPointCloudRGB> ( "perception_static_cell", 1, true );
    pub_base_map                = nh.advertise<MapPointCloudRGB> ( "basemap_rgb", 1, true );
    pub_object_text_marker      = nh.advertise<visualization_msgs::MarkerArray> ( "objectfusion_marker_text", 1, true );
    pub_object_debug_marker_    = nh.advertise<visualization_msgs::MarkerArray> ( "objectfusion_debug_marker", 1, true );
    pub_car_cloud_              = nh.advertise<MapPointCloudRGB> ( "car_layout", 1, true );
    pub_prediction_trajectory_  = nh.advertise<visualization_msgs::MarkerArray> ( "prediction_trajectory", 1, true );
    pub_vector_map_             = nh.advertise<PathplanPointCloud> ( "vector_map", 1, true );

    gobjectfusion_debug.SetGFunc(ObjectDebugGetGFunc);
    gobjectfusion_debug.SetRFunc(ObjectDebugGetRFunc);
    gobjectfusion_debug.SetBFunc(ObjectDebugGetBFunc);

    gtrajectory.SetGFunc(TrajectoryGetGFunc);
    gtrajectory.SetRFunc(TrajectoryGetRFunc);
    gtrajectory.SetBFunc(TrajectoryGetBFunc);

    gobjectfusion_text.SetToStringFunc(ObjectToStringFunc);
    gtrajectory_text.SetToStringFunc(TrajectoryToStringFunc);

    LoadBaseMap();
    LoadVectorMap();
    InitShowCar(nh);

    GetCarCells();

    m_nTimerId = startTimer(300);
    m_nTimerShow = startTimer(100);
}

void SystemToolbar::ExitDebug()
{
    sub_tpcarfaults.shutdown();
    // sub_tpmonitordb103.shutdown();
    sub_tpcontrolsys.shutdown();
    sub_tpimu.shutdown();
    sub_tpcansensor.shutdown();
    sub_chassis_status.shutdown();
    sub_pathplan_.shutdown();
    sub_perception_.shutdown();
    sub_perception_debug_.shutdown();
    sub_mapegine_.shutdown();
    sub_prediction_.shutdown();
    sub_tpbusiness_status.shutdown();
    sub_lon_control_debug.shutdown();
    sub_sensorgps_.shutdown();
    sub_fl_radar.shutdown();
    sub_fr_radar.shutdown();
    sub_rl_radar.shutdown();

    pub_mapengine_marker.shutdown();
    pub_left_mapengine_marker.shutdown();
    pub_right_mapengine_marker.shutdown();
    pub_rev_mapengine_marker.shutdown();
    pub_trajectory_marker.shutdown();
    pub_trajectory_text_marker.shutdown();
    pub_object_marker.shutdown();
    pub_object_bbox.shutdown();
    pub_objectcell.shutdown();
    pub_staticcell.shutdown();
    pub_base_map.shutdown();
    pub_object_text_marker.shutdown();
    pub_object_debug_marker_.shutdown();
    pub_car_cloud_.shutdown();
    pub_prediction_trajectory_.shutdown();
	pub_car_.shutdown();

	killTimer(m_nTimerId);
	killTimer(m_nTimerShow);
}

void SystemToolbar::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerId)
    {
        UpdateStatus(); 
        // UpdateDashboardStatus(); 
        std::cout<<"UpdateStatus ...."<<std::endl;
    }

    if (event->timerId() == m_nTimerShow)
    {
        ShowBaseMap();
        ShowVectormap();
        PublisherRvizThread();
        DisplayCar();
    }
}
/**********************************************
 * Function: ~SystemToolbar()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
SystemToolbar::~SystemToolbar()
{
    killTimer(m_nTimerId);
    killTimer(m_nTimerShow);
    sub_tpcarfaults.shutdown();
}


/**********************************************
 * Function: OnCreateButton()
 * Author: zengzicheng
 * Date: 2019-12-07
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void SystemToolbar::OnCreateButton()
{    
    std::string button_path;

    pTaskManageBtn = new QPushButton("任务");
    connect(pTaskManageBtn, SIGNAL(released()), this, SLOT(OnTaskManageButton()));
    pTaskManageBtn->setStyleSheet("QPushButton{color:black;background:transparent}");    
    button_path = ros::package::getPath("avos_monitor")+"/resources/button/taskbtn.png";
    QIcon taskbtn_icon;
    taskbtn_icon.addFile(QString::fromStdString(button_path));
    pTaskManageBtn->setIcon(taskbtn_icon);
    pTaskManageBtn->setIconSize(QSize(icon_width, icon_height));
    pTaskManageBtn->setFlat(true);

    pSettingBtn = new QPushButton("设置");
    connect(pSettingBtn, SIGNAL(released()), this, SLOT(SettingButton()));
    pSettingBtn->setStyleSheet("QPushButton{color:black;background:transparent}");  
    button_path = ros::package::getPath("avos_monitor")+"/resources/button/settingbtn.png";
    QIcon monitor_icon;
    monitor_icon.addFile(QString::fromStdString(button_path));
    pSettingBtn->setIcon(monitor_icon);
    pSettingBtn->setIconSize(QSize(icon_width, icon_height));
    pSettingBtn->setFlat(true);

    pStartDebugBtn = new QPushButton("调试");
    connect(pStartDebugBtn, SIGNAL(released()), this, SLOT(OnDebugButton()));
    pStartDebugBtn->setStyleSheet("QPushButton{color:black;background:transparent}");
    button_path = ros::package::getPath("avos_monitor")+"/resources/button/playbtn.png";
    QIcon video_icon;
    video_icon.addFile(QString::fromStdString(button_path));
    pStartDebugBtn->setIcon(video_icon);
    pStartDebugBtn->setIconSize(QSize(icon_width, icon_height));
    pStartDebugBtn->setFlat(true);

    pMapManageBtn = new QPushButton("管理");
    connect(pMapManageBtn, SIGNAL(released()), this, SLOT(OnMapManageButton()));
    pMapManageBtn->setStyleSheet("QPushButton{color:black;background:transparent}");
    button_path = ros::package::getPath("avos_monitor")+"/resources/button/mapacqubtn.png";
    QIcon mapload_icon;
    mapload_icon.addFile(QString::fromStdString(button_path));
    pMapManageBtn->setIcon(mapload_icon);
    pMapManageBtn->setIconSize(QSize(icon_width, icon_height));
    pMapManageBtn->setFlat(true);

    pCanDebugBtn = new QPushButton("CAN");
    connect(pCanDebugBtn, SIGNAL(released()), this, SLOT(OnCanDebugButton()));
    pCanDebugBtn->setStyleSheet("QPushButton{color:black;background:transparent}");
    button_path = ros::package::getPath("avos_monitor")+"/resources/button/runnability.png";
    QIcon playbag_icon;
    playbag_icon.addFile(QString::fromStdString(button_path));
    pCanDebugBtn->setIcon(playbag_icon);
    pCanDebugBtn->setIconSize(QSize(icon_width, icon_height));
    pCanDebugBtn->setFlat(true);

    pMapEditBtn = new QPushButton("地图");
    connect(pMapEditBtn, SIGNAL(released()), this, SLOT(OnMapEditButton()));
    pMapEditBtn->setStyleSheet("QPushButton{color:black;background:transparent}");
    button_path = ros::package::getPath("avos_monitor")+"/resources/button/maploadbtn.png";
    QIcon map_edit_icon;
    map_edit_icon.addFile(QString::fromStdString(button_path));
    pMapEditBtn->setIcon(map_edit_icon);
    pMapEditBtn->setIconSize(QSize(icon_width, icon_height));
    pMapEditBtn->setFlat(true);
}

/**********************************************
 * Function: InitTableView()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void SystemToolbar::InitTableView(QTableWidget* table)
{
    // table->setFixedHeight(282);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格只读，不能进行编辑
    table->setShowGrid(true);   //不显示网格线
    // table->horizontalHeader()->hide();
    table->verticalHeader()->hide();
    // table->setFixedWidth(TABLE_WIDTH); 
    // table->verticalHeader()->setStretchLastSection(true); //设置充满表宽度
    table->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    // table->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选中时为整行选中
    table->setFrameShape(QFrame::NoFrame); //设置无边框
    table->setStyleSheet("background-color:#333333");//528B8B
    table->horizontalHeader()->setFont(QFont( "Times", 12, QFont::Black));
    // table->verticalHeader()->setFixedWidth(100); //set header width
    table->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:#666666;color: white;}"); //设置表头背景色
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//Qt::AlignLeft | Qt::AlignVCenter

    QStringList header;
    header<<"状态含义"<<"当前状态"<<"状态含义"<<"当前状态";
    table->setHorizontalHeaderLabels(header);

    table->setColumnWidth(0,110);
    table->setColumnWidth(1,110);
    table->setColumnWidth(2,110);

    QStringList status_name_1, status_name_2;
    status_name_1<<"CAN0"<<"系统状态"<<"驾驶模式"<<"车辆速度"<<"定位状态"<<"GPS状态"<<"档位信息"<<"EPS状态"<<"VCU状态"<<"NBoost";

    status_name_2<<"CAN1"<<"电池电量"<<"左转向灯"<<"右转向灯"<<"近光灯"<<"远光灯"<<"红绿灯"<<"V2V"<<"系统版本"<<"车辆VIN";

    QTableWidgetItem *item;
    QColor background_color = QColor(80,80,80);
    QColor text_color = QColor(255,255,255);
    int column = 0;
    for(int row = 0; row < status_name_1.size(); row++)
    {
        item = table->item(row, column);        
        if(item) 
        {
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
        }
        else
        {
            item = new QTableWidgetItem;
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
            table->setItem(row, column, item);
        }
        item->setFont(QFont( "Times", 11, QFont::Black));
        item->setText(status_name_1.at(row));
        item->setTextAlignment(Qt::AlignCenter);
        table->setRowHeight(row, 28);  
    }

    column = 2;
    for(int row = 0; row < status_name_2.size(); row++)
    {
        item = table->item(row, column);        
        if(item) 
        {
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
        }
        else
        {
            item = new QTableWidgetItem;
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
            table->setItem(row, column, item);
        }
        item->setFont(QFont( "Times", 11, QFont::Black));
        item->setText(status_name_2.at(row));
        item->setTextAlignment(Qt::AlignCenter);
    }
}

/**********************************************
 * Function: InitSensorTable()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void SystemToolbar::InitSensorTable(QTableWidget* table)
{
    // table->setFixedHeight(282);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格只读，不能进行编辑
    table->setShowGrid(true);   //不显示网格线
    // table->horizontalHeader()->hide();
    table->verticalHeader()->hide();
    // table->setFixedWidth(TABLE_WIDTH); 
    // table->verticalHeader()->setStretchLastSection(true); //设置充满表宽度
    table->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    // table->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选中时为整行选中
    table->setFrameShape(QFrame::NoFrame); //设置无边框
    table->setStyleSheet("background-color:#333333");//528B8B
    table->horizontalHeader()->setFont(QFont( "Times", 12, QFont::Black));
    // table->verticalHeader()->setFixedWidth(100); //set header width
    table->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:#666666;color: white;}"); //设置表头背景色
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//Qt::AlignLeft | Qt::AlignVCenter

    QStringList header;
    header<<"传感器op"<<"当前状态"<<"超声波"<<"当前状态";
    table->setHorizontalHeaderLabels(header);

    table->setColumnWidth(0,110);
    table->setColumnWidth(1,110);
    table->setColumnWidth(2,110);

    QStringList status_name_1, status_name_2;
    status_name_1<<"激光雷达op"<<"组合导航op"<<"前左毫米波op"<<"前中毫米波"<<"前右毫米波"<<"后左毫米波"<<"后右毫米波"<<"高清摄像头"<<"前摄像头"<<"左摄像头"<<"后摄像头"<<"右摄像头";

    status_name_2<<"右侧前方op"<<"右侧后中op"<<"右侧后方op"<<"后方右侧op"<<"后方中间op"<<"后方左侧"<<"左侧后方"<<"左侧前中"<<"左侧前方"<<"前方左侧"<<"前方中间"<<"前方右侧";

    QTableWidgetItem *item;
    QColor background_color = QColor(80,80,80);
    QColor text_color = QColor(255,255,255);
    int column = 0;
    for(int row = 0; row < status_name_1.size(); row++)
    {
        item = table->item(row, column);        
        if(item) 
        {
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
        }
        else
        {
            item = new QTableWidgetItem;
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
            table->setItem(row, column, item);
        }
        item->setFont(QFont( "Times", 11, QFont::Black));
        
        item->setText(status_name_1.at(row));
        item->setTextAlignment(Qt::AlignCenter);
        table->setRowHeight(row, 28);  
    }

    column = 2;
    for(int row = 0; row < status_name_2.size(); row++)
    {
        item = table->item(row, column);        
        if(item) 
        {
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
        }
        else
        {
            item = new QTableWidgetItem;
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
            table->setItem(row, column, item);
        }
        item->setFont(QFont( "Times", 11, QFont::Black));
        item->setText(status_name_2.at(row));
        item->setTextAlignment(Qt::AlignCenter);
    }

    ///## init for none
            //##TEST
    column = 1;
    for(int row = 0; row < status_name_1.size(); row++)
    {
        item = table->item(row, column);        
        if(item) 
        {
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
        }
        else
        {
            item = new QTableWidgetItem;
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
            table->setItem(row, column, item);
        }
        item->setFont(QFont( "Times", 11, QFont::Black));
        item->setText("状态未知"); 
        item->setTextAlignment(Qt::AlignCenter);
    }
    column = 3;
    for(int row = 0; row < status_name_2.size(); row++)
    {
        item = table->item(row, column);        
        if(item) 
        {
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
        }
        else
        {
            item = new QTableWidgetItem;
            item->setTextColor(text_color);
            item->setBackgroundColor(background_color);
            table->setItem(row, column, item);
        }
        item->setFont(QFont( "Times", 11, QFont::Black));
        item->setText("状态未知"); 
        item->setTextAlignment(Qt::AlignCenter);
    }
}


void SystemToolbar::OnBagReplayButton() 
{
    // bag_replay_dialog_ = new BagReplayDialog(this);
    // bag_replay_dialog_->resize(800, 600);
    // bag_replay_dialog_->show();
}

void SystemToolbar::OnTaskManageButton()
{
    task_manage_dlg_ = new TaskManageDlg(this);
    task_manage_dlg_->show();
}

void SystemToolbar::OnCanDebugButton()
{
    can_debug_dlg_ = new CanDebugDlg(this);
    can_debug_dlg_->show();
}

void SystemToolbar::OnMapEditButton()
{
	if (map_edit_dlg_ != nullptr)
	{
		delete map_edit_dlg_;
	    map_edit_dlg_ = new MapEditDlg(this);
	    map_edit_dlg_->show();
	}
	else
	{
	    map_edit_dlg_ = new MapEditDlg(this);
	    map_edit_dlg_->show();		
	}
}

void SystemToolbar::SettingButton() 
{
    if(settings != nullptr)
    {
        delete settings;
        settings=new Settings();
        settings->setWindowModality(Qt::ApplicationModal);
        settings->show();
    }
    else
    {
        settings=new Settings();
        settings->setWindowModality(Qt::ApplicationModal);
        settings->show();
    }
}
void SystemToolbar::OnbagRecordButton() 
{
    // bag_record_dialog_ = new BagRecordDialog(this);
    // bag_record_dialog_->resize(400, 400);
    // bag_record_dialog_->show();
}

void SystemToolbar::OnDebugButton()
{
    std::string button_path = "";
    QIcon video_icon;

    if (is_debug_status_)
    {

        ExitDebug();
    	
        InitSensorState("nostate");
        UpdateStatus();

	    pStartDebugBtn->setStyleSheet("QPushButton{color:black;background:transparent}");
	    button_path = ros::package::getPath("avos_monitor")+"/resources/button/playbtn.png";	    
	    video_icon.addFile(QString::fromStdString(button_path));
	    pStartDebugBtn->setIcon(video_icon);
	    pStartDebugBtn->setIconSize(QSize(icon_width, icon_height));
	    pStartDebugBtn->setFlat(true);
	    pStartDebugBtn->setText("开始");
    	is_debug_status_ = false;

        Q_EMIT removePointCloud_signal();

        // 重置传感器状态

    }
    else
    {
    	InitRosSub();

        InitSensorState("normal");

	    pStartDebugBtn->setStyleSheet("QPushButton{color:black;background:transparent}");
	    button_path = ros::package::getPath("avos_monitor")+"/resources/button/stopbtn.png";
	    video_icon.addFile(QString::fromStdString(button_path));
	    pStartDebugBtn->setIcon(video_icon);
	    pStartDebugBtn->setIconSize(QSize(icon_width, icon_height));
	    pStartDebugBtn->setFlat(true);
	    pStartDebugBtn->setText("停止");
    	is_debug_status_ = true;

        Q_EMIT addPointCloud_signal();
    }
}

void SystemToolbar::OnMapManageButton()
{
    map_select_dlg_ = new MapSelectDlg(this);
    map_select_dlg_->show();

    if (map_select_dlg_->exec() == QDialog::Accepted)
    {
        if(LoadVectorMap())
        {
            ShowVectormap();
        }

        if (LoadBaseMap())
        {
            ShowBaseMap();
        }
    }
}

/**********************************************
 * Function: CallbackCarfaults()
 * Author: zengzicheng
 * Date: 2019-11-29
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void SystemToolbar::CallbackCarfaults(const common_msgs::FaultVec::ConstPtr &msg)
{
    car_faults_msg_ = *msg;
    // UpdateDataPool();
    // SetFaultsInfo();
}

void SystemToolbar::CallbackSensorgps(const ivlocmsg::ivsensorgps::ConstPtr &msg)
{
    g_common_info_.sensor_gps.status = msg->status;
    g_common_info_.sensor_gps.lon = msg->lon;
    g_common_info_.sensor_gps.lat = msg->lat;
    g_common_info_.sensor_gps.height = msg->height;
    g_common_info_.sensor_gps.heading = msg->heading;
    g_common_info_.sensor_gps.satenum = msg->satenum;
    g_common_info_.sensor_gps.is_heading_valid = msg->is_heading_valid;
    g_common_info_.sensor_gps.velocity = msg->velocity;
}

void SystemToolbar::CallbackLonDebug(const control_msgs::LoncontrolDebug::ConstPtr &msg)
{
    g_common_info_.control_status.lon_targettorque      = msg->lon_targettorque;    
    g_common_info_.control_status.brakelight            = msg->brakelight;
}

void SystemToolbar::CallbackSensorIMU(const imu::SensorImu::ConstPtr &msg)
{    
    g_common_info_.location_info.fusion_pos.xpos        = msg->pose.pose_euler.position.x.variable;
    g_common_info_.location_info.fusion_pos.ypos        = msg->pose.pose_euler.position.y.variable;
    g_common_info_.location_info.heading                = msg->pose.pose_euler.rotation.yaw.variable;//rotation.yaw.variable
    g_common_info_.location_info.velocity               = msg->v.variable;//v.variable
    g_common_info_.location_info.lon                    = msg->llh.lon.variable;//llh.lon
    g_common_info_.location_info.lat                    = msg->llh.lat.variable;
    g_common_info_.location_info.gps_status             = msg->nav_status.gps_status;//nav_status.gps_status
    g_common_info_.location_info.height                 = msg->llh.height.variable;//llh.height
    g_common_info_.location_info.utctime                = msg->dr_pose_and_time.utctime;//dr_pose_and_time.utctime
    // g_common_info_.location_info.base_length            = msg->locpos.base_length;
    // g_common_info_.location_info.velocity_left_wheel    = msg->locpos.velocity_left_wheel;
    // g_common_info_.location_info.velocity_right_wheel   = msg->locpos.velocity_right_wheel;
    g_common_info_.location_info.gps_num                = msg->nav_status.sate_num;//nav_status.sate_num
    g_common_info_.location_info.location_status        = msg->nav_status.init_status;//nav_status.init_status 0.1.2
    // g_common_info_.location_info.localization_mode      = msg->locpos.localization_mode;

    xg_ = msg->pose.pose_euler.position.x.variable;
    yg_ = msg->pose.pose_euler.position.y.variable;
    heading_ = msg->pose.pose_euler.rotation.yaw.variable;
}

void SystemToolbar::CallbackCanSensor(const ivcanbus::ivcansensor::ConstPtr &msg)
{
    for (int i = 0; i < 12; ++i)
    {
        g_common_info_.sensor_status.ult_radar[i] = msg->ultrasonic_radar.USS_Status[i];
    }
}

void SystemToolbar::CallbackClassisState(const control_msgs::chassis_status::ConstPtr &msg)
{
    g_common_info_.sensor_status.eps_status                 = msg->Eps_status;
    g_common_info_.sensor_status.vcu_status                 = msg->VCU_DTC;
    g_common_info_.sensor_status.nboost_status              = msg->NBS_Fault_St;
    g_common_info_.sensor_status.battery_soc                = msg->HVB_SOC;
    g_common_info_.sensor_status.turn_left_light_status     = msg->left_turn_light;
    g_common_info_.sensor_status.turn_right_light_status    = msg->right_turn_light;
    g_common_info_.sensor_status.head_lamp_low_beam_status  = msg->head_lamp_low;
    g_common_info_.sensor_status.head_lamp_high_beam_status = msg->head_lamp_high;
    g_common_info_.control_status.shiftposition             = msg->real_shiftposition;
}

void SystemToolbar::CallbackTrafficLight(const perception_msgs::TrafficLightDetection::ConstPtr &msg)
{
    // int8 color #UNKNOWN_COLOR = 0; RED = 1; YELLOW = 2; GREEN = 3; BLACK = 4;
    // string id # Traffic light string-ID in the map data.
    // float32 confidence  #How confidence about the detected results, between 0 and 1.
    // float32 tracking_time  # Duration of the traffic light since detected.
    

    for (int i = 0; i < msg->traffic_light.size(); ++i)
    {
        if(msg->traffic_light[i].id == "trafficlight")//"v2v"
        {
            g_common_info_.sensor_status.traffic_light = msg->traffic_light[i].color;
        }

        if(msg->traffic_light[i].id == "v2v")//"v2v"
        {
            g_common_info_.sensor_status.v2v_status = msg->traffic_light[i].color;
        }
    }
}

// module_items_[MODULE::PLANNING_M].module_name = "planning";
// module_items_[MODULE::MAPENGINE_M].module_name = "mapengine";
// module_items_[MODULE::PERCEPTION_M].module_name = "perception";
// module_items_[MODULE::CONTROL_M].module_name = "control";
// module_items_[MODULE::LOCALIZATION_M].module_name = "localization";
void SystemToolbar::CallbackBusiness(const business_platform::BusinessStatus::ConstPtr &msg)//tpbusiness_status
{
    g_common_info_.business_status.map_id = msg->self_drive_info.current_map_name;    
    g_common_info_.business_status.avos_version = msg->self_drive_info.avos_version;
    g_common_info_.business_status.vehicle_vin = msg->self_drive_info.vin;

    for (int i = 0; i < msg->self_drive_info.monitor_table.size(); ++i)
    {
        if (msg->self_drive_info.monitor_table[i].table_name == "planning")
        {
            g_common_info_.business_status.planning = msg->self_drive_info.monitor_table[i].table_fault_level;
        }

        if (msg->self_drive_info.monitor_table[i].table_name == "mapengine")
        {
            g_common_info_.business_status.map_engine = msg->self_drive_info.monitor_table[i].table_fault_level;
        }

        if (msg->self_drive_info.monitor_table[i].table_name == "perception")
        {
            g_common_info_.business_status.perception = msg->self_drive_info.monitor_table[i].table_fault_level;
        }

        if (msg->self_drive_info.monitor_table[i].table_name == "localization")
        {
            g_common_info_.business_status.localization = msg->self_drive_info.monitor_table[i].table_fault_level;
        }

        if (msg->self_drive_info.monitor_table[i].table_name == "control")
        {
            g_common_info_.business_status.control = msg->self_drive_info.monitor_table[i].table_fault_level;
        }
    }
}

/**
 * 根据 /tpcanfault 话题查看传感器状态（已弃用）
*/
void SystemToolbar::CallbackVehicleFault(const canbus_msgs::CanFault::ConstPtr &msg)
{    
    g_common_info_.sensor_status.can_status[0] = !(msg->can_fault_flag);
    g_common_info_.sensor_status.can_status[1] = !(msg->can103_fault_flag);

    g_common_info_.sensor_status.mill_radar[0] = !(msg->mmw_lidar7_flag);
    g_common_info_.sensor_status.mill_radar[1] = !(msg->mmw_lidar0_flag);
    g_common_info_.sensor_status.mill_radar[2] = !(msg->mmw_lidar1_flag);
    g_common_info_.sensor_status.mill_radar[3] = !(msg->mmw_lidar5_flag);
    g_common_info_.sensor_status.mill_radar[4] = !(msg->mmw_lidar3_flag);

    // g_common_info_.sensor_status.camera_status[0] = !(msg->fault_camera5_flag);
    // g_common_info_.sensor_status.camera_status[1] = !(msg->fault_camera1_flag);
    // g_common_info_.sensor_status.camera_status[2] = !(msg->fault_camera2_flag);
    // g_common_info_.sensor_status.camera_status[3] = !(msg->fault_camera3_flag);
    // g_common_info_.sensor_status.camera_status[4] = !(msg->fault_camera4_flag);

    // g_common_info_.sensor_status.lidar_status = !(msg->fault_lidar_flag);
    // g_common_info_.sensor_status.nav_status = !(msg->fault_imu_flag);

    // SCH 2020.10.10
}

void SystemToolbar::CallbackControlSys(const control_msgs::control_sys::ConstPtr &msg)
{    
    g_common_info_.control_status.lat_targetangle       = msg->lat_targetangle;
    g_common_info_.control_status.lat_targettorque      = msg->lat_targettorque;
    g_common_info_.control_status.vehicle_shiftposition = msg->vehicle_shiftposition;
    g_common_info_.control_status.vehicle_sys_status    = msg->vehicle_sys_status;
    g_common_info_.control_status.actuatormode          = msg->actuatormode;
    g_common_info_.control_status.reverselight          = msg->reverselight;
    g_common_info_.control_status.epb_status            = msg->epb_status;
    g_common_info_.control_status.vehicle_auto_enable   = msg->vehicle_auto_enable;
    g_common_info_.control_status.vehicle_lat_enable    = msg->vehicle_lat_enable;
    g_common_info_.control_status.vehicle_lon_enable    = msg->vehicle_lon_enable;
    g_common_info_.control_status.vehicle_epb_status    = msg->vehicle_epb_status;
    g_common_info_.control_status.road_vaild_flag       = msg->road_vaild_flag;
    g_common_info_.control_status.calibration_flag      = msg->calibration_flag;
    g_common_info_.control_status.write_enable_flag     = msg->write_enable_flag;
    g_common_info_.control_status.PowerFault            = msg->PowerFault;
    g_common_info_.control_status.ComFault              = msg->ComFault;
    g_common_info_.control_status.USSFault              = msg->USSFault;
    g_common_info_.control_status.ChassisFault          = msg->ChassisFault;    
    // rack_xcan_time = ros::Time::now().toSec();
    // std::bitset<32> can_fault(msg->ComFault);
    
    // rack_evcan_status = !can_fault[3];
    // rack_ican_status  = !can_fault[4];
}

void SystemToolbar::InitShowCar(ros::NodeHandle nh)
{
    nh.param("lengthVehicle", length, length);
    nh.param("widthVehicle", width, width);
    nh.param("heightVehicle", height, height);
    nh.param("car_source_path", car_source_path, car_source_path);

    // car marker information
    car_.header.frame_id = global_frame_;
    car_.header.stamp = ros::Time::now();
    car_.ns = "car";
    car_.id = 0;
    car_.type = visualization_msgs::Marker::MESH_RESOURCE;
    // car_.mesh_resource = "package://ivrviz/resources/car2/file.dae";
    car_.mesh_resource = car_source_path;
    car_.mesh_use_embedded_materials = true;
    car_.lifetime = ros::Duration();
    car_.action = visualization_msgs::Marker::ADD;
    car_.scale.x = 3*length / 10;  // 1;//0.55;
    car_.scale.y = 3*width / 5;    // 1;//0.37;
    car_.scale.z = 3*height / 5;
    car_.pose.position.x = 0;//(length - 0.2) / 2;  // carbiasx;
    car_.pose.position.y = 0;//(0.7 * width) / 2;   // carbiasy;
    car_.pose.position.z = 0;

    car_.pose.orientation.w = 1.0;
    car_.frame_locked = true; 

    pub_car_ = nh.advertise<visualization_msgs::Marker>("car", 10, true);
}

void SystemToolbar::DisplayCar() 
{
    static tf2_ros::TransformBroadcaster br;
    static int seq = 0;
    car_.header.stamp = ros::Time::now();
    geometry_msgs::TransformStamped transformStamped;

    transformStamped.header.seq = seq++;
    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "map";
    transformStamped.child_frame_id = global_frame_;
    transformStamped.transform.translation.x = xg_;// - g_common_info_.map_center.xg;
    transformStamped.transform.translation.y = yg_;// - g_common_info_.map_center.yg;
    tf2::Quaternion q;
    // q.setRPY(0, 0, car_pgccs->angle);
    q.setRPY(0, 0, heading_ * M_PI / 180.0);
    transformStamped.transform.rotation.x = q.x();
    transformStamped.transform.rotation.y = q.y();
    transformStamped.transform.rotation.z = q.z();
    transformStamped.transform.rotation.w = q.w();
    br.sendTransform(transformStamped);

    pub_car_.publish(car_);
}

void SystemToolbar::UpdateStatus()
{
    static QString txt; 
    static int index = 0;
    static int column = 1;

    QTableWidgetItem *item;    
    static AvvsCommonInfo tmp_info;

    index = 0;
    column = 1;
//*****************************************************************************************************************//
//  车辆状态显示
//  第一列状态显示:"CAN0"<<"系统状态"<<"驾驶模式"<<"车辆速度"<<"定位状态"<<"GPS状态"<<"档位信息"<<"EPS状态"<<"VCU状态"<<"NBoost";
//*****************************************************************************************************************//
    if (g_common_info_.sensor_status.can_status[0] != tmp_info.sensor_status.can_status[0] || is_first)
    {
        item = new QTableWidgetItem;
        // txt = QString::number(g_common_info_.location_info.lon,'f',4); 
        // item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.can_status[0] == 0)
        {
            item->setBackgroundColor(fault_level_color_2_);  
            item->setText("设备异常");
        }
        else if (g_common_info_.sensor_status.can_status[0] == 1)
        {
            item->setBackgroundColor(fault_level_color_0_);  
            item->setText("设备正常");
        }
        else
        {  
            item->setText("状态未知");            
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.can_status[0] = g_common_info_.sensor_status.can_status[0]; 
    }
    index++;

    if (g_common_info_.business_status.avos_status != tmp_info.business_status.avos_status || is_first)
    {
        item = new QTableWidgetItem;
        txt = GetAvosStateInfo(g_common_info_.business_status.avos_status); 
        item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));
        if (g_common_info_.business_status.avos_status == 0)
        {
            item->setBackgroundColor(fault_level_color_2_);
            item->setText("模块缺失");
        }
        else if (g_common_info_.business_status.avos_status == 1)
        {
            item->setBackgroundColor(fault_level_color_0_);
            item->setText("系统正常");
        } else {
            item->setText("状态未知");
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.business_status.avos_status = g_common_info_.business_status.avos_status; 
    }
    index++;

    if (g_common_info_.control_status.vehicle_sys_status != tmp_info.control_status.vehicle_sys_status || is_first)
    {
        item = new QTableWidgetItem;
        txt = GetVehicleStateInfo(g_common_info_.control_status.vehicle_sys_status);
        item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.control_status.vehicle_sys_status = g_common_info_.control_status.vehicle_sys_status; 
    }
    index++;

    if (g_common_info_.location_info.velocity != tmp_info.location_info.velocity || is_first)
    {
        item = new QTableWidgetItem;
        txt = QString::number(g_common_info_.location_info.velocity,'f',4); 
        item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.location_info.velocity = g_common_info_.location_info.velocity; 
    }
    index++;

    if (g_common_info_.location_info.location_status != tmp_info.location_info.location_status || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));
        txt = GetLocationStateInfo(g_common_info_.location_info.location_status);
        item->setText(txt);

        if (g_common_info_.location_info.location_status == 0)
        {
            item->setBackgroundColor(fault_level_color_0_);  
        }
        else if(g_common_info_.location_info.location_status == 2)
        {
            item->setBackgroundColor(fault_level_color_2_);  
        }
        else
        {
            item->setBackgroundColor(fault_level_color_1_);  
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.location_info.location_status = g_common_info_.location_info.location_status; 
    }
    index++;

    if (g_common_info_.sensor_gps.status != tmp_info.sensor_gps.status || is_first)
    {
        item = new QTableWidgetItem;
        txt = GetGpsStateInfo(g_common_info_.sensor_gps.status); 
        item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_gps.status = g_common_info_.sensor_gps.status; 
    }
    index++;

    if (g_common_info_.control_status.shiftposition != tmp_info.control_status.shiftposition || is_first)
    {
        item = new QTableWidgetItem;
        txt = GetShiftVlpositionStateInfo(g_common_info_.control_status.shiftposition); 
        item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.control_status.shiftposition = g_common_info_.control_status.shiftposition; 
    }
    index++;


    if (g_common_info_.sensor_status.eps_status != tmp_info.sensor_status.eps_status || is_first)
    {
        item = new QTableWidgetItem;
        txt = GetEpsStateInfo(g_common_info_.sensor_status.eps_status); 
        if (g_common_info_.sensor_status.eps_status == 0)
        {
            item->setBackgroundColor(fault_level_color_0_);  
        }
        else
        {
            item->setBackgroundColor(fault_level_color_2_);              
        }
        item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.eps_status = g_common_info_.sensor_status.eps_status; 
    }
    index++;

    if (g_common_info_.sensor_status.vcu_status != tmp_info.sensor_status.vcu_status || is_first)
    {
        item = new QTableWidgetItem;
        // txt = GetVCUStateInfo(g_common_info_.sensor_status.vcu_status); 
        if (g_common_info_.sensor_status.vcu_status == 0)
        {
            item->setText("设备正常");
            item->setBackgroundColor(fault_level_color_0_);  
        }
        else
        {
            item->setText("设备异常");     
            item->setBackgroundColor(fault_level_color_2_);         
        }
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.vcu_status = g_common_info_.sensor_status.vcu_status; 
    }
    index++;

    if (g_common_info_.sensor_status.nboost_status != tmp_info.sensor_status.nboost_status || is_first)
    {
        item = new QTableWidgetItem;
        txt = GetNboostStateInfo(g_common_info_.sensor_status.nboost_status); 
        item->setText(txt);
        if (g_common_info_.sensor_status.nboost_status <= 1)
        {
            item->setBackgroundColor(fault_level_color_0_);  
        }
        else if (g_common_info_.sensor_status.nboost_status <= 3)
        {
            item->setBackgroundColor(fault_level_color_1_);  
        }
        else
        {
            item->setBackgroundColor(fault_level_color_2_);  
        }
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.nboost_status = g_common_info_.sensor_status.nboost_status; 
    }
    index++;

//*****************************************************************************************************************//
//  车辆状态显示
//  第二列状态显示<<"CAN1"<<"电池电量"<<"左转向灯"<<"右转向灯"<<"近光灯"<<"远光灯"<<"红绿灯"<<"V2V"<<"系统版本"<<"VIN";
//*****************************************************************************************************************//
    column = 3;
    index = 0;

    if (g_common_info_.sensor_status.can_status[1] != tmp_info.sensor_status.can_status[1] || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.can_status[1] == 0)
        {
            item->setBackgroundColor(fault_level_color_2_);  
            item->setText("设备异常");
        }
        else if (g_common_info_.sensor_status.can_status[1] == 1)
        {
            item->setBackgroundColor(fault_level_color_0_);  
            item->setText("设备正常");
        }
        else
        {
            item->setText("状态未知");            
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.can_status[1] = g_common_info_.sensor_status.can_status[1]; 
    }
    index++;

    if (g_common_info_.sensor_status.battery_soc != tmp_info.sensor_status.battery_soc || is_first)
    {
        item = new QTableWidgetItem;
        txt = QString("%1").arg(g_common_info_.sensor_status.battery_soc);
        item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.battery_soc = g_common_info_.sensor_status.battery_soc; 
    }
    index++;

    if (g_common_info_.sensor_status.turn_left_light_status != tmp_info.sensor_status.turn_left_light_status || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.turn_left_light_status == 1)
        {
            item->setText("左转向灯开");
        }
        else if(g_common_info_.sensor_status.turn_left_light_status == 0)
        { 
            item->setText("左转向灯关");
        }
        else
        {
            item->setText("状态未知");
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.turn_left_light_status = g_common_info_.sensor_status.turn_left_light_status; 
    }
    index++;

    if (g_common_info_.sensor_status.turn_right_light_status != tmp_info.sensor_status.turn_right_light_status || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.turn_right_light_status == 1)
        { 
            item->setText("右转向灯开");
        }
        else if(g_common_info_.sensor_status.turn_right_light_status == 0)
        {
            item->setText("右转向灯关");
        }
        else
        {
            item->setText("状态未知");
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.turn_right_light_status = g_common_info_.sensor_status.turn_right_light_status; 
    }
    index++;


    if (g_common_info_.sensor_status.head_lamp_low_beam_status != tmp_info.sensor_status.head_lamp_low_beam_status || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.head_lamp_low_beam_status == 1)
        {
            item->setText("近光灯开");
        }
        else if(g_common_info_.sensor_status.head_lamp_low_beam_status == 0)
        {
            item->setText("近光灯关");
        }
        else
        {
            item->setText("状态未知");
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.head_lamp_low_beam_status = g_common_info_.sensor_status.head_lamp_low_beam_status; 
    }
    index++;
    

    if (g_common_info_.sensor_status.head_lamp_high_beam_status != tmp_info.sensor_status.head_lamp_high_beam_status || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.head_lamp_high_beam_status == 1)
        {
            item->setText("远光灯开");
        }
        else if(g_common_info_.sensor_status.head_lamp_high_beam_status == 0)
        {
            item->setText("远光灯关");
        }
        else
        {
            item->setText("状态未知");
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.head_lamp_high_beam_status = g_common_info_.sensor_status.head_lamp_high_beam_status; 
    }
    index++;
    //0:红;1:黄;2:绿;-1:状态未知;
    if (g_common_info_.sensor_status.traffic_light != tmp_info.sensor_status.traffic_light || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.traffic_light == 1)
        {
            item->setBackgroundColor(fault_level_color_2_);  
            item->setText("红灯");
        }
        else if(g_common_info_.sensor_status.traffic_light == 3)
        {
            item->setBackgroundColor(fault_level_color_0_);  
            item->setText("绿灯");
        }
        else if(g_common_info_.sensor_status.traffic_light == 2)
        {
            item->setBackgroundColor(fault_level_color_1_);  
            item->setText("黄灯");
        }
        else
        {
            item->setText("状态未知");
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.traffic_light = g_common_info_.sensor_status.traffic_light; 
    }
    index++;

    if (g_common_info_.sensor_status.v2v_status != tmp_info.sensor_status.v2v_status || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.v2v_status == 1)
        {
            item->setBackgroundColor(fault_level_color_2_);  
            item->setText("停车让行");
        }
        else if(g_common_info_.sensor_status.v2v_status == 3)
        {
            item->setBackgroundColor(fault_level_color_0_);  
            item->setText("可以通行");
        }
        else
        {
            item->setText("状态未知");
        }

        show_stat_table->setItem(index, column, item);    
        tmp_info.sensor_status.v2v_status = g_common_info_.sensor_status.v2v_status; 
    }
    index++;

    if (g_common_info_.business_status.avos_version != tmp_info.business_status.avos_version || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.business_status.avos_version != "V1.0.0.0")
        {
            item->setBackgroundColor(fault_level_color_1_);   
            item->setText("非正式版本"); 
        }
        else
        {
            item->setBackgroundColor(fault_level_color_0_);    
            item->setText("正式版本");      
        }

        show_stat_table->setItem(index, column, item);  
        tmp_info.business_status.avos_version = g_common_info_.business_status.avos_version;   
    }
    index++;

    if (g_common_info_.business_status.vehicle_vin != tmp_info.business_status.vehicle_vin || is_first)
    {
        item = new QTableWidgetItem;
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.business_status.vehicle_vin.length() != 18)
        {
            item->setBackgroundColor(fault_level_color_1_);  
            item->setText("VIN错误");
        }
        else
        {
            item->setBackgroundColor(fault_level_color_0_);   
            item->setText("VIN正常");         
        }

        show_stat_table->setItem(index, column, item);  
        tmp_info.business_status.vehicle_vin = g_common_info_.business_status.vehicle_vin;   
    }
    index++;

//******************************************************************************************************************************************//
//传感器状态显示
//"激光雷达"<<"组合导航"<<"前左毫米波"<<"前中毫米波"<<"前右毫米波"<<"后左毫米波"<<"后右毫米波"<<"高清摄像头"<<"前摄像头"<<"左摄像头"<<"后摄像头"<<"右摄像头";
//******************************************************************************************************************************************//
    index = 0;
    column = 1;
    if (g_common_info_.sensor_status.lidar_status != tmp_info.sensor_status.lidar_status || is_first)
    {
        item = new QTableWidgetItem;
        // txt = QString::number(g_common_info_.location_info.lon,'f',4); 
        // item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.lidar_status == 0)
        {
            item->setBackgroundColor(fault_level_color_2_);  
            item->setText("设备异常");
        }
        else if (g_common_info_.sensor_status.lidar_status == 1)
        {
            item->setBackgroundColor(fault_level_color_0_);  
            item->setText("设备正常");
        }
        else
        { 
            item->setText("状态未知");            
        }

        sensor_table_->setItem(index, column, item);    
        tmp_info.sensor_status.lidar_status = g_common_info_.sensor_status.lidar_status; 
    }
    index++;

    if (g_common_info_.sensor_status.nav_status != tmp_info.sensor_status.nav_status || is_first)
    {
        item = new QTableWidgetItem;
        // txt = QString::number(g_common_info_.location_info.lon,'f',4); 
        // item->setText(txt);
        item->setForeground(QBrush(text_color_));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont( "Times", 10, QFont::Black));

        if (g_common_info_.sensor_status.nav_status == 0)
        {
            item->setBackgroundColor(fault_level_color_2_);  
            item->setText("设备异常");
        }
        else if(g_common_info_.sensor_status.nav_status == 1)
        {
            item->setBackgroundColor(fault_level_color_0_);  
            item->setText("设备正常");
        }
        else
        { 
            item->setText("状态未知");            
        }

        sensor_table_->setItem(index, column, item);    
        tmp_info.sensor_status.nav_status = g_common_info_.sensor_status.nav_status; 
    }
    index++;

    for (int i = 0; i < 5; ++i)     // 毫米波雷达
    {
        if (g_common_info_.sensor_status.mill_radar[i] != tmp_info.sensor_status.mill_radar[i] || is_first)
        {
            item = new QTableWidgetItem;
            // txt = QString::number(g_common_info_.location_info.lon,'f',4); 
            // item->setText(txt);
            item->setForeground(QBrush(text_color_));
            item->setTextAlignment(Qt::AlignCenter);
        	item->setFont(QFont( "Times", 10, QFont::Black));

            if (g_common_info_.sensor_status.mill_radar[i] == 0)
            {
                item->setBackgroundColor(fault_level_color_2_);  
                item->setText("设备异常");
            }
            else if (g_common_info_.sensor_status.mill_radar[i] == 1)
            {
                item->setBackgroundColor(fault_level_color_0_);  
                item->setText("设备正常");
            }
            else
            {
                item->setText("状态未知");            
            }
            sensor_table_->setItem(index, column, item);    
            tmp_info.sensor_status.mill_radar[i] = g_common_info_.sensor_status.mill_radar[i]; 
        }
        index++;
    }

    for (int i = 0; i < 5; ++i)
    {
        if (g_common_info_.sensor_status.camera_status[i] != tmp_info.sensor_status.camera_status[i] || is_first)
        {
            item = new QTableWidgetItem;
            // txt = QString::number(g_common_info_.location_info.lon,'f',4); 
            // item->setText(txt);
            item->setForeground(QBrush(text_color_));
            item->setTextAlignment(Qt::AlignCenter);
        	item->setFont(QFont( "Times", 10, QFont::Black));



            if (g_common_info_.sensor_status.camera_status[i] == 0)
            {
                item->setBackgroundColor(fault_level_color_2_);  
                item->setText("设备异常");
            }
            else if (g_common_info_.sensor_status.camera_status[i] == 1)
            {
                item->setBackgroundColor(fault_level_color_0_);  
                item->setText("设备正常");
            }
            else
            {
                item->setText("状态未知");            
            }

            sensor_table_->setItem(index, column, item);    
            tmp_info.sensor_status.camera_status[i] = g_common_info_.sensor_status.camera_status[i]; 
        }
        index++;
    }

//*****************************************************************************************************************//
//传感器状态显示
//"右侧前方"<<"右侧后中"<<"右侧后方"<<"后方右侧"<<"后方中间"<<"后方左侧"<<"左侧后方"<<"左侧前中"<<"左侧前方"<<"前方左侧"<<"前方中间"<<"前方右侧";
//*****************************************************************************************************************//
    column = 3;
    index = 0;    
    for (int i = 0; i < 12; ++i)
    {
        if (g_common_info_.sensor_status.ult_radar[i] != tmp_info.sensor_status.ult_radar[i] || is_first)
        {
            item = new QTableWidgetItem;
            // txt = QString::number(g_common_info_.location_info.lon,'f',4); 
            // item->setText(txt);
            item->setForeground(QBrush(text_color_));
            item->setTextAlignment(Qt::AlignCenter);
        	item->setFont(QFont( "Times", 10, QFont::Black));


            if (g_common_info_.sensor_status.ult_radar[i] == 2)
            {
                item->setBackgroundColor(fault_level_color_2_);  
                item->setText("严重故障");
            }
            else if (g_common_info_.sensor_status.ult_radar[i] == 0)
            {
                item->setBackgroundColor(fault_level_color_0_);  
                item->setText("设备正常");
            }
            else if (g_common_info_.sensor_status.ult_radar[i] == 1)
            {
                item->setBackgroundColor(fault_level_color_0_);  
                item->setText("即时故障");
            }
            else
            {
                item->setText("状态未知");            
            }

            sensor_table_->setItem(index, column, item);    
            tmp_info.sensor_status.ult_radar[i] = g_common_info_.sensor_status.ult_radar[i]; 
        }
        index++;
    }

    is_first = false;
}


/**********************************************
 * Function: UpdateDashboardStatus()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void SystemToolbar::UpdateDashboardStatus()
{
    static AvvsCommonInfo avvs_temp;
    // static int turn_status_temp = 0;
    // static int temprature_temp = 0;

    bool is_update = false;
    if (avvs_temp.control_status.shiftposition != g_common_info_.control_status.shiftposition)
    {
        SetValue(SHIFT_POSITION,g_common_info_.control_status.shiftposition);
        avvs_temp.control_status.shiftposition = g_common_info_.control_status.shiftposition;
        is_update = true;
    }

    if (avvs_temp.business_status.intensity4g != g_common_info_.business_status.intensity4g)
    {
        SetValue(SIGNAL,g_common_info_.business_status.intensity4g);
        avvs_temp.business_status.intensity4g = g_common_info_.business_status.intensity4g;
        is_update = true;
    }

    if (avvs_temp.location_info.location_status != g_common_info_.location_info.location_status)
    {
        SetValue(LOCATION_STATUS,g_common_info_.location_info.location_status);
        avvs_temp.location_info.location_status = g_common_info_.location_info.location_status;
        is_update = true;
    }

    if (avvs_temp.control_status.vehicle_sys_status != g_common_info_.control_status.vehicle_sys_status)
    {
        SetValue(VEHICLE_STATUS,g_common_info_.control_status.vehicle_sys_status);
        avvs_temp.control_status.vehicle_sys_status = g_common_info_.control_status.vehicle_sys_status;
        is_update = true;
    }

    if (avvs_temp.sensor_status.battery_soc != g_common_info_.sensor_status.battery_soc)
    {
        SetValue(BATTERY_VALUE,g_common_info_.sensor_status.battery_soc);//int 0-100
        avvs_temp.sensor_status.battery_soc = g_common_info_.sensor_status.battery_soc;
        is_update = true;
    }

    if (avvs_temp.location_info.velocity != g_common_info_.location_info.velocity)
    {
        SetValue(SPEED,3.6*g_common_info_.location_info.velocity);//int 0-220km/h
        avvs_temp.location_info.velocity = g_common_info_.location_info.velocity;
        is_update = true;
    }

    if (avvs_temp.control_status.lon_targettorque != g_common_info_.control_status.lon_targettorque)
    {
        SetValue(TARGET_TORQUE,8*g_common_info_.control_status.lon_targettorque);
        avvs_temp.control_status.lon_targettorque = g_common_info_.control_status.lon_targettorque;
        is_update = true;
    }

    // if (avvs_temp.battery_info.battery_soc != g_common_info_.battery_info.battery_soc)
    // {
    //     SetValue(DRIVING_MILEAGE,3*g_common_info_.battery_info.battery_soc);//int
    //     avvs_temp.battery_info.battery_soc = g_common_info_.battery_info.battery_soc;
    //     is_update = true;
    // }

    // if (avvs_temp.battery_info.bms_warning_level != g_common_info_.battery_info.bms_warning_level)
    // {
    //     SetValue(BATTERY_LIGHT,g_common_info_.battery_info.bms_warning_level);//int 0:关，1：开
    //     avvs_temp.battery_info.bms_warning_level = g_common_info_.battery_info.bms_warning_level;
    //     is_update = true;
    // }

    if (avvs_temp.control_status.brakelight != g_common_info_.control_status.brakelight)
    {
        SetValue(BRAKE_LIGHT,g_common_info_.control_status.brakelight);
        avvs_temp.control_status.brakelight = g_common_info_.control_status.brakelight;
        is_update = true;
    }

    // if (avvs_temp.centre_status.tx2_temprature != g_common_info_.centre_status.tx2_temprature)
    // {
    //     SetValue(TEMPERATURE,g_common_info_.centre_status.tx2_temprature);//int 
    //     avvs_temp.centre_status.tx2_temprature = g_common_info_.centre_status.tx2_temprature;
    //     is_update = true;
    // }

    // if (avvs_temp.centre_status.rundistance != g_common_info_.centre_status.rundistance)
    // {
    //     SetValue(TOTAL_DISTANCE,g_common_info_.centre_status.rundistance/1000);
    //     avvs_temp.centre_status.rundistance = g_common_info_.centre_status.rundistance;
    //     is_update = true;
    // }

    // if (avvs_temp.avvs_status.is_open_link != g_common_info_.avvs_status.is_open_link)
    // {
    //     SetValue(ON_LINE_STATUS,g_common_info_.avvs_status.is_open_link);//int 
    //     avvs_temp.avvs_status.is_open_link = g_common_info_.avvs_status.is_open_link;
    //     is_update = true;
    // }

    // if (avvs_temp.vehicle_info.head_light_state != g_common_info_.vehicle_info.head_light_state)
    // {
    //     SetValue(LIGHT_STATUS,g_common_info_.vehicle_info.head_light_state);
    //     avvs_temp.vehicle_info.head_light_state = g_common_info_.vehicle_info.head_light_state;
    //     is_update = true;
    // }

    // if (avvs_temp.vehicle_info.lidar_switch_state != g_common_info_.vehicle_info.lidar_switch_state)
    // {
    //     SetValue(LIDAR_STATUS,g_common_info_.vehicle_info.lidar_switch_state);
    //     avvs_temp.vehicle_info.lidar_switch_state = g_common_info_.vehicle_info.lidar_switch_state;
    //     is_update = true;
    // }

    // if (avvs_temp.vehicle_info.AEB_enablestate != g_common_info_.vehicle_info.AEB_enablestate)
    // {
    //     SetValue(AEB_ENABLE,g_common_info_.vehicle_info.AEB_enablestate);//
    //     avvs_temp.vehicle_info.AEB_enablestate = g_common_info_.vehicle_info.AEB_enablestate;
    //     is_update = true;
    // }

    // if (avvs_temp.monitor_info.fault_level != g_common_info_.monitor_info.fault_level)
    // {
    //     SetValue(WARNING,g_common_info_.monitor_info.fault_level);//0,1  
    //     avvs_temp.monitor_info.fault_level = g_common_info_.monitor_info.fault_level;
    //     is_update = true;
    // }
    // // SetValue(BELT_STATUS,1);
    // // SetValue(DOOR_STATUS,1);
    // int turn_status = 0;

    // if (g_common_info_.vehicle_info.turnlightleftstate)
    // {
    //     turn_status = -1;
    // }
    // else if (g_common_info_.vehicle_info.turnlightrightstate)
    // {
    //     turn_status = 1;
    // }    
    // else if (g_common_info_.vehicle_info.doubleflashlightstate )
    // {
    //     turn_status = 2;
    // }
    // else
    // {
    //     turn_status = 0;     
    // }

    // if (turn_status_temp != turn_status)
    // {
    //     SetValue(TURN, turn_status);//-1 ,0 ,1
    //     turn_status_temp = turn_status;
    //     is_update = true;
    // }

    // int temprature = 0;
    // if (g_common_info_.centre_status.tx2_temprature > 60)
    // {
    //     temprature = 1;
    // }
    // else
    // { 
    //     temprature = 0;     
    // }

    // if (temprature_temp != temprature)
    // {
    //     SetValue(OVER_TEMP, temprature);
    //     temprature_temp = temprature;
    //     is_update = true;
    // }

    if (is_update)
    {
        graphicsView->scene()->update();
    }

}

/**********************************************
 * Function: SetValue()
 * Author: zengzicheng
 * Date: 2019-12-04
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void SystemToolbar::SetValue(BOARD_SET_TYPE type, float value)
{
    switch(type)
    {
        case SPEED:   
        {
            if((value >= 0) && (value <= dashboard->Vitesse->getValueMax()))
            {
                dashboard->Vitesse->setValue(value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case TARGET_TORQUE:
        {
            if((value >= 0) && (value <= dashboard->CompteTours->getValueMax()))
            {
                dashboard->CompteTours->setValue(value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case BATTERY_LIGHT:  
        {
            if((value == 0) || (value == 1))
            {
                dashboard->VoyantBatterie->setValue(value);
                // graphicsView->scene()->update();
            }
        }
        break; 
        case BRAKE_LIGHT:
        {
            if((value == 0) || (value == 1))
            {
                dashboard->VoyantBatterie->setValue(BRAKE_LIGHT,value);
                // graphicsView->scene()->update();
            }
        }   
        break; 
        case TURN:
        {
            if((value >= -1) && (value <= 2))
            {
                dashboard->Clignotant->setValue(value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case LIGHT_STATUS:
        {
            if((value > 0) && (value <= 3))
            {
                dashboard->VoyantBatterie->setValue(LIGHT_STATUS,value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case WARNING://0：无警告，不显示；1:轻微警告 绿，2：一般警告，黄；3：严重警告，红
        {
            if((value >= 0) && (value <= 3))
            {
                dashboard->Warning->setValue(value);
                if (value == 3)
                {
                    dashboard->Clignotant->setValue(2);
                }
                // graphicsView->scene()->update();
            }
        }
        break;
        case ON_LINE_STATUS:
        {
            if((value == 0) || (value == 1))
            {
                dashboard->CompteurKm->setValue(ON_LINE_STATUS,value);
            }
        }
        break;
        case SIGNAL:
        {
            if((value >= 0 ) && (value <= 4))
            { 
                dashboard->CompteurKm->setValue(value);    
                // graphicsView->scene()->update();
            }
        }
        break;
        case LOCATION_STATUS:
        {
            if((value >= 0 ) && (value <= 5))
            { 
                dashboard->CompteurKm->setValue(LOCATION_STATUS,value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case SHIFT_POSITION:
        {
            if((value >= 0 ) && (value <= 6))
            { 
                dashboard->Graphicsitem->setValue(value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case TEMPERATURE:
        {
            if((value >= 0) && (value <= dashboard->Temperature->getValueMax()))
            { 
                dashboard->Temperature->setValue(value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case BATTERY_VALUE:
        {
            if((value > 0 ) && (value < 100))
            { 
                dashboard->CompteurKm->setValue(BATTERY_VALUE,value);
                // graphicsView->scene()->update();
            }            
        }
        break;
        case DRIVING_MILEAGE:
        {
            if((value > 0 ) && (value < 1000))
            { 
                dashboard->CompteurKm->setValue(DRIVING_MILEAGE,value);
                // graphicsView->scene()->update();
            }            
        }
        break;
        case VEHICLE_STATUS:
        {
            if((value >= 0 ) && (value <= 12))
            { 
                dashboard->CompteurKm->setValue(VEHICLE_STATUS,value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case OVER_TEMP:
        {
            if((value == 0) || (value == 1))
            {
                dashboard->VoyantBatterie->setValue(OVER_TEMP,value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case DOOR_STATUS:
        {
            if((value == 0) || (value == 1))
            {
                dashboard->VoyantBatterie->setValue(DOOR_STATUS,value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case BELT_STATUS:
        {
            if((value == 0) || (value == 1))
            {
                dashboard->VoyantBatterie->setValue(BELT_STATUS,value);
                // graphicsView->scene()->update();
            }
        }
        break;
        case LIDAR_STATUS:
        {
            if((value == 0) || (value == 1))
            {
                dashboard->VoyantBatterie->setValue(LIDAR_STATUS,value);
                // graphicsView->scene()->update();
            }            
        }
        break;
        case AEB_ENABLE:
        {
            if((value == 0) || (value == 1))
            {
                dashboard->VoyantBatterie->setValue(AEB_ENABLE,value);
                // graphicsView->scene()->update();
            }             
        }
        break;
        case TOTAL_DISTANCE:
        {
            if(value >= 0)
            { 
                dashboard->CompteurKm->setValue(TOTAL_DISTANCE,value);
                // graphicsView->scene()->update();
            }            
        }
        break;
        default:
        break;
    }
}


void SystemToolbar::GetCarCells() {
    const float dis = 0.2;
    const float front = 3.2;
    const float back = -1.0;
    const float left = 0.8;
    const float right = -0.8;
    float x1 = front;
    float x2 = back;
    for (float y = right; y < left; y += dis) {
        pcl::PointXYZL point;
        point.x = x1;
        point.y = y;
        point.z = -0.2;
        gcar_layout.inner_points.push_back((point));
        point.x = x2;
        gcar_layout.inner_points.push_back((point));
    }
    float y1 = left;
    float y2 = right;
    for (float x = back + dis; x < front; x += dis) {
        pcl::PointXYZL point;
        point.x = x;
        point.y = y1;
        point.z = -0.2;
        gcar_layout.inner_points.push_back((point));
        point.y = y2;
        gcar_layout.inner_points.push_back((point));
    }
    y1 = 0;
    for (float x = 0 + dis; x < front; x += dis) {
        pcl::PointXYZL point;
        point.x = x;
        point.y = y1;
        point.z = -0.2;
        gcar_layout.inner_points.push_back((point));
    }
}


void SystemToolbar::CallbackMapengine(const mapengine_msgs::Navigation::ConstPtr &msg) {
    if (msg->map_info.currentlanelists.empty()) {
        return;
    }
    if (mapengine_list_.size() > 5) {
        mapengine_list_.erase(mapengine_list_.begin());
    }
    mapengine_list_.push_back(*msg);
}

void SystemToolbar::CallbackTrajectory(const planning_msgs::Trajectory::ConstPtr&  msg) {
    gtrajectory.inner_points = msg->points;
    planning_msgs::TrajectoryPoint point;
    point.x = -5;
    point.y = -5;
    if (msg->points.size() == 0) {
        point.velocity = 0.0;
    } else {
        point.velocity = msg->points[0].velocity;
    }
    gtrajectory_text.inner_text.clear();
    gtrajectory_text.inner_text.push_back(point);
    pathplan_time_ = msg->header.time_stamp;

    path_marker_.header.frame_id = global_frame_;
    path_marker_.ns = "ShowMotionPath";
    path_marker_.points.clear();
    path_marker_.colors.clear();

    path_marker_.header.stamp = ros::Time::now();
    path_marker_.lifetime = ros::Duration();

    path_marker_.id = 0;
    path_marker_.type = visualization_msgs::Marker::LINE_STRIP;
    path_marker_.action = visualization_msgs::Marker::ADD;

    path_marker_.scale.x = 1.0;

    path_marker_.pose.position.z = 0;
    for (unsigned int i = 0; i < msg->points.size(); i++) {
      geometry_msgs::Point p;
      p.x = msg->points[i].x;
      p.y = msg->points[i].y;
      path_marker_.points.push_back(p);
      path_marker_.colors.push_back(getColor(msg->points[i].velocity));
    }

    pub_path_marker_.publish(path_marker_);

}

void SystemToolbar::CallbackPerceptionDebug(const perception_msgs::PerceptionObjects::ConstPtr &msg) {
    gobjectfusion_debug.inner_sphere.clear();
    gobjectfusion_debug.inner_sphere.reserve(msg->objs.size());
    for (int i = 0; i < msg->objs.size(); i++) {
        CubeSphereObject object;
        object.x = msg->objs[i].x;
        object.y = msg->objs[i].y;
        object.z = 0;
        object.width = msg->objs[i].width < 0.5 ? 0.5 : msg->objs[i].width;
        object.length = msg->objs[i].length < 0.5 ? 0.5 : msg->objs[i].length;
        object.height = msg->objs[i].height < 0.5 ? 0.5 : msg->objs[i].height;
        PerceptionDebugAddition *debug = (PerceptionDebugAddition *)object.addition_message;
        debug->source = msg->objs[i].source;
        gobjectfusion_debug.inner_sphere.push_back(object);
    }
}


geometry_msgs::Point SystemToolbar::ConvertPoint(const Eigen::Vector3f& point) {
    geometry_msgs::Point point_msg;
    point_msg.x = point.x();
    point_msg.y = point.y();
    point_msg.z = point.z();
    return point_msg;
}

visualization_msgs::MarkerArray SystemToolbar::VizObjectBBox( const std::vector<CubeSphereObject>& obstacles) {
    visualization_msgs::MarkerArray marker_array;
    int obs_count = 0;
    for (const auto& obs : obstacles) {
        visualization_msgs::Marker marker;
        marker.header.frame_id = global_frame_;
        marker.header.stamp = ros::Time::now();
        marker.ns = "fusion_bbox";
        marker.id = obs_count++;
        marker.type = visualization_msgs::Marker::LINE_LIST;
        marker.action = visualization_msgs::Marker::ADD;
        marker.lifetime = ros::Duration(0.1);
        marker.scale.x = 0.1;
        marker.scale.y = 0.1;
        marker.scale.z = 0.1;
        marker.color.r = 1.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;
        marker.points.clear();
        marker.colors.clear();

        std::vector<Eigen::Vector3f> geo_bbox;
        geo_bbox.resize(10);
        Eigen::Vector3f min_pt(-obs.length / 2.0, -obs.width / 2.0,
                               -obs.height / 2.0);
        Eigen::Vector3f max_pt(obs.length / 2.0, obs.width / 2.0,
                               obs.height / 2.0);
        geo_bbox[0] = Eigen::Vector3f(min_pt[0], min_pt[1], min_pt[2]);
        geo_bbox[1] = Eigen::Vector3f(min_pt[0], min_pt[1], max_pt[2]);
        geo_bbox[2] = Eigen::Vector3f(max_pt[0], min_pt[1], min_pt[2]);
        geo_bbox[3] = Eigen::Vector3f(max_pt[0], min_pt[1], max_pt[2]);
        geo_bbox[4] = Eigen::Vector3f(max_pt[0], max_pt[1], min_pt[2]);
        geo_bbox[5] = Eigen::Vector3f(max_pt[0], max_pt[1], max_pt[2]);
        geo_bbox[6] = Eigen::Vector3f(min_pt[0], max_pt[1], min_pt[2]);
        geo_bbox[7] = Eigen::Vector3f(min_pt[0], max_pt[1], max_pt[2]);
        geo_bbox[8] = Eigen::Vector3f(0, 0, min_pt[2]);
        geo_bbox[9] = Eigen::Vector3f(obs.length / 2.0, 0, min_pt[2]);
        // transform geometry bbox from body frame to sensor frame
        Eigen::Matrix3f project =
            Eigen::AngleAxis<float>((obs.heading * 3.1415926 / 180.0), Eigen::Matrix<float, 3, 1>::UnitZ())
            .matrix();
        for (size_t i = 0; i < geo_bbox.size(); ++i) {
            geo_bbox[i] = project * geo_bbox[i];
            geo_bbox[i][0] = geo_bbox[i][0] + obs.x;
            geo_bbox[i][1] = geo_bbox[i][1] + obs.y;
            geo_bbox[i][2] = geo_bbox[i][2] + obs.z;
        }

        std_msgs::ColorRGBA this_color;
        this_color.r = 1.0;
        this_color.g = 1.0;
        this_color.b = 0.0;
        this_color.a = 1.0;
        // add pair points to the line message
        // BOTTOM lines
        marker.points.push_back(ConvertPoint(geo_bbox[0]));
        marker.points.push_back(ConvertPoint(geo_bbox[2]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[2]));
        marker.points.push_back(ConvertPoint(geo_bbox[4]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[4]));
        marker.points.push_back(ConvertPoint(geo_bbox[6]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[6]));
        marker.points.push_back(ConvertPoint(geo_bbox[0]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        // MIDDLE lines
        marker.points.push_back(ConvertPoint(geo_bbox[0]));
        marker.points.push_back(ConvertPoint(geo_bbox[1]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[2]));
        marker.points.push_back(ConvertPoint(geo_bbox[3]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[4]));
        marker.points.push_back(ConvertPoint(geo_bbox[5]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[6]));
        marker.points.push_back(ConvertPoint(geo_bbox[7]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        // TOP lines
        marker.points.push_back(ConvertPoint(geo_bbox[1]));
        marker.points.push_back(ConvertPoint(geo_bbox[3]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[3]));
        marker.points.push_back(ConvertPoint(geo_bbox[5]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[5]));
        marker.points.push_back(ConvertPoint(geo_bbox[7]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker.points.push_back(ConvertPoint(geo_bbox[7]));
        marker.points.push_back(ConvertPoint(geo_bbox[1]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        // the heading line
        marker.points.push_back(ConvertPoint(geo_bbox[8]));
        marker.points.push_back(ConvertPoint(geo_bbox[9]));
        marker.colors.push_back(this_color);
        marker.colors.push_back(this_color);
        marker_array.markers.push_back(marker);
    }
    return marker_array;
}

void SystemToolbar::CallbackPerception(const mainstream_msgs::VehiclePerception::ConstPtr &msg) {
    gobjectfusion_cube.inner_cubes.clear();
    gobjectfusion_text.inner_text.clear();
    gobjectfusion_cube.inner_cubes.reserve(msg->obstacle_info.objs.size());
    gobjectfusion_text.inner_text.reserve(msg->obstacle_info.objs.size());
    std::vector<CubeSphereObject> objects;
    for (int i = 0; i < msg->obstacle_info.objs.size(); i++) {
        CubeSphereObject object;
        object.x = msg->obstacle_info.objs[i].x;
        object.y = msg->obstacle_info.objs[i].y;
        object.z = 0;
        object.width = msg->obstacle_info.objs[i].width < 0.5 ? 0.5 : msg->obstacle_info.objs[i].width;
        object.length = msg->obstacle_info.objs[i].length < 0.5 ? 0.5 : msg->obstacle_info.objs[i].length;
        object.height = msg->obstacle_info.objs[i].height < 0.5 ? 0.5 : msg->obstacle_info.objs[i].height;
        float loc_angle = msg->localizaion.pose.pose_euler.rotation.yaw.variable;
        if (loc_angle < -180) { loc_angle += 360;}
        if (loc_angle > 180) { loc_angle -= 360;}
        object.heading = msg->obstacle_info.objs[i].heading - loc_angle;
        gobjectfusion_cube.inner_cubes.push_back(object);

        PerceptionAddition *perception_addition = (PerceptionAddition *)object.addition_message;
        perception_addition->id = msg->obstacle_info.objs[i].id;
        perception_addition->vxrel = msg->obstacle_info.objs[i].vxrel;
        perception_addition->vyrel = msg->obstacle_info.objs[i].vyrel;
        perception_addition->speed = msg->obstacle_info.objs[i].speed;
        perception_addition->type = msg->obstacle_info.objs[i].type;
        gobjectfusion_text.inner_text.push_back(object);

        objects.push_back(object);
    }

    // visualize object bounding box
    obstacle_bbox = VizObjectBBox(objects);

    gstatic_cell.inner_points = msg->obstacle_info.cells;
    gobject_cell.inner_points.clear();
    for (auto &obj : msg->obstacle_info.objs) {
        gobject_cell.inner_points.insert(gobject_cell.inner_points.end(), obj.cells.begin(), obj.cells.end()) ;
    }
}

void SystemToolbar::MakeLane(string str_namespace , visualization_msgs::Marker &output, LaneParameter param) {
    const double inner_gap_ = 1.0;
    const double max_length_ = 60.0;
    output.header.frame_id = global_frame_;
    output.header.stamp = ros::Time::now();
    output.lifetime =  ros::Duration();
    output.ns = str_namespace;
    output.id = 0;
    output.points.clear();
    output.type = visualization_msgs::Marker::LINE_LIST;
    switch (param.Lanetype) {
    case 0: //DASHED
        output.type = visualization_msgs::Marker::LINE_LIST;
        break;
    case 1: //SOLID
        output.type = visualization_msgs::Marker::LINE_STRIP;
        break;
    case 2: //None

        break;
    default:
        output.type = visualization_msgs::Marker::LINE_STRIP;
        break;
    }

    output.action =  visualization_msgs::Marker::ADD;

    output.color.r = 1.0;
    output.color.g = 1.0;
    output.color.b = 1.0;

    output.color.a = 1.0;

    output.scale.x = 0.2;
    output.scale.y = 0.2;
    output.scale.z = 0.2;

    output.pose.position.z = 3;

    output.points.clear();
    std::cout << "param.Lanetype:" << param.Lanetype;
    std::cout << " list:" << visualization_msgs::Marker::LINE_LIST << std::endl;
    if (param.Lanetype == 2) {
        return;
    }

    for ( double j = 0; j < max_length_ ; j += inner_gap_ )
    {
        geometry_msgs::Point p;
        p.x = j;
        p.y = param.a * j * j + param.b * j + param.c ;
        output.points.push_back ( p );
    }
    std::cout << "points:" << output.points.size() << std::endl;
}

void SystemToolbar::MakePredictLines(std::string str_namespace, const prediction_msgs::PredictionObjects::ConstPtr &objects,
                      visualization_msgs::MarkerArray &output_lines) {
    int i = 0, temp_num = 0;
    for (auto const& object : objects->prediction_obstacle) {
        int j = 0;
        if (object.trajectories.size() <= 0) {
            continue;
        }
        for (auto const& lane : object.trajectories) {
            int id = temp_num;
            if (id >= output_lines.markers.size()) {
                visualization_msgs::Marker output;
                output.header.frame_id = global_frame_;
                output.header.stamp = ros::Time::now();
                output.lifetime =  ros::Duration();
                output.ns = str_namespace;
                output.id = id;
                output.type = visualization_msgs::Marker::LINE_STRIP;
                output.action =  visualization_msgs::Marker::ADD;
                output.color.r = 0.0;
                output.color.g = 0.0;
                output.color.b = 1.0;
                output.color.a = 1.0;
                output.scale.x = 0.5;
                output.scale.y = 0.5;

                output.pose.position.z = 0.1;
                output.points.clear();
                for (auto const& point : lane.points) {
                    geometry_msgs::Point p;
                    p.x = point.x;
                    p.y = point.y;
                    output.points.push_back (p);
                }
                output.pose.orientation.w = 1.0;
                output_lines.markers.push_back(output);
            } else {
                output_lines.markers[id].header.frame_id = global_frame_;
                output_lines.markers[id].header.stamp = ros::Time::now();
                output_lines.markers[id].ns = str_namespace;
                output_lines.markers[id].id = id;
                output_lines.markers[id].lifetime = ros::Duration();
                output_lines.markers[id].color.g = 0.0;
                output_lines.markers[id].color.b = 1.0;
                output_lines.markers[id].color.r = 0.0;
                output_lines.markers[id].color.a = 1.0;
                output_lines.markers[id].scale.x = 0.5;
                output_lines.markers[id].scale.y = 0.5;
                output_lines.markers[id].type = visualization_msgs::Marker::LINE_STRIP;
                output_lines.markers[id].action = visualization_msgs::Marker::ADD;

                output_lines.markers[id].pose.position.z = 0.1;
                output_lines.markers[id].points.clear();
                output_lines.markers[id].colors.clear();
                for (auto const& point : lane.points) {
                    geometry_msgs::Point p;
                    p.x = point.x;
                    p.y = point.y;
                    output_lines.markers[id].points.push_back (p);
                }
                output_lines.markers[id].pose.orientation.w = 1.0;
            }
            j++; temp_num++;
        }
        i++;
    }
    for (int id_more = temp_num; id_more < output_lines.markers.size(); id_more++) {
        output_lines.markers[id_more].action = visualization_msgs::Marker::DELETE;
    }
}

void SystemToolbar::CallbackPredictionObjects(const prediction_msgs::PredictionObjects::ConstPtr &objects) {
    if (objects->prediction_obstacle.size() <= 0)
    {
        return;
    }
    MakePredictLines("prediction_trajectory", objects, prediction_trajectory_array_);
}

void SystemToolbar::TimeMatching() {
    if (mapengine_list_.empty()) {
        return;
    }
    int index = mapengine_list_.size() - 1;
    double min_time = 1000.0;
    for (int i = 0; i < mapengine_list_.size(); i++) {
        double delta_time = fabs(mapengine_list_.at(i).header.time_stamp - pathplan_time_);
        if (delta_time < min_time) {
            min_time = delta_time;
            index = i;
        }
    }
    // std::cout << "TimeMatching min_time is " << min_time << ", index is " << index
              // << ", size is " << mapengine_list_.size() << std::endl;
    auto &map_info = mapengine_list_.at(index).map_info;
    mapengine_path.Reset();
    for (auto &linelist : map_info.currentlanelists) {
        for (auto &frontlane : linelist.frontlanelist) {
            mapengine_path.inner_points.insert(mapengine_path.inner_points.end(),
                                               frontlane.referenceline.begin(), frontlane.referenceline.end());
        }
    }
    left_mapengine_path.Reset();
    for (auto &linelist : map_info.leftlanelists) {
        for (auto &frontlane : linelist.frontlanelist) {
            left_mapengine_path.inner_points.insert(left_mapengine_path.inner_points.end(),
                                                    frontlane.referenceline.begin(), frontlane.referenceline.end());
        }
    }
    right_mapengine_path.Reset();
    for (auto &linelist : map_info.rightlanelists) {
        for (auto &frontlane : linelist.frontlanelist) {
            right_mapengine_path.inner_points.insert(right_mapengine_path.inner_points.end(),
                    frontlane.referenceline.begin(), frontlane.referenceline.end());
        }
    }
    rev_mapengine_path.Reset();
    for (auto &frontlane : map_info.revlanelists.frontlanelist) {
        rev_mapengine_path.inner_points.insert(rev_mapengine_path.inner_points.end(),
                    frontlane.referenceline.begin(), frontlane.referenceline.end());
    }
    for (auto &backlane : map_info.revlanelists.backlanelist) {
        rev_mapengine_path.inner_points.insert(rev_mapengine_path.inner_points.end(),
                    backlane.referenceline.begin(), backlane.referenceline.end());
    }
}

std_msgs::ColorRGBA SystemToolbar::getColor(float speed) {
  std_msgs::ColorRGBA color_rgba;
  if (fabs(speed) <= 0.001) {
    color_rgba.r = 1.0;
    color_rgba.g = 1.0;
    color_rgba.b = 1.0;
    color_rgba.a = 0.5;
    return color_rgba;
  }
  if (speed < 0) {
    color_rgba.r = 0.0;
    color_rgba.g = 0.0;
    color_rgba.b = 0.0;
    color_rgba.a = 0.5;
    return color_rgba;
  }
  if (speed >= UP_SPEED) {
    color_rgba.r = 1.0;
    color_rgba.g = 0.0;
    color_rgba.b = 0.0;
    color_rgba.a = 0.5;
    return color_rgba;
  }
  if (speed < UP_SPEED / 2) {
    color_rgba.r = speed / (UP_SPEED / 2);
    color_rgba.g = 1;
    color_rgba.b = 0.0;
    color_rgba.a = 0.5;
  } else if (speed < UP_SPEED) {
    color_rgba.r = 1;
    color_rgba.g = 2 - (speed) / (UP_SPEED / 2);
    color_rgba.b = 0.0;
    color_rgba.a = 0.5;
  } else {
    color_rgba.r = 0.0;
    color_rgba.g = 0.0;
    color_rgba.b = 0.0;
    color_rgba.a = 0.0;
  }

  return color_rgba;
}

void SystemToolbar::PublisherRvizThread()
{
    TimeMatching();

    gobjectfusion_cube.publish(pub_object_marker, 'r');
    gobjectfusion_text.publish(pub_object_text_marker, 'w');
    gobjectfusion_debug.publish(pub_object_debug_marker_, 'n');
    pub_object_bbox.publish(obstacle_bbox);
    mapengine_path.publish(pub_mapengine_marker, 'w');

    left_mapengine_path.publish(pub_left_mapengine_marker, 'y');
    right_mapengine_path.publish(pub_right_mapengine_marker, 'y');
    rev_mapengine_path.publish(pub_rev_mapengine_marker, 'r');
    gtrajectory.publish(pub_trajectory_marker, 'n');
    gtrajectory_text.publish(pub_trajectory_text_marker, 'g');

    gcar_layout.publish(pub_car_cloud_, 'w');
    pub_prediction_trajectory_.publish(prediction_trajectory_array_);
}

bool SystemToolbar::LoadBaseMap()
{
    string basemap_api_path = "";
    getPackagePath("basemap", basemap_api_path);
    Mapinfo *mapinfo = MapLoader::GetMapinfoPtr();
    std::string map_name = mapinfo->GetMapHeader().name;
    std::string basemap_dir = mapinfo->GetMapParamHeader().basemap_dir;
    basemap_api_path = basemap_api_path + "/../../map_file/" + map_name + "/" + basemap_dir;
    publish_image.Init(basemap_api_path);
    
    return true;
}

bool SystemToolbar::LoadVectorMap()
{
    PointGCCS car_pgccs;
    if (LoadVectorFile(car_pgccs) == -1) {
        qDebug()<<"LoadVectorMap Error!";
        return false;
    }
    xg_ = car_pgccs.xg;
    yg_ = car_pgccs.yg;
    heading_ = car_pgccs.angle;

    g_common_info_.location_info.fusion_pos.xpos  = car_pgccs.xg;
    g_common_info_.location_info.fusion_pos.ypos  = car_pgccs.yg;
    g_common_info_.location_info.heading          = car_pgccs.angle;
    return true;
}

void SystemToolbar::ShowBaseMap(void)
{
    publish_image.Publish(global_frame_, pub_base_map, xg_, yg_, heading_);
}

/**********************************************
 * Function: ShowVectormap()
 * Author: wanghaiping
 * Date: 2019-12-01
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void SystemToolbar::ShowVectormap(void) 
{
    show_vector_map_.Reset();
    car_pgccs_.xg = xg_;
    car_pgccs_.yg = yg_;
    car_pgccs_.angle = heading_;

    for (auto lane_info_map : glane_info_map_vec) 
    {
        for (auto it = lane_info_map->begin(); it != lane_info_map->end(); ++it) 
        {
            for (auto gccs_point : it->second.gccs_point) 
            {
                geo_tool_.GCCS2VCS(car_pgccs_, gccs_point, vsc_point_);
                show_vector_map_.inner_points.push_back(vsc_point_);
            }
        }
    }
    show_vector_map_.publish(pub_vector_map_, 'y');
}

/**
 * 根据 fault_info 话题来判断车辆传感器状态
*/
void SystemToolbar::CallbackFaultInfo(const common_msgs::FaultVec::ConstPtr &msg) {
    int needRefeash = false;
    if (msg->info_vec.size() > 0) {
        // std::cout << "fault info size: " << msg->info_vec.size() << std::endl;
        for (auto fault_info : msg->info_vec) {
            if (fault_info.module_name == "ins") {          // 组合惯导在 fualt_info 话题中的 module_name
                g_common_info_.sensor_status.nav_status = fault_info.fault_type;
                if (nav_timer->isActive())  nav_timer->stop();
                nav_timer->start(2000);
                // std::cout << "--> nav fault" << std::endl;
            } else if (fault_info.module_name == "lidar_driver") {     // 激光雷达
                g_common_info_.sensor_status.lidar_status = fault_info.fault_type;
                if (lidar_timer->isActive())  lidar_timer->stop();
                lidar_timer->start(2000);
                // std::cout << "--> lidar fault" << std::endl;
            } else if (fault_info.module_name == "radar7") {       // 前左毫米波
                g_common_info_.sensor_status.mill_radar[0] = fault_info.fault_type;
                if (fl_radar->isActive())  fl_radar->stop();
                fl_radar->start(2000);
                // std::cout << "--> fl_radar fault" << std::endl;
            } else if (fault_info.module_name == "radar0") {       // 前中毫米波
                needRefeash = true;
                g_common_info_.sensor_status.mill_radar[1] = fault_info.fault_type;
                if (fc_radar->isActive()) fc_radar->stop();
                fc_radar->start(2000);      // 重新开始计时
                // std::cout << "--> fc_radar fault" << std::endl;
            } else if (fault_info.module_name == "radar1") {       // 前右毫米波
                g_common_info_.sensor_status.mill_radar[2] = fault_info.fault_type;
                if (fr_radar->isActive())  fr_radar->stop();
                fr_radar->start(2000);
                // std::cout << "--> fr_radar fault" << std::endl;
            } else {

            }
            
        }
        if (needRefeash) {
            // this->UpdateStatus();
        }
    }  
}

/**
 * type: 
 *      normal: 把所有传感器设置为状态正常
 *      nostate: 把所有传感器设置为状态未知
*/
void SystemToolbar::InitSensorState(std::string type) {
    int sensor_val, ult_val;
    if (type == "normal") {
        sensor_val = 1;     // 对于 sensor_status 1 表示正常
        ult_val = 0;        // 对于 ult_radar 0 表示正常
    } else {
        sensor_val = 2;     // 对于 sensor_status 非0非1 都表示状态未知
        ult_val = 3;        // 对于 ult_radar 非012 都表示状态未知
    }

    g_common_info_.sensor_status.can_status[0] = sensor_val;
    g_common_info_.sensor_status.can_status[1] = sensor_val;

    g_common_info_.sensor_status.camera_status[0] = sensor_val;  // 相机
    g_common_info_.sensor_status.camera_status[1] = sensor_val;
    g_common_info_.sensor_status.camera_status[2] = sensor_val;
    g_common_info_.sensor_status.camera_status[3] = sensor_val;
    g_common_info_.sensor_status.camera_status[4] = sensor_val;

    g_common_info_.sensor_status.lidar_status = sensor_val;      // 激光雷达
    g_common_info_.sensor_status.nav_status = sensor_val;        // 组合惯导
    // 毫米波雷达
    g_common_info_.sensor_status.mill_radar[0] = sensor_val;     // 前左
    g_common_info_.sensor_status.mill_radar[1] = sensor_val;     // 前中
    g_common_info_.sensor_status.mill_radar[2] = sensor_val;     // 前右
    g_common_info_.sensor_status.mill_radar[3] = sensor_val;     // 后左
    g_common_info_.sensor_status.mill_radar[4] = sensor_val;     // 后右

    // 超声波激光雷达
    int ult_radar_length = 0;
    ult_radar_length = sizeof(g_common_info_.sensor_status.ult_radar) / sizeof(g_common_info_.sensor_status.ult_radar[0]);
    for (int i=0; i<ult_radar_length; i++) {
        g_common_info_.sensor_status.ult_radar[i] = ult_val;      // 0 表示设备正常
    }
}

void SystemToolbar::CallbackFLRadar(const perception_msgs::RadarObjectList::ConstPtr &msg) {
    // std::cout << "fl radar size is: " << msg->radarobjects.size() << std::endl;
}

void SystemToolbar::CallbackFRRadar(const perception_msgs::RadarObjectList::ConstPtr &msg) {
    // std::cout << "fr radar size is: " << msg->radarobjects.size() << std::endl;
}

void SystemToolbar::CallbackRLRadar(const perception_msgs::RadarObjectList::ConstPtr &msg) {
    // std::cout << "rl radar size is: " << msg->radarobjects.size() << std::endl;
}
