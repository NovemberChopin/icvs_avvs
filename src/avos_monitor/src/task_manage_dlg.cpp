#include "task_manage_dlg.h"
#include "ui_task_manage_dlg.h"
#include "map/map_loader/include/show_vectormap.h"

TaskManageDlg::TaskManageDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskManageDlg),
    service_client("/business_platform/service")
{
    ui->setupUi(this);
    InitTableView();

    ui->target_attribute_combox->addItem("终点");
    ui->target_attribute_combox->addItem("途经点");

	ui->task_pause_btn->setEnabled(false);
	ui->task_stop_btn->setEnabled(false);

    ros::NodeHandle nh;
    sub_nav_pose_ = nh.subscribe("/move_base_simple/goal", 100, &TaskManageDlg::CallbackNavPos, this);
}

TaskManageDlg::~TaskManageDlg()
{
    delete ui;
}

void TaskManageDlg::closeEvent(QCloseEvent *event)
{
	sub_nav_pose_.shutdown();
}

void TaskManageDlg::InitTableView()
{
    point_model_ = new QStandardItemModel();
    ui->point_table_view->setModel(point_model_);

    point_model_->setColumnCount(4);
    point_model_->setHeaderData(0, Qt::Horizontal, "点属性");
    point_model_->setHeaderData(1, Qt::Horizontal, "坐标X");
    point_model_->setHeaderData(2, Qt::Horizontal, "坐标Y");
    point_model_->setHeaderData(3, Qt::Horizontal, "航向角");

    // point_model_->setHeaderData(0, Qt::Vertical, "序号");
    QAbstractButton *btn = ui->point_table_view->findChild<QAbstractButton*>();
    if (btn)
    {
        btn->setText(QStringLiteral("序号"));
        btn->installEventFilter(this);/*注册事件 CPersonMng::eventFilter*/
        QStyleOptionHeader opt;
        opt.text = btn->text();
        QSize s = (btn->style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), btn).expandedTo(QApplication::globalStrut()));
        if (s.isValid())
        {
            ui->point_table_view->verticalHeader()->setMinimumWidth(50);
        }
    }

    ui->point_table_view->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//Qt::AlignLeft | Qt::AlignVCenter
    ui->point_table_view->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);//Qt::AlignLeft | Qt::AlignVCenter

    ui->point_table_view->setColumnWidth(0, 100);
    ui->point_table_view->setColumnWidth(1, 150);
    ui->point_table_view->setColumnWidth(2, 150);
    // ui->point_table_view->setColumnWidth(3, 150);
}

bool TaskManageDlg::eventFilter(QObject* o, QEvent* e)
{
    if (e->type() == QEvent::Paint)
    {
        QAbstractButton* btn = qobject_cast<QAbstractButton*>(o);
        if (btn)
        {
            QStyleOptionHeader opt;
            opt.init(btn);
            QStyle::State state = QStyle::State_None;
            if (btn->isEnabled())
                state |= QStyle::State_Enabled;
            if (btn->isActiveWindow())
                state |= QStyle::State_Active;
            if (btn->isDown())
                state |= QStyle::State_Sunken;
            opt.state = state;
            opt.rect = btn->rect();
            opt.text = btn->text(); // this line is the only difference to QTableCornerButton  
            opt.position = QStyleOptionHeader::OnlyOneSection;
            QStylePainter painter(btn);
            painter.drawControl(QStyle::CE_Header, opt);
            return true; // eat event  
        }
    }
    return false;
}
//cmd:0：default;1：准备自动驾驶; 2：开始任务; 3：结束任务; 4：暂停任务; 5：继续任务
void TaskManageDlg::on_task_start_btn_clicked()
{
	QString msg = "";
	if (task_status_ == 0)
	{
		if(TaskExecute(2, msg))
		{
	        QMessageBox::information(this, "信息提示", "任务下发成功！");
	        task_status_ = 2;
			ui->task_pause_btn->setEnabled(true);
			ui->task_stop_btn->setEnabled(true);
		}
		else
		{
	        QMessageBox::information(this, "信息提示", msg);		
		}
	}
	else
	{
        QMessageBox::information(this, "信息提示", "任务还未结束，无法开始新任务！");
        return;
	}
}

void TaskManageDlg::on_task_pause_btn_clicked()
{
	QString msg = "";
	if (task_status_ == 2)
	{
		if(TaskExecute(4, msg))
		{
	        QMessageBox::information(this, "信息提示", "任务暂停成功！");
	        task_status_ = 4;
	        ui->task_pause_btn->setText("任务继续");
		}
		else
		{
	        QMessageBox::information(this, "信息提示", msg);		
		}
	}
	else if (task_status_ == 4)
	{
		if(TaskExecute(5, msg))
		{
	        QMessageBox::information(this, "信息提示", "任务继续执行成功！");
	        task_status_ = 2;
	        ui->task_pause_btn->setText("任务暂停");
		}
		else
		{
	        QMessageBox::information(this, "信息提示", msg);		
		}
	}
	else
	{
        QMessageBox::information(this, "信息提示", "任务还未执行！");
        return;
	}

}

void TaskManageDlg::on_task_stop_btn_clicked()
{
	QString msg = "";
	if (task_status_ == 2 || task_status_ == 4)
	{
		if(TaskExecute(3, msg))
		{
	        QMessageBox::information(this, "信息提示", "任务结束成功！");
	        task_status_ = 0;
	        ui->task_pause_btn->setText("任务暂停");
			ui->task_pause_btn->setEnabled(false);
			ui->task_stop_btn->setEnabled(false);
		}
		else
		{
	        QMessageBox::information(this, "信息提示", msg);		
		}
	}
}

void TaskManageDlg::on_target_attribute_combox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "终点")
    {        
        PointGCCS output_pgccs;
        GetPathEndPoint(output_pgccs);

        ui->target_pose_x->setText(QString::number(output_pgccs.xg,'f',9));
        ui->target_pose_y->setText(QString::number(output_pgccs.yg,'f',9));
        ui->target_pose_heading->setText(QString::number(output_pgccs.angle,'f',6));
    }
}

void TaskManageDlg::on_add_point_btn_clicked()
{
	static TaskPoint task_point ;
	static QString text_temp = "";
	text_temp = ui->target_attribute_combox->currentText();
    if (text_temp.isEmpty())
    {    	
        QMessageBox::information(this, "信息提示", "目标点属性为空！");
    	return;
    }

    if (text_temp == "途经点")
    {
    	task_point.type = 1;
    }
    else if (text_temp == "终点")
    {
    	task_point.type = 2;   	
    }

    text_temp = ui->target_pose_x->text();
    if (text_temp.isEmpty())
    {
        QMessageBox::information(this, "信息提示", "目标点坐标X为空！");
    	return;
    }
    task_point.x = text_temp.toFloat();

    text_temp = ui->target_pose_y->text();
    if (text_temp.isEmpty())
    {
        QMessageBox::information(this, "信息提示", "目标点坐标Y为空！");
    	return;
    }
    task_point.y = text_temp.toFloat();

    text_temp = ui->target_pose_heading->text();
    if (text_temp.isEmpty())
    {
        QMessageBox::information(this, "信息提示", "目标点航向角为空！");
    	return;
    }
    task_point.heading = text_temp.toFloat();

    if (CheckPointExist(task_point))
    {
        QMessageBox::information(this, "信息提示", "目标点已存在，请重新添加！");
    	return;
    }

    int indexPos = point_model_->rowCount();
    QStandardItem *item = new QStandardItem;  

    text_temp = ui->target_attribute_combox->currentText();
    item = new QStandardItem;  
    item->setTextAlignment(Qt::AlignCenter);     
    item->setText(text_temp); 
    point_model_->setItem(indexPos, 0, item);

	text_temp = ui->target_pose_x->text();
    item = new QStandardItem;  
    item->setTextAlignment(Qt::AlignCenter);     
    item->setText(text_temp); 
    point_model_->setItem(indexPos, 1, item);


	text_temp = ui->target_pose_y->text();
    item = new QStandardItem;  
    item->setTextAlignment(Qt::AlignCenter);     
    item->setText(text_temp); 
    point_model_->setItem(indexPos, 2, item);

	text_temp = ui->target_pose_heading->text();
    item = new QStandardItem;  
    item->setTextAlignment(Qt::AlignCenter);     
    item->setText(text_temp); 
    point_model_->setItem(indexPos, 3, item);

    vfunction_point_.push_back(task_point);
}

void TaskManageDlg::on_remove_point_btn_clicked()
{
	static TaskPoint task_point ;
	static QString text_temp = "";
    int cur_row = ui->point_table_view->currentIndex().row();

    text_temp = point_model_->data(point_model_->index(cur_row,0)).toString();
    if (text_temp.isEmpty())
    {    	
        QMessageBox::information(this, "信息提示", "目标点属性为空！");
    	return;
    }

    if (text_temp == "途经点")
    {
    	task_point.type = 1;
    }
    else if (text_temp == "终点")
    {
    	task_point.type = 2;   	
    }
    else
    {
        QMessageBox::information(this, "信息提示", "目标点属性错误！");
        return;    	
    }

    text_temp = point_model_->data(point_model_->index(cur_row,1)).toString();
    if (text_temp.isEmpty())
    {
        QMessageBox::information(this, "信息提示", "目标点坐标X为空！");
    	return;
    }
    task_point.x = text_temp.toFloat();

    text_temp = point_model_->data(point_model_->index(cur_row,2)).toString();
    if (text_temp.isEmpty())
    {
        QMessageBox::information(this, "信息提示", "目标点坐标Y为空！");
    	return;
    }
    task_point.y = text_temp.toFloat();

    text_temp = point_model_->data(point_model_->index(cur_row,3)).toString();
    if (text_temp.isEmpty())
    {
        QMessageBox::information(this, "信息提示", "目标点航向角为空！");
    	return;
    }
    task_point.heading = text_temp.toFloat();

    if (!RemovePointFromVector(task_point))
    {
        QMessageBox::information(this, "信息提示", "列表中没有找到需要删除的点！");
    	return;
    }

    point_model_->removeRow(cur_row);

}

bool TaskManageDlg::CheckPointExist(const TaskPoint &point)
{
	// qDebug() << "CheckPointExist Points---> 坐标X:" << p->x << "  坐标Y:" << p->y << "　航向角:" << p->heading;
	for (std::vector<TaskPoint>::iterator p = vfunction_point_.begin(); p != vfunction_point_.end(); ++p)
	{
		if (p->x == point.x && p->y == point.y)
		{
			return true;
		}
	}
	return false;
}

bool TaskManageDlg::RemovePointFromVector(const TaskPoint &point)
{
	for (std::vector<TaskPoint>::iterator p = vfunction_point_.begin(); p != vfunction_point_.end(); ++p)
	{
		if (p->x == point.x && p->y == point.y && p->type == point.type)
		{
			vfunction_point_.erase(p);
			return true;
		}
	}
	return false;
}

bool TaskManageDlg::CheckPoint()
{
    TaskPoint end_point;
    int end_count = 0;
    for (std::vector<TaskPoint>::iterator it = vfunction_point_.begin(); it != vfunction_point_.end();)
    {
        if (it->type == 2)
        {
            end_count++;
            if (end_count > 1)
            {
                return false;
            }
            end_point.type = it->type;
            end_point.x = it->x;
            end_point.y = it->x;
            end_point.heading = it->heading;
            vfunction_point_.erase(it);
        }
        else
        {
            it++;
        }
    }
    vfunction_point_.push_back(end_point);//将终点放在最后
    return true;
}

//cmd:0：default;1：准备自动驾驶; 2：开始任务; 3：结束任务; 4：暂停任务; 5：继续任务
bool TaskManageDlg::TaskExecute(int cmd, QString & msg)
{
    if (CheckPoint() == false)
    {
        msg = "同一个任务中不能存在两个终点！";
        return false;
    }

    business_platform_api_msgs::AutomaticTaskInput input;
    business_platform_api_msgs::AvosServiceReply output;
    
    for(unsigned int i = 0; i < vfunction_point_.size(); ++i)
    {
        business_platform_api_msgs::AutomaticTaskDetail task_detail;
        task_detail.mission_id = i+1;
        task_detail.type = 4;

        task_detail.target_point.x = vfunction_point_[i].x;
        task_detail.target_point.y = vfunction_point_[i].y;

        input.task_detail.push_back(task_detail);
    }

    if(input.task_detail.size() > 0)
    {
        input.task_detail[input.task_detail.size() -1].type = 1;
    }

    input.cmd = cmd;

    int value = service_client.CallFunc<business_platform_api_msgs::AutomaticTaskInput, business_platform_api_msgs::AvosServiceReply>(1, 1, input, output );

    if (value == -1)
    {
        msg = "调用任务执行服务失败，请查检连接！";
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
        	msg = QString("任务执行失败(%1)!").arg(QString::fromStdString(output.addition_message));
        	return false;
        }
    }
	return false;
}

int  TaskManageDlg::GetTaskStatus(QString & msg)
{
    business_platform_api_msgs::AutomaticTaskStatus output;
    std_msgs::UInt8 input;
    input.data = 0;
    int value = service_client.CallFunc<std_msgs::UInt8 , business_platform_api_msgs::AutomaticTaskStatus>(1, 2, input, output);

    if(value == -1)
    {
        msg = "调用任务状态查询服务失败，请查检连接！";
        return -1;
    }
    else
    {
    	if (output.status == -1)
    	{    		
        	msg = QString("任务执行失败(%1)!").arg(QString::fromStdString(output.addition_message));
    	}
    	return output.status;
    }
}

void TaskManageDlg::CallbackNavPos(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
	tf::Quaternion quat;
	tf::quaternionMsgToTF(msg->pose.orientation, quat);
	double roll, pitch, yaw;//定义存储r\p\y的容器
	tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);//进行转换
	// qDebug() << "CallbackNavPos-->roll:" << roll << "  pitch:" << pitch << "  yaw:" << yaw;

	avos::vectormap::PointVCS point_vcs;
	point_vcs.x = msg->pose.position.x;
	point_vcs.y = msg->pose.position.y;
	point_vcs.angle = yaw;

	avos::vectormap::PointGCCS car_gccs;
	car_gccs.xg = g_common_info_.location_info.fusion_pos.xpos;//g_common_info_.map_center.xg;
	car_gccs.yg = g_common_info_.location_info.fusion_pos.ypos;//g_common_info_.map_center.yg;
    car_gccs.angle = g_common_info_.location_info.heading;    

	avos::vectormap::PointGCCS output_pgccs;
	geo_tool_.VCS2GCCS(car_gccs, point_vcs, output_pgccs);

	ui->target_pose_x->setText(QString::number(output_pgccs.xg,'f',9));
	ui->target_pose_y->setText(QString::number(output_pgccs.yg,'f',9));
	ui->target_pose_heading->setText(QString::number(output_pgccs.angle,'f',6));
}