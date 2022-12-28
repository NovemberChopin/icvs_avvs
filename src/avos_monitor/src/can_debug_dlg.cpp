#include "can_debug_dlg.h"
#include "ui_can_debug_dlg.h"


//sshpass -p idriver_bbox ssh nvidia@192.168.1.102 -t candump can0
CanDebugDlg::CanDebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanDebugDlg)
{
    ui->setupUi(this);
    // m_nTimerId = startTimer(100);
}

CanDebugDlg::~CanDebugDlg()
{
    killTimer(m_nTimerId);
    delete ui;
}

void CanDebugDlg::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerId)
    {
    	if (can_connect_status_)
    	{
        	ui->receive_can_text->setText(can_recieve_);
    	}
    }
}

void CanDebugDlg::on_connect_can_btn_clicked()
{
	if (can_connect_status_)
	{
		OnDisconnectCan();
		can_connect_status_ = false;
		ui->connect_can_btn->setText("连接CAN");
	}
	else
	{
		if(OnConnectCan())
		{
			can_connect_status_ = true;
			ui->connect_can_btn->setText("断开CAN");
		}
		else
		{
        	QMessageBox::information(this, "信息提示", "CAN连接失败！");
		}
	}
}

void CanDebugDlg::on_start_recieve_btn_clicked()
{
	if (!can_connect_status_)
	{
        QMessageBox::information(this, "信息提示", "CAN未连接，需要先连接CAN才能接收！");
        return;
	}

	if (can_recieve_status_)
	{
		can_recieve_status_ = false;
		ui->start_recieve_btn->setText("开始接收");
	}
	else
	{
		can_recieve_status_ = true;
		ui->start_recieve_btn->setText("暂停接收");		
	}
}

void CanDebugDlg::on_send_can_btn_clicked()
{

}

void CanDebugDlg::on_cycle_send_can_btn_clicked()
{

}

bool CanDebugDlg::OnConnectCan()
{
    std::shared_ptr<std::thread> func_progress_ = std::make_shared<std::thread>(std::bind(&CanDebugDlg::RecieveCanDataThread, this));
    func_progress_->detach();
	return true;
}

bool CanDebugDlg::OnDisconnectCan()
{
	return true;
}

void CanDebugDlg::RecieveCanDataThread()
{	
    char result_buf[1024], command[1024];
    int rc = 0; // used to receive the command return value
    FILE *fp;
    /*Write command to buf*/
    snprintf(command, sizeof(command), "sshpass -p idriver_bbox ssh nvidia@192.168.1.102 -t candump can0");// | grep 603 
    /*Execute the preset command and read the standard output of the command*/
    fp = popen(command, "r");//read: Network is down
    if (NULL == fp)
    {
        return;
    }

    while (fgets(result_buf, sizeof(result_buf), fp) != NULL)//打开失败返回：read: Network is down
    {
        if ('\n' == result_buf[strlen(result_buf) - 1])
        {
            result_buf[strlen(result_buf) - 1] = '\0';
        }

        if (result_buf != command)
        {
    		std::cout << "OnConnectCan:" << result_buf << std::endl;
        	memcpy(command, result_buf, sizeof(result_buf));
        	can_recieve_ = QString::fromStdString(result_buf);
        }
    	// ui->receive_can_text->setText(QString::fromStdString(result_buf));

    	if (!can_connect_status_)
    	{
    		break;
    	}

    }

    /*Wait for the command to execute and close the pipe and file Pointers*/
    rc = pclose(fp);
    if (-1 == rc)
    {
        return;
    }

}