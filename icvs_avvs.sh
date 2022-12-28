#!/bin/bash
# Function: Start_Landship 
# Date:     2020.12.01
# Version:  V1.0

source /opt/ros/melodic/setup.bash
source /home/optimus/project/icvs_avvs/devel/setup.bash
export ROS_MASTER_URI=http://192.168.1.102:11311
MASTER_IP="192.168.1.102"

# MASTER_IP="127.0.0.1"
# export ROS_MASTER_URI=http://127.0.0.1:11311

work_mode=1 # 1-检测到102 ping不通 2-检测到工程启动超时 3-检测到工程正常启动

function CheckEnvironment()
{   
    (
    echo "5"
    echo "# 环境检测中..."
    ping -s 1 -w 4 $MASTER_IP >/dev/null 2>&1
    if [ "$?" != "0" ];
    then
        zenity --info \
        --text="未检测到车辆，请检查车辆是否开机，线缆连接是否正常!" \
        --title="提示"
    else
        #180s超时时间
        declare -i i=10
        while [ $i -lt 100 ]; 
        do
            ros_node_check=`timeout 2s rosnode list | wc -l`
            if [ $ros_node_check -lt 1 ];
            then 
                work_mode=2
                sleep 1 
                echo $i; i=$i+1;
                echo "# 系统启动中..."
            else
                echo "100"
                echo "# 系统启动成功"
                work_mode=3
                break
            fi
        done 
        printf "%d\n" "$work_mode"
        if [ "$work_mode" -eq 2 ];
        then
            zenity --info \
            --text="系统启动失败，建议重启车辆后再试！" \
            --title="提示"

        elif [ "$work_mode" = 3 ];
        then
            StartRVIZ
        else
            sleep 0.1
        fi
    fi
    ) |          zenity --progress \
                        --title="系统提示" \
                        --text="系统启动中..." \
                        --width 400 \
                        --height 150 \
                        --percentage=0 \
                        --auto-close 
}




function StartRVIZ()
{
    source /home/optimus/project/icvs_avvs/devel/setup.bash
    roslaunch /home/optimus/project/icvs_avvs/src/launch/icvs_avvs.launch
}


CheckEnvironment
