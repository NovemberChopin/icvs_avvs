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


version="0.1.2"
# ****** default parameter ******
target_user="idriver"
# target_user="dlonghui" #debug
target_ip="192.168.1.101"
# target_ip="192.168.1.47" #debug
target_online="false"
remote_project="/work/share/project/"
exe_status="false"
# ****** default file path ******
origin_path=`pwd`
remote_home="/home/${target_user}/"
remote_ota="ota/"
target_pass_word="idriver"
project_name=$(pwd |awk -F '/' '{print $NF}')
package_name="${project_name}.tar.gz"

# ****** default file name ******
default_map_name="map_file.tar.gz"
default_ota_name="ota.tar.gz"
default_avos_name="avos.tar.gz"
remote_sh="remote_run.sh"
versioncheck_sh="version_check.sh"
remote_name="remote_run.tar.gz"
version_check="version_check.tar.gz"
start_launch_file="landship.sh"
readme="README.md"
program_exe="landship.desktop"

# ****** default password ******
remote_pw="echo "${target_pass_word}" | sudo -S"
# remote_pw="echo "root" | sudo -S" #debug
ssh_pass="sshpass -p ${target_pass_word}"
# ssh_pass="sshpass -p root" #debug
remove_yes="-o StrictHostKeyChecking=no"
local_pw="idriver_bbox"

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
  target_ip_102=${target_ip}
  if ping -c1 $target_ip_102
  then
    echo -e "${GREEN}${target_ip} --> The target vehicle is online , go ahead ...${NONE}"
    target_online="true"
  else
    echo -e "${RED}${target_ip} --> The target device is offline , please check the connect!${NONE}"
    target_online="false"
  fi
}

function add_key() {
  if [ $# != 2 ] ; then
    this_user=${target_user}
    this_ip=${target_ip}
  else
    this_user=$1
    this_ip=$2
  fi
  local_key_name="/home/idriver/.ssh/id_rsa.pub"
  remote_key_name="/home/${this_user}/.ssh/id_rsa.remote"
  target_pre="${this_user}@${this_ip}"
  cmd_cp_key="${target_pre}:${remote_key_name}"

  echo -e "We are going to add key."
  scp ~/.ssh/id_rsa.pub ${cmd_cp_key}

  echo -e "Adding your key to authorized_keys..."
  cmd_add="cat ${remote_key_name} >> ~/.ssh/authorized_keys ; rm ${remote_key_name}; sudo chmod 600 /home/${this_user}/.ssh/authorized_keys"
  echo -e "We are going to ssh target device."
  ssh ${target_pre} -t ${cmd_add}
  return_var=$?
  if [ ${return_var} != 0 ] ; then
    echo -e "${RED}Faild to add key.${NONE}"
  else
    echo -e "${GREEN}Key added.${NONE}"
  fi
}

# create bash shell in the remote
function createRemoteRun()
{
  cd ${origin_path}
  #tar -czf ${remote_name} ${remote_sh} 
  tar_cmd="tar -xzf ${remote_name}"
  if [ $target_online == "true" ]
  then
    echo -e "${target_ip} -->Start to create remote_run.sh."
    if ${ssh_pass} scp ${remove_yes} ${remote_name} ${target_user}@${target_ip}:${remote_home}
    then
      echo -e "${GREEN}${target_ip} -->Copy remote_run.sh bash is ok.${NONE}"
    else
      echo -e "${RED}${target_ip} -->Copy remote_run.sh bash is error.${NONE}"
      return 0
    fi
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${tar_cmd}
    then 
      echo -e "${GREEN}${target_ip} -->Extract remote_run.tar.gz bash is ok.${NONE}"
    else
      echo -e "${RED}${target_ip} -->Extract remote_run.tar.gz bash is error.${NONE}"
      return 0
    fi
    echo -e "${GREEN}${target_ip} -->Create remote_run.sh finished.${NONE}"
  else
    return 0
  fi
}
# create bash shell in the remote
function createRemoteCheck()
{
  cd ${origin_path}
  #tar -czf ${remote_name} ${remote_sh} 
  
  tar_cmd="tar -xzf ${version_check}"
  if [ $target_online == "true" ]
  then
    echo -e "${target_ip} --> begin createRemoteCheck."
    if ${ssh_pass} scp ${remove_yes} ${version_check} ${target_user}@${target_ip}:${remote_home}
    then
      echo -e "${target_ip} -->copy remote bash is ok."
    else
      echo -e "${target_ip} -->copy remote bash is error."
      return 0
    fi
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${tar_cmd}
    then 
      echo -e "${target_ip} -->Extract remote check bash is ok."
    else
      echo -e "${target_ip} -->Extract remote ckeck bash is error."
      return 0
    fi
    echo -e "${target_ip} -->createRemoteCheck finished."
  else
    return 0
  fi
}
# delete bash shell in the remote
function deleteRemoteRun()
{
  de_cmd="cd ${remote_home};rm ${remote_sh};rm ${remote_name}"
  if [ $target_online == "true" ]
  then
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${de_cmd}
    then
      echo -e "${GREEN}${target_ip} --> Delete remote package and bash is ok.${NONE}"
    else
      echo -e "${RED}${target_ip} --> Delete remote package and bash is error.${NONE}"
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
  echo -e "${YELL}===begin update all , project , ota , map ,and avos===${NONE}"
  echo -e "${YELL}********** project *********${NONE}"
  updateProject
  echo -e "${YELL}********** ota *********${NONE}"
  updateOta
  echo -e "${YELL}********** avos *********${NONE}"
  updateAvos
  echo -e "${YELL}********** map *********${NONE}"
  updateMap
  echo -e "${YELL}********** stop *********${NONE}\n"
}

# update project package
function updateProject()
{
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "${target_ip} --> Start to download the project ..."
    createRemoteRun
    remote_cmd2="source ${remote_home}${remote_sh};mkProjectFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd2}
    then
      echo -e "${GREEN}${target_ip} --> Source remote script is ok.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}${target_ip} --> Source remote script is error.${NONE}"
      echo -e "${RED}${target_ip} --> The project download is failed .${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${package_name} ${target_user}@${target_ip}:${remote_project}
      then
        echo -e "${GREEN}${target_ip} --> Scp project package to vehicle is OK.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Scp project package to vehicle is error.${NONE}"
        echo -e "${RED}${target_ip} --> The project download is failed .${NONE}"
        return 0
      fi
    else
      return 0
    fi

    # backup_local_map
    echo "${target_ip} --> Start to backup current map files..."
    if [ $exe_status == "true" ]
    then
      remote_cmd="source ${remote_home}${remote_sh};backup_local_map ${target_pass_word} ${project_name};"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
      then
        echo -e "${GREEN}${target_ip} --> package current map files from project is ok.${NONE}" 
      else
        echo -e "${RED}${target_ip} --> package current map files from project is error.${NONE}" 
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project};${remote_pw} rm -rf ${remote_project}/${project_name}/*;"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${tar_cmd2}
      then
        echo -e "${GREEN}${target_ip} --> Delete old project files is OK.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Delete old project files is error.${NONE}"
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project};${remote_pw} tar -xzf ${remote_project}${package_name};"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${tar_cmd2}
      then
        echo -e "${GREEN}${target_ip} --> Extract project package is OK.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Extract project package is error.${NONE}"
        echo -e "${RED}${target_ip} --> The project download is failed .${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${package_name};"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${rm_cmd2}
      then
        echo -e "${GREEN}${target_ip} --> Delete project package is ok.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Delete project package is error.${NONE}"
      fi
    else
      return 0
    fi

    # recover_local_map
    echo "${target_ip} --> Start to recover current map files to project..."
    if [ $exe_status == "true" ]
    then
      remote_cmd="source ${remote_home}${remote_sh};recover_local_map ${target_pass_word} ${project_name};"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
      then
        echo -e "${GREEN}${target_ip} --> Recover current map file to project is ok.${NONE}" 
      else
        echo -e "${RED}${target_ip} --> Recover current map file to project is error.${NONE}" 
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name};"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${chmod_cmd}
      then
        echo -e "${GREEN}${target_ip} --> Chmod project is ok.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Chmod project is error.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      echo -e "${GREEN}${target_ip} --> The project download is successful.${NONE}"
    else
      echo -e "${RED}${target_ip} --> The project download is failed .${NONE}"
      return 0
    fi
  else
    echo -e "${RED}${target_ip} --> The project download is failed .${NONE}"
    return 0
  fi
}

# check version info
function checkversion()
{
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "\n${BLUE}begin check version info 102 ..."
    createRemoteCheck
    echo -e "${BLUE}"
    remote_cmd_version="source ${remote_home}${versioncheck_sh};check;"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd_version}
    then
      echo -e "${GREEN}102 source ok${NONE}" 
    else
      echo -e "${RED}102 source error${NONE}"
      return 0
    fi
    echo -e "\n${GREEN}The version info update finished 102.${NONE}\n"
  else
    return 0
  fi
}

#TODO

# update map package
function updateMap()
{
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "${target_ip} --> Start to update the map  ..."
    createRemoteRun
    remote_cmd2="source ${remote_home}${remote_sh};mkMapFile ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd2}
    then
      echo -e "${GREEN}${target_ip} --> Source remote script is ok.${NONE}" 
      exe_status="true"
    else
      echo -e "${RED}${target_ip} --> Source remote script is error.${NONE}"
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      if ${ssh_pass} scp ${default_map_name} ${target_user}@${target_ip}:${remote_project}${project_name}/src/
      then
        echo -e "${GREEN}${target_ip} --> Scp map package to vehicle is OK.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Scp map package to vehicle is error.${NONE}"
        echo -e "${RED}${target_ip} --> Download map package to vehicle is failed.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd1="cd ${remote_project}${project_name}/src/;${remote_pw} rm -rf ${remote_project}${project_name}/src/map_file/*;"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${rm_cmd1}
      then
        echo -e "${GREEN}${target_ip} --> Delete old map files from project is ok.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Delete old map files from project is error.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      tar_cmd2="cd ${remote_project}${project_name}/src/;${remote_pw} tar -xzf ${remote_project}${project_name}/src/${default_map_name};"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${tar_cmd2}
      then
        echo -e "${GREEN}${target_ip} --> Extract map package to project is OK.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Extract map package to project is error.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      rm_cmd2="${remote_pw} rm ${remote_project}${project_name}/src/${default_map_name};"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${rm_cmd2}
      then
        echo -e "${GREEN}${target_ip} --> Delete map package is ok.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Delete map package is error.${NONE}"
        return 0
      fi 
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      exe_status="false"
      chmod_cmd="${remote_pw} chmod -R 777 ${remote_project}${project_name}/src/map;"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${chmod_cmd}
      then
        echo -e "${GREEN}${target_ip} --> Chmod map is ok.${NONE}"
        exe_status="true"
      else
        echo -e "${RED}${target_ip} --> Chmod map is error.${NONE}"
        return 0
      fi
    else
      return 0
    fi

    if [ $exe_status == "true" ]
    then
      echo -e "${GREEN}${target_ip} --> The map updating have finished .${NONE}"
    else
      echo -e "${RED}${target_ip} --> The map updating faild .${NONE}"
      return 0
    fi
  else
    return 0
  fi
}

# update ota package
function updateOta()
{
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "${BLUE}begin update ota..."
    createRemoteRun
    remote_cmd="source ${remote_home}${remote_sh};mkOtaFile;"
    echo -e "${BLUE}"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}102 source ok${NONE}" 
    else
      echo -e "${RED}102 source error${NONE}" 
      return 0
    fi
    echo -e "${BLUE}"
    if ${ssh_pass} scp ${default_ota_name} ${target_user}@${target_ip}:${remote_project}
    then
      echo -e "${GREEN}102 copy ota ok${NONE}"
    else
      echo -e "${RED}102 copy ota error${NONE}"
      return 0
    fi
    tar_cmd="cd ${remote_project};${remote_pw} tar -xzf ${remote_project}${default_ota_name}"
    echo -e "${BLUE}"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${tar_cmd}
    then
      echo -e "${GREEN}102 extract ota ok${NONE}"
    else
      echo -e "${RED}102 extract ota error${NONE}"
      return 0
    fi
    rm_cmd="${remote_pw} rm ${remote_project}${default_ota_name};"
    echo -e "${BLUE}"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${rm_cmd}
    then
      echo -e "${GREEN}102 delete ota package ok${NONE}"
    else
      echo -e "${RED}102 delete ota package error${NONE}"
      return 0
    fi

    ssh_host=${target_user}@${target_ip}
    file="/work/share/project/ota/src"
    if ${ssh_pass} ssh $ssh_host test -e $file
    then
      echo -e "${GREEN}The 102 ota update finished .${NONE}"
    else
      echo "102"
      # echo -e "${RED}The 102 ota update fault .${NONE}"
    fi
  else
    return 0
  fi
  echo -e "${GREEN}The ota update finished 102.${NONE}"

}
# update avos package
function updateAvos()
{
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "${BLUE}we will update avos${NONE}"
    ssh_host=${target_user}@${target_ip}
    file="~/.ros/avos"
    if ${ssh_pass} ssh $ssh_host test -e $file
    then
      read -t 30 -p "the avos already exist , are you sure want to update ?(yes/no)" answer
    else
      read -t 30 -p "the avos is not exist , do you want to update ?(yes/no)" answer
    fi

    if [ ${answer} == "yes" ]
    then
      createRemoteRun
      remote_cmd="source ${remote_home}${remote_sh};mkAvosFile;"
      echo -e "${BLUE}"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
      then
        echo -e "${GREEN}102 source ok${NONE}" 
      else
        echo -e "${RED}102 source error${NONE}" 
        return 0
      fi
      echo -e "${BLUE}"
      if ${ssh_pass} scp ${default_avos_name} ${target_user}@${target_ip}:${remote_home}.ros/
      then
        echo -e "${GREEN}102 copy avos ok${NONE}"
      else
        echo -e "${RED}102 copy avos error${NONE}"
        return 0
      fi
      tar_cmd="cd ${remote_home}.ros;${remote_pw} tar -xzf ${remote_home}.ros/${default_avos_name}"
      echo -e "${BLUE}"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${tar_cmd}
      then
        echo -e "${GREEN}102 extract avos ok${NONE}"
      else
        echo -e "${RED}102 extract avos error${NONE}"
        return 0
      fi
      rm_cmd="${remote_pw} rm ${remote_home}.ros/${default_avos_name};"
      echo -e "${BLUE}"
      if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${rm_cmd}
      then
        echo -e "${GREEN}102 delete avos package ok${NONE}"
      else
        echo -e "${RED}102 delete avos package error${NONE}"
        return 0
      fi
      echo -e "${GREEN}The avos update finished .${NONE}"
      deleteRemoteRun
      echo -e "${BLUE}"
      if ${ssh_pass} ssh $ssh_host test -e $file
      then
        echo -e "${GREEN}The avos update finished .${NONE}"
      else
        echo -e "${RED}The avos update fault .${NONE}"
      fi
    else [ ${answer} == "no" ]
      return 0
    fi
  else
    return 0
  fi  
}
# create project package
function makeOta(){
  echo "sudo to mkdir ../.temp:"
  echo ${local_pw} | sudo -S mkdir -p ../.temp/t_for_rm
  echo "clear ../.temp ..."
  sudo chmod 777 ../.temp -R
  sudo rm ../.temp/* -fr
  echo "copy files to ../.temp ..."
  sudo cp ./src ../.temp -r
  sudo cp ./devel ../.temp -r
  sudo cp ./build ../.temp -r
  #TODO:
  sudo cp ./${start_launch_file} ../.temp
  sudo cp ./${readme} ../.temp
  sudo cp ./${program_exe} ../.temp
  sudo cp ./version ../.temp -r

  origin_path=`pwd`
  cd ../.temp
  sudo chmod 777 ./* -R
  
  echo "clear tmp files ... "
  sudo chmod 777 ./* -R
  sudo find src/ -name '*.cpp'|xargs rm -fr
  sudo find src/ -name '*.cc'|xargs rm -fr
  sudo find src/ -name '*.c'|xargs rm -rf
  sudo find src/ -name '*.h'|xargs rm -fr
  sudo find src/ -name '*.hpp'|xargs rm -fr
  sudo find src/ -name '*.msg'|xargs rm -fr
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
  tar_name='resident.tar.gz'
  cd ../.temp
  tar -czf $tar_name *
  cp $tar_name $origin_path
  echo "clear ../.temp..."
  rm * -fr 
  cd $origin_path
  return 0
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
  sudo cp ./${program_exe} ../.temp/${project_name}

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
  tar -zcvf remote_run.tar.gz remote_run.sh
  echo "Start to make ota package..."
  # makeOta
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
  # makeOta
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


# print
function print_usage() {
  RED='\033[0;31m'
  BLUE='\033[0;34m'
  BOLD='\033[1m'
  NONE='\033[0m'

  echo -e "\n${RED}Usage${NONE}:
  ${BOLD} deploy ${NONE} [OPTION]"

  echo -e "\n${RED}Options${NONE}:
  ${BLUE}-h|-help${NONE}: print the tool help
  ${BLUE}-all|-all${NONE}: update all project , ota and avos
  ${BLUE}-p|-project${NONE}: only update project
  ${BLUE}-m|-map${NONE}: only update map
  ${BLUE}-o|-ota${NONE}: only update ota
  ${BLUE}-a|-avos${NONE}: only update avos
  ${BLUE}-c|-create${NONE}: make ota package,and create a release package
  ${BLUE}-r|-restart${NONE}: reboot the tx2 102 and 103
  ${BLUE}-b|-backup${NONE}: backup the avos and the map file
  ${BLUE}-cv|-checkversion${NONE}: check the version info
  Version:${version}
  "
}

function restart()
{
  echo "Start to reboot the system."
  cmd_reboot="${remote_pw} reboot;"
  echo "${target_ip} --> We are going to reboot:"
  ${ssh_pass} ssh ${remove_yes} ${target_user}@${target_ip} -t ${cmd_reboot}
  
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
  # # if ${ssh_pass} ssh -o ServerAliveInterval=30 ${target_user}@${target_ip} -t ${cmd_record}
  # if ${ssh_pass} ssh -fn ${target_user}@${target_ip} -t ${cmd_record}
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
  if [ $target_online == "true" ]
  then
    echo -e "${BLUE}begin backup avos..."
    createRemoteRun
    remote_cmd="source ${remote_home}${remote_sh};back_up_avos;"
    echo -e "${BLUE}"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}102 source ok${NONE}" 
    else
      echo -e "${RED}102 source error${NONE}" 
      return 0
    fi
    echo -e "${BLUE}"
    if ${ssh_pass} scp ${target_user}@${target_ip}:/home/nvidia/.ros/avos.tar.gz .
    then
      echo -e "${GREEN}102 upload avos ok${NONE}"
    else
      echo -e "${RED}102 upload avos error${NONE}"
      return 0
    fi
    deleteRemoteRun
  else
    return 0
  fi
}

function backup_project()
{
  echo "${target_ip} -->Start to backup the project."
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "${target_ip} --> Start to backup the project..."
    createRemoteRun
    remote_cmd="source ${remote_home}${remote_sh};back_up_project ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}${target_ip} --> source remote script is ok.${NONE}" 
    else
      echo -e "${RED}${target_ip} --> source remote script is error.${NONE}" 
      return 0
    fi
    if ${ssh_pass} scp ${target_user}@${target_ip}:/work/share/project/${project_name}/${project_name}.tar.gz .
    then
      echo -e "${GREEN}${target_ip} --> Uploading project is ok.${NONE}"
    else
      echo -e "${RED}${target_ip} --> Uploading project is error.${NONE}"
      return 0
    fi
    deleteRemoteRun
  else
    return 0
  fi
}

function backup_ota()
{
  echo "start backup ota"
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "${BLUE}begin backup ota..."
    createRemoteRun
    remote_cmd="source ${remote_home}${remote_sh};back_up_ota ${target_pass_word} ${project_name};"
    echo -e "${BLUE}"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}102 source ok${NONE}" 
    else
      echo -e "${RED}102 source error${NONE}" 
      return 0
    fi
    echo -e "${BLUE}"
    if ${ssh_pass} scp ${target_user}@${target_ip}:/work/share/project/ota/ota.tar.gz .
    then
      echo -e "${GREEN}102 upload ota ok${NONE}"
    else
      echo -e "${RED}102 upload ota error${NONE}"
      return 0
    fi
    deleteRemoteRun
  else
    return 0
  fi
}

function backup_local_map()
{
  echo "${target_ip} --> Start to backup current map file ."
  is_online
  if [ $target_online == "true" ]
  then
    remote_cmd="source ${remote_home}${remote_sh};backup_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}${target_ip} --> package current map file is ok.${NONE}" 
    else
      echo -e "${RED}${target_ip} --> package current map file is error.${NONE}" 
      return 0
    fi
  else
    return 0
  fi
}

function recover_local_map()
{
  echo "${target_ip} --> Start to recover current map file."
  is_online
  if [ $target_online == "true" ]
  then
    remote_cmd="source ${remote_home}${remote_sh};recover_local_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}${target_ip} --> recover current map file is ok.${NONE}" 
    else
      echo -e "${RED}${target_ip} --> recover current map file is error.${NONE}" 
      return 0
    fi
  else
    return 0
  fi
}

function backup_map()
{
  echo "${target_ip} --> Start to backup the map."
  is_online
  if [ $target_online == "true" ]
  then
    echo -e "${target_ip} --> Start to backup the map..."
    createRemoteRun
    remote_cmd="source ${remote_home}${remote_sh};back_up_map ${target_pass_word} ${project_name};"
    if ${ssh_pass} ssh ${target_user}@${target_ip} -t ${remote_cmd}
    then
      echo -e "${GREEN}${target_ip} --> source remote script is ok.${NONE}" 
    else
      echo -e "${RED}${target_ip} --> source remote script is error.${NONE}" 
      return 0
    fi
    if ${ssh_pass} scp ${target_user}@${target_ip}:/work/share/project/${project_name}/src/map_file.tar.gz
    then
      echo -e "${GREEN}${target_ip} --> Uploading map package is ok.${NONE}"
    else
      echo -e "${RED}${target_ip} --> Uploading map package is error.${NONE}"
      return 0
    fi
    deleteRemoteRun
  else
    return 0
  fi
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
    -all|-all)
      updateAll $@
      ;;
    -d|-download)
      download $@
      ;;
    -p|-project)
      updateProject $@
      ;;
    -m|-map)
      updateMap $@
      ;;
    -o|-ota)
      updateOta $@
      ;;
    -a|-avos)
      updateAvos $@
      ;;
    -c|-create)
      createOtaTar $@
      ;;
    -h|-help)
      print_usage $@
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
    -cv|-checkversion)
      checkversion $@
      ;;
    -k|-addsshkey)
      add_key $@
      ;;
    *)
      print_usage
      ;;
  esac
}
