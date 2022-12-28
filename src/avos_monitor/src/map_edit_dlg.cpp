#include "map_edit_dlg.h"
#include "ui_map_edit_dlg.h"


MapEditDlg::MapEditDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapEditDlg)
{
    ui->setupUi(this);

    ui->function_combox->addItem("交通灯");
    ui->function_combox->addItem("V2V");
    ui->function_combox->addItem("停止线");

    ui->road_attribut_combox->addItem("单车道");	
    ui->road_attribut_combox->addItem("双车道");	
  
    shell_path_ = ros::package::getPath("avos_monitor") + "/resources/shell/";

    OnLoadConfigParams();
    ui->vehicle_map_path_edit->setText(QString::fromStdString(vehicle_map_path_)); 
    ui->map_rawdata_path_edit->setText(save_map_file_path_);   
    ui->import_data_path_edit->setText(gps_txt_file_path_);
    ui->local_map_bag_path_edit->setText(map_bag_file_path_);
    ui->export_map_path_edit->setText(export_map_path_);
    ui->map_name_edit->setText(map_name_);
    ui->left_side_distance_edit->setText(QString("%1").arg(left_extend_));
    ui->right_side_distance_edit->setText(QString("%1").arg(right_extand_));
    ui->point_count_edit->setText(QString::number(point_count_));

    point_color_.a = 0.8;
    point_color_.r = 0.0;
    point_color_.g = 0.0;
    point_color_.b = 1.0;

    ui->start_acq_map_btn->setEnabled(false);
    ui->add_function_btn->setEnabled(false);
}

MapEditDlg::~MapEditDlg()
{
    killTimer(m_nTimerId);
    delete ui;
}

void MapEditDlg::closeEvent(QCloseEvent *event)
{
    killTimer(m_nTimerId);
    if (is_download_map_)
    {
        QMessageBox::warning(this, "信息提示", "正在下发地图，请稍候！");
        // event->ignore();
        return;
    }

	if (is_start_recieve_gps_)
	{
		sub_sensorgps_.shutdown();
	}

	OnSaveConfigParams();
}

void MapEditDlg::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerId)
    {   //没有定位数据时需要将数据清空
        if ((ros::Time::now().toSec() - gps_topic_start_time_ > 3) && (gps_topic_hz_ > 0))
        {
            gps_topic_hz_ = 0;  
            sensor_gps_status_.Reset();
    		UpdateGpsInfo();          
        }
        ui->point_count_edit->setText(QString::number(point_count_));
    }
}

void MapEditDlg::UpdateGpsInfo()
{
	ui->gps_status_edit->setText(GetGpsStateInfo(sensor_gps_status_.status));
	ui->gps_rate_edit->setText(QString::number(sensor_gps_status_.gps_topic_hz,'f',4));
	ui->current_log_edit->setText(QString::number(sensor_gps_status_.lon,'f',9));
	ui->current_lat_edit->setText(QString::number(sensor_gps_status_.lat,'f',9));
	ui->current_heading_edit->setText(QString::number(sensor_gps_status_.heading,'f',6));
	ui->gps_state_num_edit->setText(QString("%1").arg(sensor_gps_status_.satenum));
	ui->current_heading_valid_edit->setText(QString("%1").arg(sensor_gps_status_.is_heading_valid));
	ui->current_heigth_edit->setText(QString::number(sensor_gps_status_.height,'f',9));
    ui->current_velocity_edit->setText(QString::number(sensor_gps_status_.velocity,'f',4));
}

void MapEditDlg::CallbackSensorgps(const ivlocmsg::ivsensorgps::ConstPtr &msg)
{
    static int count = 0;
    static double secs = 0;

    if (count == 0)
    {
        gps_topic_start_time_ = ros::Time::now().toSec();
    }
    else if (count == 10)
    {
        secs = ros::Time::now().toSec() - gps_topic_start_time_;
        gps_topic_hz_ = 10/secs;
        count = -1;        
    }
    count++;

    if (sensor_gps_status_.status != msg->status)
    {
    	ui->gps_status_edit->setText(GetGpsStateInfo(msg->status));
        sensor_gps_status_.status = msg->status;
    }

    if (sensor_gps_status_.gps_topic_hz != gps_topic_hz_)
    {
    	ui->gps_rate_edit->setText(QString::number(gps_topic_hz_,'f',4));
        sensor_gps_status_.gps_topic_hz = gps_topic_hz_;
    }

    if (sensor_gps_status_.lon != msg->lon)
    {
    	ui->current_log_edit->setText(QString::number(msg->lon,'f',9));
        sensor_gps_status_.lon = msg->lon;
    }

    if (sensor_gps_status_.lat != msg->lat)
    {
    	ui->current_lat_edit->setText(QString::number(msg->lat,'f',9));
        sensor_gps_status_.lat = msg->lat;
    }

    if (sensor_gps_status_.height != msg->height)
    {
    	ui->current_heigth_edit->setText(QString::number(msg->height,'f',9));
        sensor_gps_status_.height = msg->height;
    }

    if (sensor_gps_status_.heading != msg->heading)
    {
    	ui->current_heading_edit->setText(QString::number(msg->heading,'f',6));
        sensor_gps_status_.heading = msg->heading;
    }

    if (sensor_gps_status_.satenum != msg->satenum)
    {
    	ui->gps_state_num_edit->setText(QString("%1").arg(msg->satenum));
        sensor_gps_status_.satenum = msg->satenum;
    }

    if (sensor_gps_status_.is_heading_valid != msg->is_heading_valid)
    {
    	ui->current_heading_valid_edit->setText(QString("%1").arg(msg->is_heading_valid));
        sensor_gps_status_.is_heading_valid = msg->is_heading_valid;
    }

    if (sensor_gps_status_.velocity != msg->velocity)
    {
        ui->current_velocity_edit->setText(QString::number(msg->velocity,'f',4));
        sensor_gps_status_.velocity = msg->velocity;
    }

    if (acq_map_status_ == 1)
    {
		int status = -1;
		if (msg->status == 4 && msg->is_heading_valid == 1) {
			status = 0;
		}

		avos::vectormap::PointGCCS gccs;
		avos::vectormap::PointGPS gps;
		gps.lon = msg->lon;
		gps.lat = msg->lat;
		gps.heading = msg->heading;

		if (gps.Check() == false) {
			return;
		}

		geo_tool_.GPS2GCCS(gps, gccs);
		std::unique_lock<std::mutex> locker(data_update_mutex);
		double dis = hypot(last_gccs.xg - gccs.xg, last_gccs.yg - gccs.yg);
		if (dis < 100 && dis > 0.05) 
		{
            if (add_function_)
            {
                map_protocol.SetData(msg->lon, msg->lat, msg->heading, gccs.xg, gccs.yg, gccs.angle, status, function_type_);
                add_function_ = false;
            }
            else
            {
                map_protocol.SetData(msg->lon, msg->lat, msg->heading, gccs.xg, gccs.yg, gccs.angle, status, 0);                    
            }

            if (point_count_ > 1)
            {
                lane_data.push_back(map_protocol);
            }

			last_gccs = gccs;
            point_count_++;
		} 
		else 
		{
			AERROR << "position is not record " << dis;
		}

		if (dis > 100) {
			last_gccs = gccs;
		}
    }
}

void MapEditDlg::on_start_recieve_gps_btn_clicked()
{
	if (is_start_recieve_gps_)
	{
    	killTimer(m_nTimerId);
    	sub_sensorgps_.shutdown();
    	ui->start_recieve_gps_btn->setText("准备采集");
    	is_start_recieve_gps_ = false;        
        ui->start_acq_map_btn->setEnabled(false);
        ui->add_function_btn->setEnabled(false);
	}
	else
	{
    	sub_sensorgps_ = nh_.subscribe("/ivsensorgps", 100, &MapEditDlg::CallbackSensorgps, this);
    	m_nTimerId = startTimer(100);
    	ui->start_recieve_gps_btn->setText("切断数据");
    	is_start_recieve_gps_ = true;
        ui->start_acq_map_btn->setEnabled(true);
        ui->start_recieve_gps_btn->setEnabled(false);
	}
}

void MapEditDlg::on_select_save_path_btn_clicked()
{
	if (ui->map_name_edit->text().isEmpty())
	{
        QMessageBox::warning(this, "信息提示", "请给地图命名！");
		return;
	}

    QFileDialog fd(this, "选择采集地图原始数据存放路径"); 

    fd.setFileMode(QFileDialog::Directory); //FileMode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly }
    fd.setViewMode(QFileDialog::Detail);//设置视图模式 ViewMode { Detail, List }
    fd.setAcceptMode(QFileDialog::AcceptSave);//AcceptMode { AcceptOpen, AcceptSave }
    fd.setStyleSheet("QTextEdit{color:white;background-color:#555555;}"\
    "QComboBox{color:rgb(255, 255, 255);background-color:rgb(85, 87, 83);\
    border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 2px;}\
    QHeaderView::section{background-color:#666666;color: white;}");

    if (fd.exec() == QDialog::Accepted) {
        QString sel = fd.selectedFiles()[0];     
        save_map_file_path_ = sel;
        ui->map_rawdata_path_edit->setText(save_map_file_path_);   
    }else{
        return ;
    }
}

void MapEditDlg::on_start_acq_map_btn_clicked()
{
	if (is_start_acq_map_)
	{
    	ui->start_acq_map_btn->setText("开始采集");
        ui->start_acq_map_btn->setEnabled(false);
        ui->add_function_btn->setEnabled(false);
    	is_start_acq_map_ = false;
    	acq_map_status_ = 0;
        ui->start_recieve_gps_btn->setEnabled(true);
    	SaveMap();
	}
	else
	{
    	ui->start_acq_map_btn->setText("结束采集");
        ui->add_function_btn->setEnabled(true);
    	is_start_acq_map_ = true;
    	acq_map_status_ = 1;
    	lane_data.clear();
        point_count_ = 0;
        ui->point_count_edit->setText(QString::number(point_count_));
	}
}

void MapEditDlg::on_function_combox_currentIndexChanged(int index)
{
	function_type_ = index + 1;
}

void MapEditDlg::on_add_function_btn_clicked()
{
	avos::vectormap::PointGCCS gccs;
	avos::vectormap::PointGPS gps;
	gps.lon = sensor_gps_status_.lon;
	gps.lat = sensor_gps_status_.lat;
	gps.heading = sensor_gps_status_.heading;
	if (gps.Check() == false) {
        QMessageBox::warning(this, "信息提示", "GPS数据无效，请重新选择！");
		return;
	}
    add_function_ = true;
}

void MapEditDlg::on_select_import_data_btn_clicked()
{
    QFileDialog fd(this, "选择地图原始数据", "", "GPS数据(*.txt)");
    fd.setFileMode(QFileDialog::ExistingFiles); //FileMode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly }
    fd.setViewMode(QFileDialog::Detail);//设置视图模式 ViewMode { Detail, List }
    fd.setAcceptMode(QFileDialog::AcceptOpen);//AcceptMode { AcceptOpen, AcceptSave }
    fd.setStyleSheet("QTextEdit{color:white;background-color:#555555;}"\
    "QComboBox{color:rgb(255, 255, 255);background-color:rgb(85, 87, 83);\
    border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 2px;}\
    QHeaderView::section{background-color:#666666;color: white;}");

    if (fd.exec() == QDialog::Accepted) {
        QString sel = fd.selectedFiles()[0];
        ui->import_data_path_edit->setText(sel);

        if (sel.length() > 0) 
        {
        	gps_txt_file_path_ = sel;
        }
    }
}

void MapEditDlg::on_select_export_path_btn_clicked()
{
    QFileDialog fd(this, "选择地图输出路径");

    fd.setFileMode(QFileDialog::Directory); //FileMode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly }
    fd.setViewMode(QFileDialog::Detail);//设置视图模式 ViewMode { Detail, List }
    fd.setAcceptMode(QFileDialog::AcceptSave);//AcceptMode { AcceptOpen, AcceptSave }
    fd.setStyleSheet("QTextEdit{color:white;background-color:#555555;}"\
    "QComboBox{color:rgb(255, 255, 255);background-color:rgb(85, 87, 83);\
    border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 2px;}\
    QHeaderView::section{background-color:#666666;color: white;}");

    if (fd.exec() == QDialog::Accepted) {
        QString sel = fd.selectedFiles()[0];
        ui->export_map_path_edit->setText(sel);

        if (sel.length() > 0) 
        {
            export_map_path_ = sel;
        }
    }
}

void MapEditDlg::on_road_attribut_combox_currentIndexChanged(int index)
{
    road_attribut_ = index;
}

//--input_path /home/idriver/test   --output_path   /home/idriver/test --leftlane_width=2  --rightlane_width=6  --isleft_doublelane=false   --isright_doublelane=true
// bool MapEditDlg::ExportMap(std::string cmd , std::string &error)
void MapEditDlg::on_export_map_btn_clicked()
{
    if (ui->export_map_path_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "地图导出路径为空，请重新选择！");
        return;
    }

    if (ui->import_data_path_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "请选择地图原始数据文件！");
        return;
    }

    if (ui->right_side_distance_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "右边界扩展宽度为空，请设置！");
        return;
    }

    if (ui->left_side_distance_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "左边界扩展宽度为空，请设置！");
        return;
    }

    export_map_path_ = ui->export_map_path_edit->text();
    gps_txt_file_path_ = ui->import_data_path_edit->text();

    right_extand_ = ui->right_side_distance_edit->text().toFloat();
    left_extend_ = ui->left_side_distance_edit->text().toFloat();

    QString qcmd = QString("%1 %2 %3 %4 false false").arg(gps_txt_file_path_).arg(export_map_path_).arg(left_extend_).arg(right_extand_);
    std::string cmd = qcmd.toStdString();
    std::string error = "";
    qDebug() << "cmd:" << qcmd;
    if(ExportMap(cmd, error))
    {
        QMessageBox::information(this, "信息提示", "导出成功！");        
    }
    else
    {        
        QMessageBox::warning(this, "信息提示", QString("导出失败(%1)").arg(QString::fromStdString(error)));
    }
}

void MapEditDlg::on_select_map_bag_btn_clicked()
{
    // QFileDialog fd(this, "选择地图文件夹");
    QFileDialog fd(this, "选择地图文件", "", "地图数据(*.tar.gz)");
    fd.setFileMode(QFileDialog::ExistingFiles); //FileMode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly }
    fd.setViewMode(QFileDialog::Detail);//设置视图模式 ViewMode { Detail, List }
    fd.setAcceptMode(QFileDialog::AcceptOpen);//AcceptMode { AcceptOpen, AcceptSave }
    fd.setStyleSheet("QTextEdit{color:white;background-color:#555555;}"\
    "QComboBox{color:rgb(255, 255, 255);background-color:rgb(85, 87, 83);\
    border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 2px;}\
    QHeaderView::section{background-color:#666666;color: white;}");

    if (fd.exec() == QDialog::Accepted) {
        QString sel = fd.selectedFiles()[0];
        ui->local_map_bag_path_edit->setText(sel);

        if (sel.length() > 0) 
        {
        	map_bag_file_path_ = sel;
        }
    }
}

void MapEditDlg::on_download_map_to_vehicle_btn_clicked()
{
	if (ui->vehicle_map_path_edit->text().isEmpty())
	{
        QMessageBox::warning(this, "信息提示", "车端地图路径为空，请重新选择！");
        return;
	}

	if (ui->local_map_bag_path_edit->text().isEmpty())
	{
        QMessageBox::warning(this, "信息提示", "本地地图文件路径为空，请重新选择！");
        return;
	}

    // std::shared_ptr<std::thread> func_progress = std::make_shared<std::thread>(std::bind(&MapEditDlg::OnScpMapToVehicle, this));
    // func_progress->join();
    OnScpMapToVehicle();
}

void MapEditDlg::on_vehicle_map_path_edit_textChanged(const QString &arg1)
{
    vehicle_map_path_ = arg1.toStdString();
}


void MapEditDlg::SaveMap()
{
    if (ui->map_rawdata_path_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "地图保存路径为空，请重新选择！");
        return;
    }

    if (ui->map_name_edit->text().isEmpty())
    {
        QMessageBox::warning(this, "信息提示", "地图名为空，请先给地图命名！");
        return;
    }

    QString map_file_path = ui->map_rawdata_path_edit->text() + "/" + ui->map_name_edit->text() + ".txt";

	SerializationStructVector<XjMapProtocol>(lane_data, map_file_path.toStdString());
}

void MapEditDlg::OnLoadConfigParams()
{    
    try
    {
        QSettings map_edit_setting("map_edit_setting","avos_monitor");
        vehicle_map_path_ = map_edit_setting.value("vehicle_map_path","").toString().toStdString();
        save_map_file_path_ = map_edit_setting.value("save_map_path","").toString();
        gps_txt_file_path_ = map_edit_setting.value("map_data_path","").toString();
        map_bag_file_path_ = map_edit_setting.value("map_bag_local_path","").toString();
        export_map_path_ = map_edit_setting.value("export_map_path","").toString();
        left_extend_ = map_edit_setting.value("left_extend","2").toFloat();
        right_extand_ = map_edit_setting.value("right_extand","2").toFloat();
        map_name_ = map_edit_setting.value("map_name","map").toString();
    }
    catch(...)
    {
    }
}

void MapEditDlg::OnSaveConfigParams()
{
    try
    {
        QSettings map_edit_setting("map_edit_setting","avos_monitor");
        map_edit_setting.setValue("vehicle_map_path",QString::fromStdString(vehicle_map_path_));
        map_edit_setting.setValue("save_map_path",save_map_file_path_);
        map_edit_setting.setValue("map_data_path",gps_txt_file_path_);
        map_edit_setting.setValue("map_bag_local_path",map_bag_file_path_);
        map_edit_setting.setValue("export_map_path",export_map_path_);
        map_edit_setting.setValue("left_extend",left_extend_);
        map_edit_setting.setValue("right_extand",right_extand_);
        map_edit_setting.setValue("map_name",map_name_);
    }
    catch(...)
    {
    }
}

void MapEditDlg::OnScpMapToVehicle()
{	
    is_download_map_ = true;
    std::string cmd = "bash " + shell_path_ + "download_map.sh " + shell_path_ + " " + map_bag_file_path_.toStdString();

    qDebug() << "scp map file to vehicle cmd:" << QString::fromStdString(cmd);

    if(system(cmd.c_str()) != 0)
    {
        QMessageBox::warning(this, "信息提示", "下载地图到车端失败！");
        // qDebug()<< "下载地图到车端失败！";
    } 
    else
    {
        QMessageBox::information(this, "信息提示", "下载地图到车端成功！");
        // qDebug() << "下载地图到车端成功！";
    }

    is_download_map_ = false;
}

//--input_path /home/idriver/test   --output_path   /home/idriver/test --leftlane_width=2  --rightlane_width=6  --isleft_doublelane=false   --isright_doublelane=true
bool MapEditDlg::ExportMap(std::string cmd , std::string &error)
{
    pid_t status;
    std::string cd_cmd = "bash " + shell_path_ + "export_map.sh " + ros::package::getPath("avos_monitor") + "/thirdlib " + cmd;
    status = system(cd_cmd.c_str());
    // qDebug() << QString::fromStdString(cmd);

    if (-1 == status) {
        error = "phase 1:system error: -1";
        return false;
    } else {
        if (WIFEXITED(status)) {
            if (0 == WEXITSTATUS(status)) {
                error = "successfully";
                return true;
            } else {
                error = "phase 3:run shell script fail, script exit code: " + std::to_string(WEXITSTATUS(status));
                return false;
            }
        } else {
            error = "phase 2:exit status: " + std::to_string(WEXITSTATUS(status));
            return false;
        }
    }
}




// }//avvs