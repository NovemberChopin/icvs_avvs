/******************************************************************************
* Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: ivbusinesscentre
* FileName: centrecommon.cpp
*
* Description: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    
*
* History:
* xxxxxx         xxxx/xx/xx    1.0.0    build this module.
******************************************************************************/
#include "data_pool.h"


pthread_mutex_t         get_monitor_mutex_;          //文件读写操作互斥量



AvvsCommonInfo          g_common_info_;
FaultsInfo              g_monitor_info_;

/**********************************************
 * Function: ParamInit()
 * Author: zengzicheng
 * Date: 2019-04-01
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void InitCommonParams()
{        
    pthread_mutex_init(&get_monitor_mutex_, NULL);
}




int myexec(const char *cmd, std::vector<std::string> &resvec) {
    resvec.clear();
    FILE *pp = popen(cmd, "r"); //建立管道
    if (!pp) {
        return -1;
    }
    char tmp[1024]; //设置一个合适的长度，以存储每一行输出
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        if (tmp[strlen(tmp) - 1] == '\n') {
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        }
        resvec.push_back(tmp);
    }
    pclose(pp); //关闭管道
    return resvec.size();
}


bool getPackagePath(std::string package_name, std::string &res) {
    std::vector<std::string> resvec;
    std::string cmd = "rospack find " + package_name;
    myexec(cmd.c_str(), resvec);
    if (resvec.size() == 0)
        return false;
    res = resvec[0];
    return true;
}


int IsFolderExist(const char* path)
{
    DIR *dp;
    if ((dp = opendir(path)) == NULL)
    {
        return 0;
    }

    closedir(dp);
    return -1;
}

//检查文件(所有类型)是否存在
//-1:存在 0:不存在
int IsFileExist(const char* path)
{
    return !access(path, F_OK);
}


void SetFileDialog(QFileDialog &fd, QFileDialog::FileMode f_mode, QFileDialog::ViewMode v_mode, QFileDialog::AcceptMode a_mode)
{
    fd.setOptions(QFileDialog::DontConfirmOverwrite);
    fd.setFileMode(f_mode); //FileMode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly }
    fd.setViewMode(v_mode);//设置视图模式 ViewMode { Detail, List }
    fd.setAcceptMode(a_mode);//AcceptMode { AcceptOpen, AcceptSave }
    fd.setStyleSheet("QTextEdit{color:white;background-color:#555555;}"\
                     "QComboBox{color:rgb(255, 255, 255);background-color:rgb(85, 87, 83);\
                     border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 2px;}\
                     QHeaderView::section{background-color:#666666;color: white;}");
}

void SetMessageBox(QMessageBox &box)
{    
    box.setStandardButtons (QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Yes,QString("是"));
    box.setButtonText (QMessageBox::No,QString("否"));
    box.setButtonText (QMessageBox::Cancel,QString("取消"));
    box.setStyleSheet("QPushButton { background-color: qlineargradient(spread:pad, x1:0.0618557, y1:1, x2:0.948454, y2:0.057, stop:0 rgba(50, 50, 50, 255), stop:0.618557 rgba(100, 100, 100, 255)); color: white; }"\
                      "QPushButton:hover{background-color:qlineargradient(spread:pad, x1:0.0618557, y1:1, x2:0.948454, y2:0.057, stop:0 rgba(100, 100, 100, 255), stop:0.618557 rgba(50, 50, 50, 255));color: white;}"\
                      "QDialog{color:white;background-color:#555555;}"\
                      "QLabel{color:white;background-color:#555555;}");
}
/**********************************************
 * Function: GetVehicleStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetVehicleStateInfo(int state)
{
  //车辆状态 0：初始化,1：休眠,2：遥控就绪,3：自动就绪,4：遥控驾驶,5：自动驾驶,6：人工干预,7：急停,8：故障
    static QString txt;
    switch(state)
    {
        case 0:
        {
            txt = "0:初始化";
        }
        break;
        case 1:
        {
            txt = "1:休眠";
        }
        break;
        case 2:
        {
            txt = "2:遥控就绪";
        }
        break;
        case 3:
        {
            txt = "3:自动就绪";
        }
        break;
        case 4:
        {
            txt = "4:遥控驾驶";
        }
        break;
        case 5:
        {
            txt = "5:自动驾驶";
        }
        break;
        case 6:
        {
            txt = "6:人工干预";
        }
        break;
        case 7:
        {
            txt = "7:急停";
        }
        break;
        case 8:
        {
            txt = "8:故障";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }
    return txt;
}


/**********************************************
 * Function: GetDriverModeStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetDriverModeStateInfo(int state)
{
    static QString txt;
    if (state)
    {
        txt = "能力具备";
    }
    else
    {
        txt = "不可用";
    } 

    return txt;
}


/**********************************************
 * Function: GetLocationStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetLocationStateInfo(int state)
{
    //0:好1:一般2:不可用
    static QString txt;

    switch(state)
    {
        case 0:
        {
            txt = "很好";
        }
        break;
        case 1:
        {
            txt = "一般";
        }
        break;
        case 2:
        {
            txt = "不可用";
        }
        break;
        case 3:
        {
          txt = "IMU初始化中";
        }
        break;
        case 4:
        {
          txt = "激光初始化中";
        }
        break;
        case 5:
        {
          txt = "地图加载中";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}


/**********************************************
 * Function: GetEpsStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetEpsStateInfo(int state)
{
  //0：停止工作状态Stop;0x10：手动控制状态Manual control;0x20: 自动控制状态Auto control;0x55：错误状态Error
    static QString txt;
    switch(state)
    {
        case 0:
        {
            txt = "工作正常";
        }
        break;
        case 1:
        {
            txt = "EPS故障";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }
    // switch(state)
    // {
    //     case 0:
    //     {
    //         txt = "停止工作";
    //     }
    //     break;
    //     case 0x10:
    //     {
    //         txt = "手动控制";
    //     }
    //     break;
    //     case 0x20:
    //     {
    //         txt = "自动控制";
    //     }
    //     break;
    //     case 0x55:
    //     {
    //         txt = "错误状态";
    //     }
    //     break;
    //     default:
    //     {            
    //         txt = "未知";
    //     }
    //     break;
    // }

    return txt;
}
QString GetVCUStateInfo(int state)
{
    static QString txt;
    switch(state)
    {
        case 0:
        {
            txt = "工作正常";
        }
        break;
        case 0x10:
        {
            txt = "手动控制";
        }
        break;
        case 0x20:
        {
            txt = "自动控制";
        }
        break;
        case 0x55:
        {
            txt = "错误状态";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}

QString GetNboostStateInfo(int state)
{
    //0:No fault 0x1:Minor fault 0x2:General fault 0x3:Major fault 0x4:Serious fault 0x5:Especially serious fault
    static QString txt;
    switch(state)
    {
        case 0x0:
        {
            txt = "工作正常";
        }
        break;
        case 0x1:
        {
            txt = "轻微故障";
        }
        break;
        case 0x2:
        {
            txt = "常见故障";
        }
        break;
        case 0x3:
        {
            txt = "主要故障";
        }
        break;
        case 0x4:
        {
            txt = "严重故障";
        }
        break;
        case 0x5:
        {
            txt = "特级严重故障";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}

QString GetAvosStateInfo(int state)
{
    static QString txt;
    switch(state)
    {
        case 0:
        {
            txt = "模块缺失";
        }
        break;
        case 1:
        {
            txt = "启动正常";
        }
        break;
        case -1:
        {
            txt = "状态未知";
        }
        break;
        default:
        break;
    }

    return txt;
}
/**********************************************
 * Function: GetActuatorStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/

QString GetActuatorStateInfo(int state)
{
  //执行器模式:0-减速Decelerate；1-加速Accelerate;2-紧急制动Emergency braking
    static QString txt;

    switch(state)
    {
        case 0:
        {
            txt = "0:减速";
        }
        break;
        case 1:
        {
            txt = "1:加速";
        }
        break;
        case 2:
        {
            txt = "2:紧急制动";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}



/**********************************************
 * Function: GetShiftVlpositionStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetShiftVlpositionStateInfo(int state)
{
  //档位信息:0-无效;1-R档;2-N档;3-D档
    static QString txt;

    switch(state)
    {
        case 0:
        {
            txt = "0-无效";
        }
        break;
        case 1:
        {
            txt = "1-R档";
        }
        break;
        case 2:
        {
            txt = "2-N档";
        }
        break;
        case 3:
        {
            txt = "3-D档";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}


/**********************************************
 * Function: GetGpsStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetGpsStateInfo(int state)
{
  //０无定位信息１单点定位２伪距差分３不考虑４RTK固定解５RTK浮点解gps状态: 4为正常
    static QString txt;

    switch(state)
    {
        case 0:
        {
            txt = "0:定位无效";
        }
        break;
        case 1:
        {
            txt = "1:单点定位";
        }
        break;
        case 2:
        {
            txt = "2:伪距差分";
        }
        break;
        case 4:
        {
            txt = "4:RTK固定解";
        }
        break;
        case 5:
        {
            txt = "5:RTK浮点解";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}

/**********************************************
 * Function: GetStartStateInfo()
 * Author: zengzicheng
 * Date: 2019-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetStartStateInfo(int state)
{
  //０无定位信息１单点定位２伪距差分３不考虑４RTK固定解５RTK浮点解gps状态: 4为正常
    static QString txt;

    switch(state)
    {
        case 0:
        {
            txt = "0:启动失败";
        }
        break;
        case 1:
        {
            txt = "1:启动成功";
        }
        break;
        case 2:
        {
            txt = "2:正在启动";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}

/**********************************************
 * Function: GetOpenCloseInfo()
 * Author: zengzicheng
 * Date: 2018-12-05
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
QString GetOpenCloseInfo(int state)//0:关闭；1：开启
{
    static QString txt;

    switch(state)
    {
        case 0:
        {
            txt = "0:关闭";
        }
        break;
        case 1:
        {
            txt = "1:开启";
        }
        break;
        default:
        {            
            txt = "未知";
        }
        break;
    }

    return txt;
}