#!/bin/bash

# version 0.0.1
# version 0.0.2 romove the password 20181120
# version 0.0.3 modify bug , origin_path 20181121
# version 0.0.4 add reboot , 20181125
# version 0.0.5 add readme , 20181205
# version 0.0.6 change : 1.ota_exe.tar.gz to resident.tar.gz;2.and add version file;3.delete readALLfile , 20190124
# version 0.0.7 change : 1.add backup avos file ,ota file ,and resident file , 20190130
# version 0.0.8 change : 1.add backup avos file ,map file , 20190131,2.-all update all file
# version 0.0.9 change : 1.add check local version info 20190215
# version 0.1.0 change : 1.change avos version not write if update by ota 20190221
# version 0.1.1 change : 1.restore 1.0 to 0.9 about write avos version, 2.tar the resident from resident fodler up ;3.change update project folder ;4.change update ota folder .20190325
# version 0.1.2 change : 1.Add the function When create the project of create a release package .20190401
# version 0.1.3 change : 增加地图备份功能逻辑，在更新工程时会先备份当前地图，工程更新成功后会自动恢复原有的地图文件，解决了更新工程会导致原地图被覆盖的问题；
# version 0.1.4 change : 增加launch配置文件的更新功能，在一键部署中增加了配置更新的步骤，同时也提供单独下载更新配置文件的功能；

version="0.1.4"
# ****** default parameter ******
target_user="nvidia"
# target_user="dlonghui" #debug
TargetIP_102="192.168.1.102"
TargetIP_103="192.168.1.103"
# target_ip="192.168.1.47" #debug
online_102="false"
online_103="false"
remote_project="/work/share/project/"
exe_status="false"
# ****** default file path ******
origin_path=`pwd`
remote_home="/home/${target_user}/"
remote_ota="ota/"
target_pass_word="idriver_bbox"
#project_name=$(pwd |awk -F '/' '{print $NF}')
project_name=resident
package_name="${project_name}.tar.gz"

# ****** default file name ******
default_map_name="map.tar.gz"
default_ota_name="ota.tar.gz"
default_avos_name="avos.tar.gz"
start_avos_script="start_avos.tar.gz"
remote_sh="remote_run.sh"
versioncheck_sh="version_check.sh"
remote_name="remote_run.tar.gz"
version_check="version_check.tar.gz"
start_launch_file="avos.sh"
readme="ReadMe.md"
function_point="function_point.yaml"
function_point_tar="function_point.tar.gz"
launch_name="launch.tar.gz"

# ****** default password ******
remote_pw="echo "${target_pass_word}" | sudo -S"
# remote_pw="echo "root" | sudo -S" #debug
ssh_pass="sshpass -p ${target_pass_word}"
# ssh_pass="sshpass -p root" #debug
remove_yes="-o StrictHostKeyChecking=no"
local_pw="idriver"

# ****** default color ******
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELL='\033[0;33m'
BOLD='\033[1m'
NONE='\033[0m'

# ****** function ******
# check the car is or not online
function is_online()
{
  target_ip_102=${TargetIP_102}
  if ping -c1 $target_ip_102
  then
    echo -e "${GREEN}102板卡在线，可以往下执行......${NONE}"
    online_102="true"
  else
    echo -e "${RED}102板卡不在线，无法Ping通，请检查网络连接.${NONE}"
    online_102="false"
  fi

  target_ip_103=${TargetIP_103}
  if ping -c1 $target_ip_103
  then
    echo -e "${GREEN}103板卡在线，可以往下执行......${NONE}"
    online_103="true"
  else
    echo -e "${RED}103板卡不在线，无法Ping通，请检查网络连接.${NONE}"
    online_103="false"
  fi
}

function add_key() {
  add_key_local_to_102
  add_key_local_to_103
  add_key_102_to_103
}

function add_key_local_to_102() {
  this_user=${target_user}
  this_ip=${TargetIP_102}
  local_key_name="/home/idriver/.ssh/id_rsa.pub"
  remote_key_name="/home/${this_user}/.ssh/id_rsa.remote"
  target_pre="${this_user}@${this_ip}"
  cmd_cp_key="${target_pre}:${remote_key_name}"

  echo -e "开始从本地电脑添加公钥到102板卡......."
  ${ssh_pass} scp ~/.ssh/id_rsa.pub ${cmd_cp_key}

  echo -e "添加你的公钥到102板卡的 authorized_keys..."
  cmd_add="cat ${remote_key_name} >> ~/.ssh/authorized_keys ; rm ${remote_key_name}; sudo chmod 600 /home/${this_user}/.ssh/authorized_keys"
  
  ${ssh_pass} ssh ${target_pre} -t ${cmd_add}
  return_var=$?
  if [ ${return_var} != 0 ] ; then
    echo -e "${RED}102板卡密钥写入失败.${NONE}"
  else
    echo -e "${GREEN}102板卡密钥写入成功.${NONE}"
  fi
}


function add_key_local_to_103() {
  this_user=${target_user}
  this_ip=${TargetIP_103}
  local_key_name="/home/idriver/.ssh/id_rsa.pub"
  remote_key_name="/home/${this_user}/.ssh/id_rsa.remote"
  target_pre="${this_user}@${this_ip}"
  cmd_cp_key="${target_pre}:${remote_key_name}"

  echo -e "开始从本地电脑添加公钥到103板卡......."
  ${ssh_pass} scp ~/.ssh/id_rsa.pub ${cmd_cp_key}

  echo -e "添加你的公钥到103板卡的 authorized_keys..."
  cmd_add="cat ${remote_key_name} >> ~/.ssh/authorized_keys ; rm ${remote_key_name}; sudo chmod 600 /home/${this_user}/.ssh/authorized_keys"
  
  ${ssh_pass} ssh ${target_pre} -t ${cmd_add}
  return_var=$?
  if [ ${return_var} != 0 ] ; then
    echo -e "${RED}103板卡密钥写入失败.${NONE}"
  else
    echo -e "${GREEN}103板卡密钥写入成功.${NONE}"
  fi
}

function add_key_102_to_103() {
  is_online
  if [ $online_102 == "true" ]
  then
    createRemoteRun102    
    echo -e "开始从102板卡添加公钥到103板卡 ..."    
    remote_cmd2="source ${remote_home}${remote_sh};add_key_to_103;"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd2}
    then
      echo -e "${GREEN}从102板卡往103板卡写入密钥成功.${NONE}" 
      rm -rf remote_run.tar.gz
    else
      echo -e "${RED}从102板卡往103板卡写入密钥失败.${NONE}"
      rm -rf remote_run.tar.gz
      return 0
    fi
  else    
    echo -e "${RED}从102板卡往103板卡写入密钥失败.${NONE}"
    rm -rf remote_run.tar.gz
    return 0
  fi
}

# create bash shell in the remote
function createRemoteRun102()
{
  cd ${origin_path}  
  tar -zcvf remote_run.tar.gz remote_run.sh
  #tar -czf ${remote_name} ${remote_sh} 
  tar_cmd="tar -xzf ${remote_name}"
  if [ $online_102 == "true" ]
  then
    echo -e "开始给102板卡创建远程设备的操作脚本......."
    if ${ssh_pass} scp ${remove_yes} ${remote_name} ${target_user}@${TargetIP_102}:${remote_home}
    then
      echo -e "${GREEN}远程拷贝脚本到102板卡成功.${NONE}"
    else
      echo -e "${RED}远程拷贝脚本到102板卡错误.${NONE}"
      return 0
    fi
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${tar_cmd}
    then 
      echo -e "${GREEN}解压脚本到102板卡成功.${NONE}"
    else
      echo -e "${RED}解压脚本到102板卡错误.${NONE}"
      return 0
    fi
    echo -e "${GREEN}在102板卡上创建脚本成功.${NONE}"
  else
    return 0
  fi
}

function createRemoteRun103()
{
  cd ${origin_path}  
  tar -zcvf remote_run.tar.gz remote_run.sh
  #tar -czf ${remote_name} ${remote_sh} 
  tar_cmd="tar -xzf ${remote_name}"
  if [ $online_103 == "true" ]
  then
    echo -e "开始给103板卡创建远程设备的操作脚本......."
    if ${ssh_pass} scp ${remove_yes} ${remote_name} ${target_user}@${TargetIP_103}:${remote_home}
    then
      echo -e "${GREEN}远程拷贝脚本到103板卡成功.${NONE}"
    else
      echo -e "${RED}远程拷贝脚本到103板卡错误.${NONE}"
      return 0
    fi
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${tar_cmd}
    then 
      echo -e "${GREEN}解压脚本到103板卡成功.${NONE}"
    else
      echo -e "${RED}解压脚本到103板卡错误.${NONE}"
      return 0
    fi
    echo -e "${GREEN}在103板卡上创建脚本成功.${NONE}"
  else
    return 0
  fi
}

# delete bash shell in the remote
function deleteRemoteRun102()
{
  de_cmd="cd ${remote_home};rm ${remote_sh};rm ${remote_name}"
  if [ $online_102 == "true" ]
  then
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${de_cmd}
    then
      echo -e "${GREEN}删除102板卡上的操作脚本成功.${NONE}"
    else
      echo -e "${RED}删除102板卡上的操作脚本失败.${NONE}"
    fi
  else
    return 0
  fi
  cd ${origin_path}
  #rm ${remote_name}
}

function deleteRemoteRun103()
{
  de_cmd="cd ${remote_home};rm ${remote_sh};rm ${remote_name}"
  if [ $online_103 == "true" ]
  then
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${de_cmd}
    then
      echo -e "${GREEN}删除103板卡上的操作脚本成功.${NONE}"
    else
      echo -e "${RED}删除103板卡上的操作脚本失败.${NONE}"
    fi
  else
    return 0
  fi
  cd ${origin_path}
  #rm ${remote_name}
}
# update all 
function updateAll()
{
  echo -e "${YELL}********开始一键部署，包括密钥的写入、自启动脚本、AVOS工程、地图和配置**********${NONE}"

  echo -e "${YELL}第一步：现在开始进行密钥的写入，无密码登录102和103板卡，无密码从102板卡登录103板卡${NONE}"
  sleep 5
  add_key

  echo -e "${YELL}第二步：现在开始现在开始进行自启动脚本的更新，包括102板卡和103板卡的脚本更新${NONE}"
  sleep 5
  updateScripts

  echo -e "${YELL}第三步：现在开始更新AVOS工程，包括102板卡和103板卡的工程更新${NONE}"
  sleep 2
  updateProject102
  sleep 2
  updateProject103

  echo -e "${YELL}第四步：现在开始更新地图，包括102板卡和103板卡的地图更新${NONE}"
  sleep 5
  updateMap

  echo -e "${YELL}第五步：现在开始更新配置文件，包括102板卡和103板卡的launch更新${NONE}"
  sleep 5
  updateLaunch

  echo -e "${YELL}第六步：一键部署成功，请下电重启车辆，进行自动驾驶验证${NONE}"
  # restart
  # echo -e "${YELL}第七步：车辆重启成功，请耐心等待..............${NONE}\n"
}


function update_target_point {
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "开始修改目标点功能点坐标......."
    if ${ssh_pass} scp ${remove_yes} ${function_point} ${target_user}@${TargetIP_102}:${remote_project}/${project_name}/
    then
      echo -e "${GREEN}修改目标功能点坐标成功，现在可以重新进自动驾驶去到这个目标功能点.${NONE}"
    else
      echo -e "${RED}目标功能点压缩文件传输错误.${NONE}"
      return 0
    fi
  else
    echo -e "${RED}修改目标功能点坐标失败，网络未连接.${NONE}"
    return 0
  fi
}

function backup_current_map_102 {
  is_online
  if [ $online_102 == "true" ]
  then
    echo "开始备份102板卡当前地图文件(map_file)..."
    remote_cmd="source ${remote_home}${remote_sh};backup_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}备份102板卡上的地图文件成功.${NONE}" 
    else
      echo -e "${RED}备份102板卡上的地图文件失败.${NONE}" 
    fi
  else
    echo -e "${RED}网络未连接，备份102板卡上的地图文件失败.${NONE}"
    return 0
  fi
}

function recover_current_map_102 {
  is_online
  if [ $online_102 == "true" ]
  then
    echo "开始恢复102板卡的地图文件(map_file)..."
    remote_cmd="source ${remote_home}${remote_sh};recover_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}恢复102板卡上的地图文件成功.${NONE}" 
    else
      echo -e "${RED}恢复102板卡上的地图文件失败.${NONE}" 
    fi
  else
    echo -e "${RED}网络未连接，恢复102板卡上的地图文件失败.${NONE}"
    return 0
  fi
}

function backup_current_map_103 {
  is_online
  if [ $online_103 == "true" ]
  then
    echo "开始备份103板卡当前地图文件(map_file)..."
    remote_cmd="source ${remote_home}${remote_sh};backup_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${remote_cmd}
    then
      echo -e "${GREEN}备份103板卡上的地图文件成功.${NONE}" 
    else
      echo -e "${RED}备份103板卡上的地图文件失败.${NONE}" 
    fi
  else
    echo -e "${RED}网络未连接，备份103板卡上的地图文件失败.${NONE}"
    return 0
  fi
}

function recover_current_map_103 {
  is_online
  if [ $online_103 == "true" ]
  then
    echo "开始恢复103板卡的地图文件(map_file)..."
    remote_cmd="source ${remote_home}${remote_sh};recover_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${remote_cmd}
    then
      echo -e "${GREEN}恢复103板卡上的地图文件成功.${NONE}" 
    else
      echo -e "${RED}恢复103板卡上的地图文件失败.${NONE}" 
    fi
  else
    echo -e "${RED}网络未连接，恢复103板卡上的地图文件失败.${NONE}"
    return 0
  fi
}

function updateProject()
{
  backup_current_map_102
  updateProject102
  recover_current_map_102

  backup_current_map_103
  updateProject103
  recover_current_map_103
}

function updateProject102()
{
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "开始更新103板卡的AVOS工程......."
    createRemoteRun102
    remote_cmd2="source ${remote_home}${remote_sh};mkProjectFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd2}
    then
      echo -e "${GREEN}在102板卡上创建工程文件夹成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}在102板卡上创建工程文件夹错误.${NONE}"
      echo -e "${RED}更新102板卡的工程失败.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${package_name} ${target_user}@${TargetIP_102}:${remote_project}
      then
        echo -e "${GREEN}下载工程(resident.tar.gz)到102板卡成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}下载工程(resident.tar.gz)到102板卡错误.${NONE}"
        echo -e "${RED}更新102板卡的AVOS工程失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project};${remote_pw} rm -rf ${remote_project}/${project_name}/*;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${tar_cmd2}
      then
        echo -e "${GREEN}删除102板卡的旧工程文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除102板卡的旧工程文件失败.${NONE}"
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project};${remote_pw} tar -xzf ${remote_project}${package_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${tar_cmd2}
      then
        echo -e "${GREEN}解压102板卡的工程文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}解压102板卡的工程文件错误.${NONE}"
        echo -e "${RED}更新102板卡的工程失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${package_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${rm_cmd2}
      then
        echo -e "${GREEN}删除102板卡的工程更新包.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除102板卡的工程更新包错误.${NONE}"
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${chmod_cmd}
      then
        echo -e "${GREEN}给103板卡的工程文件加上最高访问权限.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}给103板卡的工程文件加上最高访问权限错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      sync_cmd="${remote_pw} sync;"
      ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${sync_cmd}
      echo -e "${GREEN}更新102板卡的工程成功.${NONE}"
    else
      echo -e "${RED}更新102板卡的工程失败.${NONE}"
      return 0
    fi
  else
    echo -e "${RED}更新102板卡的工程失败.${NONE}"
    return 0
  fi
}
# update project package
function updateProject103()
{
  is_online
  if [ $online_103 == "true" ]
  then
    echo -e "开始更新103板卡的AVOS工程......"
    createRemoteRun103
    remote_cmd2="source ${remote_home}${remote_sh};mkProjectFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${remote_cmd2}
    then
      echo -e "${GREEN}在103板卡上创建工程文件夹成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}在103板卡上创建工程文件夹错误.${NONE}"
      echo -e "${RED}更新103板卡的工程失败.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${package_name} ${target_user}@${TargetIP_103}:${remote_project}
      then
        echo -e "${GREEN}下载工程(resident.tar.gz)到103板卡成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}下载工程(resident.tar.gz)到103板卡错误.${NONE}"
        echo -e "${RED}更新103板卡的AVOS工程失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project};${remote_pw} rm -rf ${remote_project}/${project_name}/*;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${tar_cmd2}
      then
        echo -e "${GREEN}删除103板卡的旧工程文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除103板卡的旧工程文件失败.${NONE}"
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project};${remote_pw} tar -xzf ${remote_project}${package_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${tar_cmd2}
      then
        echo -e "${GREEN}解压103板卡的工程文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}解压103板卡的工程文件错误.${NONE}"
        echo -e "${RED}更新103板卡的工程失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${package_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${rm_cmd2}
      then
        echo -e "${GREEN}删除103板卡的工程更新包.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除103板卡的工程更新包错误.${NONE}"
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${chmod_cmd}
      then
        echo -e "${GREEN}给103板卡的工程文件加上最高访问权限.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}给103板卡的工程文件加上最高访问权限错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then      
      sync_cmd="${remote_pw} sync;"
      ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${sync_cmd}
      echo -e "${GREEN}更新103板卡的工程成功.${NONE}"
    else
      echo -e "${RED}更新103板卡的工程失败.${NONE}"
      return 0
    fi
  else
    echo -e "${RED}更新103板卡的工程失败.${NONE}"
    return 0
  fi
}

function updateLaunch()
{
  updateLaunch102
  updateLaunch103
}


function updateLaunch102()
{
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "开始更新102板卡的launch配置文件........"
    createRemoteRun102
    remote_cmd2="source ${remote_home}${remote_sh};mkLaunchFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd2}
    then
      echo -e "${GREEN}在102板卡上创建launch文件夹成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}在102板卡上创建launch文件夹错误.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${launch_name} ${target_user}@${TargetIP_102}:${remote_project}${project_name}/src/
      then
        echo -e "${GREEN}下载配置文件(launch.tar.gz)到102板卡成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}下载配置文件(launch.tar.gz)到102板卡错误.${NONE}"
        echo -e "${RED}更新102板卡的launch文件失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd1="cd ${remote_project}${project_name}/src/;${remote_pw} rm -rf ${remote_project}${project_name}/src/launch/*;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${rm_cmd1}
      then
        echo -e "${GREEN}删除102板卡的旧配置文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除102板卡的旧配置文件失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project}${project_name}/src/;${remote_pw} tar -xzf ${remote_project}${project_name}/src/${launch_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${tar_cmd2}
      then
        echo -e "${GREEN}解压102板卡的配置文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}解压102板卡的配置文件错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${project_name}/src/${launch_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${rm_cmd2}
      then
        echo -e "${GREEN}删除102板卡的配置更新包成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除102板卡的配置更新包错误.${NONE}"
        return 0
      fi 
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name}/src/launch;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${chmod_cmd}
      then
        echo -e "${GREEN}给102板卡的配置文件加上最高访问权限.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}给102板卡的配置文件加上最高访问权限错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then      
      sync_cmd="${remote_pw} sync;"
      ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${sync_cmd}
      echo -e "${GREEN}通过sync指令将新下载到102板卡的launch的数据写入磁盘.${NONE}"
    else
      echo -e "${RED}102板卡的sync指令执行失败.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      echo -e "${GREEN}更新102板卡的配置成功.${NONE}"
    else
      echo -e "${RED}更新102板卡的配置失败.${NONE}"
      return 0
    fi
  else
    return 0
  fi
}


function updateLaunch103()
{
  is_online
  if [ $online_103 == "true" ]
  then
    echo -e "开始更新103板卡的launch配置文件........"
    createRemoteRun103
    remote_cmd2="source ${remote_home}${remote_sh};mkLaunchFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${remote_cmd2}
    then
      echo -e "${GREEN}在103板卡上创建launch文件夹成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}在103板卡上创建launch文件夹错误.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${launch_name} ${target_user}@${TargetIP_103}:${remote_project}${project_name}/src/
      then
        echo -e "${GREEN}下载配置文件(launch.tar.gz)到103板卡成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}下载配置文件(launch.tar.gz)到103板卡错误.${NONE}"
        echo -e "${RED}更新103板卡的launch文件失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd1="cd ${remote_project}${project_name}/src/;${remote_pw} rm -rf ${remote_project}${project_name}/src/launch/*;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${rm_cmd1}
      then
        echo -e "${GREEN}删除103板卡的旧配置文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除103板卡的旧配置文件失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project}${project_name}/src/;${remote_pw} tar -xzf ${remote_project}${project_name}/src/${launch_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${tar_cmd2}
      then
        echo -e "${GREEN}解压103板卡的配置文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}解压103板卡的配置文件错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${project_name}/src/${launch_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${rm_cmd2}
      then
        echo -e "${GREEN}删除103板卡的配置更新包成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除103板卡的配置更新包错误.${NONE}"
        return 0
      fi 
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name}/src/launch;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${chmod_cmd}
      then
        echo -e "${GREEN}给103板卡的配置文件加上最高访问权限.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}给103板卡的配置文件加上最高访问权限错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then      
      sync_cmd="${remote_pw} sync;"
      ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${sync_cmd}
      echo -e "${GREEN}通过sync指令将新下载到102板卡的launch的数据写入磁盘.${NONE}"
    else
      echo -e "${RED}103板卡的sync指令执行失败.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      echo -e "${GREEN}更新103板卡的配置成功.${NONE}"
    else
      echo -e "${RED}更新103板卡的配置失败.${NONE}"
      return 0
    fi
  else
    return 0
  fi
}
# update map package
function updateMap()
{
  updateMap102
  updateMap103
}

function updateMap102()
{
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "开始更新102板卡的地图数据........"
    createRemoteRun102
    remote_cmd2="source ${remote_home}${remote_sh};mkMapFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd2}
    then
      echo -e "${GREEN}在102板卡上创建地图文件夹成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}在102板卡上创建地图文件夹错误.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${default_map_name} ${target_user}@${TargetIP_102}:${remote_project}${project_name}/src/
      then
        echo -e "${GREEN}下载地图(map.tar.gz)到102板卡成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}下载地图(map.tar.gz)到102板卡错误.${NONE}"
        echo -e "${RED}更新102板卡的地图失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd1="cd ${remote_project}${project_name}/src/;${remote_pw} rm -rf ${remote_project}${project_name}/src/map_file/*;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${rm_cmd1}
      then
        echo -e "${GREEN}删除102板卡的旧地图文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除102板卡的旧地图文件失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project}${project_name}/src/;${remote_pw} tar -xzf ${remote_project}${project_name}/src/${default_map_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${tar_cmd2}
      then
        echo -e "${GREEN}解压102板卡的地图文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}解压102板卡的地图文件错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${project_name}/src/${default_map_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${rm_cmd2}
      then
        echo -e "${GREEN}删除102板卡的地图更新包.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除102板卡的地图更新包错误.${NONE}"
        return 0
      fi 
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name}/src/map_file;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${chmod_cmd}
      then
        echo -e "${GREEN}给102板卡的地图文件加上最高访问权限.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}给102板卡的地图文件加上最高访问权限错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then      
      sync_cmd="${remote_pw} sync;"
      ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${sync_cmd}
      echo -e "${GREEN}通过sync指令将新下载到102板卡的地图的数据写入磁盘.${NONE}"
    else
      echo -e "${RED}102板卡的sync指令执行失败.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      echo -e "${GREEN}更新102板卡的地图成功.${NONE}"
    else
      echo -e "${RED}更新102板卡的地图失败.${NONE}"
      return 0
    fi
  else
    return 0
  fi
}


function updateMap103()
{
  is_online
  if [ $online_103 == "true" ]
  then
    echo -e "开始更新103板卡的地图数据(map.tar.gz)......."
    createRemoteRun103
    remote_cmd2="source ${remote_home}${remote_sh};mkMapFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${remote_cmd2}
    then
      echo -e "${GREEN}在103板卡上创建地图文件夹成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}在103板卡上创建地图文件夹错误.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${default_map_name} ${target_user}@${TargetIP_103}:${remote_project}${project_name}/src/
      then
        echo -e "${GREEN}下载地图(map.tar.gz)到103板卡成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}下载地图(map.tar.gz)到103板卡错误.${NONE}"
        echo -e "${RED}更新103板卡的地图失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd1="cd ${remote_project}${project_name}/src/;${remote_pw} rm -rf ${remote_project}${project_name}/src/map_file/*;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${rm_cmd1}
      then
        echo -e "${GREEN}删除103板卡的旧地图文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除103板卡的旧地图文件失败.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project}${project_name}/src/;${remote_pw} tar -xzf ${remote_project}${project_name}/src/${default_map_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${tar_cmd2}
      then
        echo -e "${GREEN}解压103板卡的地图文件成功.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}解压103板卡的地图文件错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${project_name}/src/${default_map_name};"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${rm_cmd2}
      then
        echo -e "${GREEN}删除103板卡的地图更新包.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}删除103板卡的地图更新包错误.${NONE}"
        return 0
      fi 
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name}/src/map_file;"
      if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${chmod_cmd}
      then
        echo -e "${GREEN}给103板卡的地图文件加上最高访问权限.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}给103板卡的地图文件加上最高访问权限错误.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then      
      sync_cmd="${remote_pw} sync;"
      ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${sync_cmd}
      echo -e "${GREEN}通过sync指令将新下载到103板卡的地图的数据写入磁盘.${NONE}"
    else
      echo -e "${RED}103板卡的sync指令执行失败.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      echo -e "${GREEN}更新103板卡的地图成功.${NONE}"
    else
      echo -e "${RED}更新103板卡的地图失败.${NONE}"
      return 0
    fi
  else
    return 0
  fi
}

# update map package
function updateScripts()
{
  updateScript102
  updateScript103
}


function updateScript102()
{
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "开始更新102板卡的自启动脚本(start_avos.sh)......"
    tar -zcvf ${start_avos_script} start_avos.sh

    if ${ssh_pass} scp ${remove_yes} ${start_avos_script} ${target_user}@${TargetIP_102}:${remote_home}
    then
      echo -e "${GREEN}拷贝自启动脚本到102板卡上成功.${NONE}"
    else
      echo -e "${RED}拷贝自启动脚本到102板卡上失败.${NONE}"
      return 0
    fi

    tar_cmd="tar -xzf ${start_avos_script}"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${tar_cmd}
    then 
      echo -e "${GREEN}解压102板卡上的自启动脚本成功.${NONE}"
    else
      echo -e "${RED}解压102板卡上的自启动脚本错误.${NONE}"
      return 0
    fi

    createRemoteRun102
    remote_cmd2="source ${remote_home}${remote_sh};updateScript ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd2}
    then
      echo -e "${GREEN}更新102板卡的自启动脚本成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}更新102板卡的自启动脚本失败.${NONE}"
      return 0
    fi
  else
    return 0
  fi
}


function updateScript103()
{
  is_online
  if [ $online_103 == "true" ]
  then
    echo -e "开始更新103板卡的自启动脚本(start_avos.sh)......"
    tar -zcvf ${start_avos_script} start_avos.sh

    if ${ssh_pass} scp ${remove_yes} ${start_avos_script} ${target_user}@${TargetIP_103}:${remote_home}
    then
      echo -e "${GREEN}拷贝自启动脚本到103板卡上成功.${NONE}"
    else
      echo -e "${RED}拷贝自启动脚本到103板卡上失败.${NONE}"
      return 0
    fi

    tar_cmd="tar -xzf ${start_avos_script}"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${tar_cmd}
    then 
      echo -e "${GREEN}解压103板卡上的自启动脚本成功.${NONE}"
    else
      echo -e "${RED}解压103板卡上的自启动脚本错误.${NONE}"
      return 0
    fi

    createRemoteRun103
    remote_cmd2="source ${remote_home}${remote_sh};updateScript;"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_103} -t ${remote_cmd2}
    then
      echo -e "${GREEN}更新103板卡的自启动脚本成功.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}更新103板卡的自启动脚本失败.${NONE}"
      return 0
    fi
  else
    return 0
  fi
}


# create project package
function makeOta2(){
  echo "sudo to mkdir ../.temp:"
  echo ${local_pw} | sudo -S mkdir -p ../.temp/${project_name}
  echo "clear ../.temp ..."
  sudo chmod 777 ../.temp -R
  sudo rm ../.temp/${project_name}/* -fr
  echo "copy files to ../.temp/${project_name} ..."
  sudo cp ./src ../.temp/${project_name} -r
  sudo cp ./devel ../.temp/${project_name} -r
  sudo cp ./build ../.temp/${project_name} -r
  #TODO:
  sudo cp ./${start_launch_file} ../.temp/${project_name}
  sudo cp ./${readme} ../.temp/${project_name}
  sudo cp ./${function_point} ../.temp/${project_name}

  origin_path=`pwd`
  cd ../.temp/${project_name}
  sudo chmod 777 ./* -R
  
  echo "clear tmp files ... "
  sudo chmod 777 ./* -R
  sudo find src/ -name '*.cpp'|xargs rm -fr
  sudo find src/ -name '*.cc'|xargs rm -fr
  sudo find src/ -name '*.c'|xargs rm -rf
  sudo find src/ -name '*.h'|xargs rm -fr
  sudo find src/ -name '*.hpp'|xargs rm -fr
  sudo find src/ -name '*.msg'|xargs rm -fr
  sudo find src/ -name '*.java'|xargs rm -fr
  sudo find src/ -name '*.gradle'|xargs rm -fr
  sudo find src/ -name '*.properties'|xargs rm -fr
  sudo find build/ -name '*.cmake'|xargs rm -fr
  sudo find build/ -name '*.make'|xargs rm -fr
  sudo find build/ -name '*Makefile'|xargs rm -fr
  sudo find . -name 'CMakeLists.txt'|xargs rm -fr
  sudo find . -name '*.cu'|xargs rm -fr
  sudo find . -name '*.cuh'|xargs rm -fr
  sudo find . -name '*.tar.gz'|xargs rm -fr
  sudo find . -name '*.tar'|xargs rm -fr
  sudo find . -name '*.swp'|xargs rm -fr
  sudo find . -name '*~'|xargs rm -fr
  find . -name *.o | xargs rm -fr
  find src/ -name '.git'|xargs rm -fr

  rm devel/include/ -fr

  find src/ -name '*.msg'|xargs rm -fr
  find . -name '*.cfi' | xargs rm -fr
  find . -name 'srv' | xargs rm -fr
  find . -name '*.cmake'|xargs rm -fr

  rm devel/share -fr

  #readALLfile .
  sudo chmod 777 ./* -R
  echo "return to ${origin_path}"
  cd $origin_path
  # MAKE TAR:
  tar_name="${project_name}.tar.gz"
  cd ../.temp
  tar -czf $tar_name ${project_name}
  cp $tar_name $origin_path
  echo "clear ../.temp..."
  rm * -fr 
  cd $origin_path
  return 0
}

# create ota project package
function download(){
  echo "Start to make ota package..."
  makeOta2
  echo "Make the ota package done."
  echo "*************************************"
  echo "Start to download package to vehicle ......"
  updateProject
  cd ${origin_path}
  rm ${package_name}
  rm -rf remote_run.tar.gz
}

# create ota project package
function createOtaTar(){
  echo "Begin to make ota package..."
  makeOta2
  echo "Make ota package done."
  # echo "*************************************"
  # echo "Will create a release package ......"
  # createRelease
}

function createRelease(){
  cd $origin_path
  release_version=$1

  project_version_file=$origin_path/ReadMe.md
  if  test -e $project_version_file
  then
      arg_avos=`cat ${project_version_file} | grep version `
      release_version=`echo ${arg_avos#*:} `
      release_version=`echo ${release_version#*\"} `
      release_version=`echo ${release_version%%\"} `
      echo "project version : ${release_version}"
  else
      echo -e "${RED}there is no ReadMe.md file ${NONE}"
      return 0
  fi


  file=$origin_path/updatecar
  md5=`md5sum $origin_path/resident.tar.gz | cut -d " " -f1`
  echo "the file resident.tar.gz md5 is $md5 "
  if test -e $file
  then
    # read -t 30 -p "the avos already exist , are you sure want to update ?(yes/no)" answer
    cd updatecar
    echo "resident.tar.gz:$md5" > checklist.dat
    echo "resident.tar.gz:/work/share/project:$release_version" > updatelist.dat
    cd $origin_path
  else
    # read -t 30 -p "the avos is not exist , do you want to update ?(yes/no)" answer
    mkdir updatecar
    cd updatecar
    echo "resident.tar.gz:$md5" > checklist.dat
    echo "resident.tar.gz:/work/share/project:$release_version" > updatelist.dat
  fi
  cd $origin_path

  cp resident.tar.gz updatecar
  tar -czf updatecar.tar.gz updatecar

  rm -rf updatecar

  echo "The release package done !"
}


function restart()
{
  restart102
  restart103
}

function restart102()
{
  cmd_reboot="${remote_pw} reboot;"
  echo "开始重启102板卡........"
  ${ssh_pass} ssh ${remove_yes} ${target_user}@${TargetIP_102} -t ${cmd_reboot}
}

function restart103()
{
  cmd_reboot="${remote_pw} reboot;"
  echo "开始重启103板卡........"
  ${ssh_pass} ssh ${remove_yes} ${target_user}@${TargetIP_103} -t ${cmd_reboot}
}


function backup()
{  
  echo -e "${YELL}===begin backup avos , map ===${NONE}"
  echo -e "${YELL}********** backup avos *********${NONE}"
  backup_avos
  # backup_project
  # backup_ota
  echo -e "${YELL}********** backup map *********${NONE}"
  backup_map
  # record_sh="record.sh"
  # cmd_record="source ${remote_home}${record_sh};start_record" # nohub start &   &> /dev/null &
  # echo -e "${BLUE}"
  # # if ${ssh_pass} ssh -o ServerAliveInterval=30 ${target_user}@${TargetIP_102} -t ${cmd_record}
  # if ${ssh_pass} ssh -fn ${target_user}@${TargetIP_102} -t ${cmd_record}
  # then
  #   echo -e "${GREEN}start record ok ${NONE}"
  # else
  #   echo -e "${RED}record error${NONE}"
  # fi
  echo -e "${YELL}********** backup stop *********${NONE}"
}

function backup_avos()
{
  echo "start backup avos"
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "${BLUE}begin backup avos..."
    createRemoteRun102
    remote_cmd="source ${remote_home}${remote_sh};back_up_avos;"
    echo -e "${BLUE}"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}102 source ok${NONE}" 
    else
      echo -e "${RED}102 source error${NONE}" 
      return 0
    fi
    echo -e "${BLUE}"
    if ${ssh_pass} scp ${target_user}@${TargetIP_102}:/home/nvidia/.ros/avos.tar.gz .
    then
      echo -e "${GREEN}102 upload avos ok${NONE}"
    else
      echo -e "${RED}102 upload avos error${NONE}"
      return 0
    fi
    deleteRemoteRun102
  else
    return 0
  fi
}

function backup_project()
{
  echo "${TargetIP_102} -->Start to backup the project."
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "${TargetIP_102} --> Start to backup the project..."
    createRemoteRun102
    remote_cmd="source ${remote_home}${remote_sh};back_up_project ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}${TargetIP_102} --> source remote script is ok.${NONE}" 
    else
      echo -e "${RED}${TargetIP_102} --> source remote script is error.${NONE}" 
      return 0
    fi
    if ${ssh_pass} scp ${target_user}@${TargetIP_102}:/work/share/project/${project_name}/${project_name}.tar.gz .
    then
      echo -e "${GREEN}${TargetIP_102} --> Uploading project is ok.${NONE}"
    else
      echo -e "${RED}${TargetIP_102} --> Uploading project is error.${NONE}"
      return 0
    fi
    deleteRemoteRun102
  else
    return 0
  fi
}

function backup_ota()
{
  echo "start backup ota"
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "${BLUE}begin backup ota..."
    createRemoteRun102
    remote_cmd="source ${remote_home}${remote_sh};back_up_ota ${target_pass_word} ${project_name};"
    echo -e "${BLUE}"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}102 source ok${NONE}" 
    else
      echo -e "${RED}102 source error${NONE}" 
      return 0
    fi
    echo -e "${BLUE}"
    if ${ssh_pass} scp ${target_user}@${TargetIP_102}:/work/share/project/ota/ota.tar.gz .
    then
      echo -e "${GREEN}102 upload ota ok${NONE}"
    else
      echo -e "${RED}102 upload ota error${NONE}"
      return 0
    fi
    deleteRemoteRun102
  else
    return 0
  fi
}

function backup_local_map()
{
  echo "${TargetIP_102} --> Start to backup current map file ."
  is_online
  if [ $online_102 == "true" ]
  then
    remote_cmd="source ${remote_home}${remote_sh};backup_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}${TargetIP_102} --> package current map file is ok.${NONE}" 
    else
      echo -e "${RED}${TargetIP_102} --> package current map file is error.${NONE}" 
      return 0
    fi
  else
    return 0
  fi
}

function recover_local_map()
{
  echo "${TargetIP_102} --> Start to recover current map file."
  is_online
  if [ $online_102 == "true" ]
  then
    remote_cmd="source ${remote_home}${remote_sh};recover_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}${TargetIP_102} --> recover current map file is ok.${NONE}" 
    else
      echo -e "${RED}${TargetIP_102} --> recover current map file is error.${NONE}" 
      return 0
    fi
  else
    return 0
  fi
}

function backup_map()
{
  echo "${TargetIP_102} --> Start to backup the map."
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "${TargetIP_102} --> Start to backup the map..."
    createRemoteRun102
    remote_cmd="source ${remote_home}${remote_sh};back_up_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}${TargetIP_102} --> source remote script is ok.${NONE}" 
    else
      echo -e "${RED}${TargetIP_102} --> source remote script is error.${NONE}" 
      return 0
    fi
    if ${ssh_pass} scp ${target_user}@${TargetIP_102}:/work/share/project/${project_name}/src/map_file.tar.gz
    then
      echo -e "${GREEN}${TargetIP_102} --> Uploading map package is ok.${NONE}"
    else
      echo -e "${RED}${TargetIP_102} --> Uploading map package is error.${NONE}"
      return 0
    fi
    deleteRemoteRun102
  else
    return 0
  fi
}


function backup_launch()
{
  echo "${TargetIP_102} --> Start to backup launch."
  is_online
  if [ $online_102 == "true" ]
  then
    echo -e "${TargetIP_102} --> Start to backup launch..."
    createRemoteRun102
    remote_cmd="source ${remote_home}${remote_sh};back_up_launch ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}${TargetIP_102} --> source remote script is ok.${NONE}" 
    else
      echo -e "${RED}${TargetIP_102} --> source remote script is error.${NONE}" 
      return 0
    fi
    if ${ssh_pass} scp ${target_user}@${TargetIP_102}:/work/share/project/${project_name}/src/launch.tar.gz
    then
      echo -e "${GREEN}${TargetIP_102} --> Uploading launch package is ok.${NONE}"
    else
      echo -e "${RED}${TargetIP_102} --> Uploading launch package is error.${NONE}"
      return 0
    fi
    deleteRemoteRun102
  else
    return 0
  fi
}

function rosnode_run()
{  
  local node_name=$2
  local target_ip=${TargetIP_102}
  is_online
  if [ $online_102 == "true" ]
  then
    if [ ${node_name} == "ivlocalization" ] ; then
      target_ip=${TargetIP_103}
      createRemoteRun103
      echo "${target_ip} --> Start to run rosnode ${node_name}."
    elif [ ${node_name} == "perception"]; then
      target_ip=${TargetIP_103}
      createRemoteRun103
    elif [ ${node_name} == "planning"]; then
      target_ip=${TargetIP_102}
      createRemoteRun102
    elif [ ${node_name} == "control"]; then
      target_ip=${TargetIP_102}
      createRemoteRun102
    elif [ ${node_name} == "canbus"]; then
      target_ip=${TargetIP_102}
      createRemoteRun102
    elif [ ${node_name} == "business_platform"]; then
      target_ip=${TargetIP_102}
      createRemoteRun102
    elif [ ${node_name} == "ivcamera"]; then
      target_ip=${TargetIP_103}
      createRemoteRun103
    elif [ ${node_name} == "lslidar"]; then
      target_ip=${TargetIP_102}
      createRemoteRun102
    elif [ ${node_name} == "ars408"]; then
      target_ip=${TargetIP_102}
      createRemoteRun102
    else [ ${node_name} == "imu"];
      target_ip=${TargetIP_102}
      createRemoteRun102
    fi

    remote_cmd="source ${remote_home}${remote_sh};rosnode_run ${node_name};"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}${target_ip} --> source remote script is ok ${remote_cmd}.${NONE}" 
    else
      echo -e "${RED}${target_ip} --> source remote script is error ${remote_cmd}.${NONE}" 
      return 0
    fi
  else
    return 0
  fi
}

function rosnode_kill()
{
  local node_name=$2
  is_online
  if [ $online_102 == "true" ]
  then
    createRemoteRun102
    remote_cmd="source ${remote_home}${remote_sh};rosnode_kill ${node_name};"
    if ${ssh_pass} ssh ${target_user}@${TargetIP_102} -t ${remote_cmd}
    then
      echo -e "${GREEN}${target_ip} --> source remote script is ok.${NONE}" 
    else
      echo -e "${RED}${target_ip} --> source remote script is error.${NONE}" 
      return 0
    fi
  else
    return 0
  fi
}

# print
function print_usage() {
  RED='\033[0;31m'
  BLUE='\033[0;34m'
  BOLD='\033[1m'
  NONE='\033[0m'

  echo -e "\n${BLUE}${BOLD}Usage${NONE}:
  ${BOLD} deploy ${NONE} ${BOLD}[OPTION]"

  echo -e "\n${BLUE}${BOLD}Options${NONE}:
  ${RED}${BOLD}-h|-help${NONE}: ${BOLD}                 查看帮助信息
  ${RED}${BOLD}-all|-all${NONE}: ${BOLD}                工程一键部署，中间可能会有多次密码的输入，请按提示操作
  ${RED}${BOLD}-d|-download${NONE}: ${BOLD}             创建工程包resident.tar.gz并下载工程到102和103板卡，无源码不可用
  ${RED}${BOLD}-p|-updateProject${NONE}: ${BOLD}        下载已经创建好的工程包resident.tar.gz到102和103板卡
  ${RED}${BOLD}-m|-map${NONE}: ${BOLD}                  更新已经打包好的地图文件(map.tar.gz)
  ${RED}${BOLD}-c|-create${NONE}: ${BOLD}               从源码工程中提取制作AVOS工程包--resident.tar.gz，无源码不可用
  ${RED}${BOLD}-r|-restart${NONE}: ${BOLD}              软重启102和103板卡
  ${RED}${BOLD}-bp|-backup_project${NONE}: ${BOLD}      备份工程和地图
  ${RED}${BOLD}-bm|-backup_map${NONE}: ${BOLD}          仅备份地图
  ${RED}${BOLD}-k|-addsshkey${NONE}: ${BOLD}            将本地公钥加入到102和103板卡，102板卡的公钥加入到103板卡，实现102到103无需密码即可拷贝
  ${RED}${BOLD}-k102|-addsshkey${NONE}: ${BOLD}         将本地公钥加入到102板卡，实现本地到102无需密码可操作
  ${RED}${BOLD}-k103|-addsshkey${NONE}: ${BOLD}         将本地公钥加入到103板卡，实现本地到102无需密码可操作
  ${RED}${BOLD}-k102_103|-addsshkey${NONE}: ${BOLD}     102板卡的公钥加入到103板卡，实现102到103无需密码即可拷贝
  ${RED}${BOLD}-sh|-updateScripts${NONE}: ${BOLD}       更新自启动脚本(start_avos.sh)
  ${RED}${BOLD}-t|-update_target_point${NONE}: ${BOLD}  更新存储目标功能点的坐标(function_point.yaml)
  ${RED}${BOLD}-l|-updateLaunch${NONE}: ${BOLD}         更新工程的launch配置文件(launch.tar.gz)
  ${RED}${BOLD}-bl|-backup_launch${NONE}: ${BOLD}       备份launch配置文件
  Version:${version}
  "
}

# main function
function deploy() {
  #source devel/setup.bash
  RED='\033[0;31m'
  BLUE='\033[0;34m'
  BOLD='\033[1m'
  NONE='\033[0m'  

  local cmd=$1
  shift  
  case $cmd in
    -h|-help)
      print_usage $@
      ;;
    -all|-all)
      updateAll $@
      ;;
    -d|-download)
      download $@
      ;;
    -m|-map)
      updateMap $@
      ;;
    -l|-launch)
      updateLaunch $@
      ;;
    -c|-create)
      createOtaTar $@
      ;;
    -p|-updateProject)
      updateProject $@
      ;;
    -r|-restart)
      restart $@
      ;;
    -bp|-backup_project)
      backup_project $@
      ;;
    -bm|-backup_map)
      backup_map $@
      ;;
    -bl|-backup_launch)
      backup_launch $@
      ;;
    -k|-addsshkey)
      add_key $@
      ;;
    -k102|-addsshkey)
      add_key_local_to_102 $@
      ;;
    -k103|-addsshkey)
      add_key_local_to_103 $@
      ;;
    -k102_103|-addsshkey)
      add_key_102_to_103 $@
      ;;
    -sh|-updateScripts)
      updateScripts $@
      ;;
    -t|-update_target_point)
      update_target_point $@
      ;;
    -kill|-rosnode_kill)
      rosnode_kill $@
      ;;
    -run|-rosnode_run)
      rosnode_run $@
      ;;
    *)
      print_usage
      ;;
  esac
}
