#!/bin/bash

# version 0.0.1
# version 0.0.2 romove the password 20181120
# version 0.0.3 modify bug , origin_path 20181121

# mkdir some files
arg=`ifconfig eth0 |grep "inet addr"| cut -f 2 -d ":"|cut -f 1 -d " " `
localhost_ip_101="192.168.1.101"
# localhost_ip_102="192.168.1.47" # debug
origin_path=`pwd`
password="idriver_bbox"
project_name="resident"
back_up_status="false"
ssh_pass="sshpass -p ${password}"
# password="root" # debug

function mkAvosFile()
{
  cd ${HOME}
  if [ ! -d ".ros" ] 
  then
    mkdir .ros
  else
    echo ${password} | sudo -S rm .ros/* -rf
  fi
}

function mkProjectFile()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su
  cd /
  if [ ! -d "work/share/project/${project_name}" ]
  then
    echo ${password} | sudo -S mkdir -p work/share/project/${project_name}
  fi
  cd /work/share/project
  sudo rm -rf resident.tar.gz
  sudo chmod -R 777 ${project_name}
  cd $origin_path
}

function mk_102()
{
  echo ${password} | sudo -S su
  cd /
  if [ ! -d "work/share/project/resident" ]
  then
    echo ${password} | sudo -S mkdir -p work/share/project/resident
  else
    sudo rm /work/share/project/resident/* -rf
  fi
  cd /work/share/project
  sudo chmod -R 777 resident
  cd $origin_path
}

function mkMapFile()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su
  cd /
  if [ ! -d "work/share/project/${project_name}/src/map_file/" ]
  then
    echo ${password} | sudo -S mkdir -p work/share/project/${project_name}/src/map_file/
  fi
  cd /work/share/project/${project_name}/src/  
  sudo rm -rf *.tar.gz
  sudo chmod -R 777 map_file
  cd $origin_path
}

function mkLaunchFile()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su
  cd /
  if [ ! -d "work/share/project/${project_name}/src/launch/" ]
  then
    echo ${password} | sudo -S mkdir -p work/share/project/${project_name}/src/launch/
  fi
  cd /work/share/project/${project_name}/src/  
  sudo rm -rf *.tar.gz
  sudo chmod -R 777 launch
  cd $origin_path
}

function mk_map_102()
{
  echo ${password} | sudo -S su
  cd /
  cd /work/share/project/resident
  sudo chmod -R 777 *
  cd $origin_path
}

function mkOtaFile()
{
  echo ${password} | sudo -S su  
  cd /
  if [ ! -d "work/share/project/ota" ]
  then
    sudo mkdir -p work/share/project/ota
  else
    sudo rm /work/share/project/ota/* -rf
  fi
  if [ ! -d "work/share/project/downloads/firmware/dmcu" ]
  then
    echo ${password} | sudo -S mkdir -p work/share/project/downloads
    sudo mkdir -p work/share/project/downloads/avos
    sudo mkdir -p work/share/project/downloads/map
    sudo mkdir -p work/share/project/downloads/launch
    sudo mkdir -p work/share/project/downloads/app
    sudo mkdir -p work/share/project/downloads/firmware/bcm
    sudo mkdir -p work/share/project/downloads/firmware/mcu
    sudo mkdir -p work/share/project/downloads/firmware/dmcu
  #else
    #sudo rm /work/share/project/downloads/* -rf
    
  fi

  if [ ! -d "work/share/project/history/firmware/dmcu" ]
  then
    echo ${password} | sudo -S mkdir -p work/share/project/history
    sudo mkdir -p work/share/project/history/avos
    sudo mkdir -p work/share/project/history/map
    sudo mkdir -p work/share/project/history/launch
    sudo mkdir -p work/share/project/history/app
    sudo mkdir -p work/share/project/history/firmware/bcm
    sudo mkdir -p work/share/project/history/firmware/mcu
    sudo mkdir -p work/share/project/history/firmware/dmcu
  #else
    #sudo rm /work/share/project/history/* -rf
  fi
  cd /work/share/project
  sudo chmod -R 777 ota
  cd $origin_path
}

function mkAllFile()
{
  echo "mkdir all files"
  mkAvosFile
  mkProjectFile
  mkOtaFile
}

function back_up_avos()
{
  cd ~/.ros
  tar -czf avos.tar.gz avos
  cd $origin_path
}

function back_up_project()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su  
  cd /
  cd work/share/project/${project_name}
  tar_name="${project_name}.tar.gz"
  sudo tar -czf $tar_name *
  cd $origin_path
}

function back_up_ota()
{
  echo ${password} | sudo -S su  
  cd /
  cd work/share/project/ota
  tar_name='ota.tar.gz'
  sudo tar -czf $tar_name *
  cd $origin_path
}

function back_up_map()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su  
  cd /
  cd work/share/project/${project_name}/src
  tar_name='map_file.tar.gz'
  sudo tar -czf $tar_name map_file
  cd $origin_path
}

function back_up_launch()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su  
  cd /
  cd work/share/project/${project_name}/src
  tar_name='launch.tar.gz'
  sudo tar -czf $tar_name launch
  cd $origin_path
}

function backup_local_map()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su
  cd /
  if [ ! -d "work/share/project/${project_name}/src/map_file/" ]
  then
    return 0
  else
    cd work/share/project/${project_name}/src/
    tar_name='map_file.tar.gz'
    sudo tar -czf $tar_name map_file
    sudo mv $tar_name ~/
    back_up_status="true"
    cd $origin_path
  fi
}

function recover_local_map()
{
  password=$1
  project_name=$2
  echo ${password} | sudo -S su  
  cd /
  cd work/share/project/${project_name}/src
  tar_name='map_file.tar.gz'
  sudo mv ~/$tar_name .
  sudo tar -xzf $tar_name map_file
  sudo chmod -R 777 map_file
  sudo rm $tar_name
  cd $origin_path
}

function add_key_to_103() {
  if cat ~/.ssh/id_rsa.pub
  then
    echo -e "id_rsa.pub 公钥已经存在."
  else
    echo -e "生成公钥，执行 ssh-keygen 命令行."
    ssh-keygen
  fi
  local_key_name="/home/nvidia/.ssh/id_rsa.pub"
  remote_key_name="/home/nvidia/.ssh/id_rsa.remote"
  target_pre="nvidia@192.168.1.103"
  cmd_cp_key="${target_pre}:${remote_key_name}"

  echo -e "开始从102板卡添加公钥到103板卡......."
  ${ssh_pass} scp ~/.ssh/id_rsa.pub ${cmd_cp_key}

  echo -e "添加102板卡的公钥到103板卡的 authorized_keys..."
  cmd_add="cat ${remote_key_name} >> ~/.ssh/authorized_keys ; rm ${remote_key_name}; sudo chmod 600 /home/nvidia/.ssh/authorized_keys"

  ${ssh_pass} ssh ${target_pre} -t ${cmd_add}
  return_var=$?
  if [ ${return_var} != 0 ] ; then
    echo -e "${RED}从102板卡往103板卡密钥写入失败.${NONE}"
  else
    echo -e "${GREEN}从102板卡往103板卡密钥写入成功.${NONE}"
  fi
}

function updateScript()
{
  echo ${password} | sudo -S su
  cd /home/nvidia/
  sudo mv start_avos.sh /boot/
  cd $origin_path
}

function rosnode_kill()
{
  local node_name=$1
  cd /work/share/project/resident/
  source devel/setup.bash
  rosnode kill $node_name
  cd $origin_path
}

function rosnode_run()
{
  local node_name=$1
  echo ${password} | sudo -S su
  cd /work/share/project/resident/
  source devel/setup.bash

  if [ $node_name == "ivlocalization" ] ; then
    roslaunch launch localization.launch &
  elif [ $node_name == "perception" ]; then
    roslaunch launch perception.launch &
  elif [ $node_name == "planning" ]; then
    roslaunch launch planning.launch &
  elif [ $node_name == "control" ]; then
    roslaunch launch planning.launch &
  elif [ $node_name == "canbus" ]; then
    roslaunch launch canbus102.launch &
  elif [ $node_name == "business_platform" ]; then
    roslaunch launch business_platform.launch &
  elif [ $node_name == "ivcamera" ]; then
    roslaunch launch ivcamera.launch &
  elif [ $node_name == "lslidar" ]; then
    roslaunch launch lslidar_c32.launch &
  elif [ $node_name == "ars408" ]; then
    roslaunch launch ars408.launch &
  else [ $node_name == "imu" ];
    roslaunch launch navigation.launch &
  fi

  cd $origin_path
}