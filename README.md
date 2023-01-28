version:V1.0.0.0

1、将avos_rviz.tar.gz 解压到home目录下，不能是其他的路径；

2、安装protobuf 3.5.1
```
(1) tar -xvjf protobuf-3.5.1.tar.bz2
(2) cd protobuf-3.5.1/
(3) ./configure
(4) make clean
(5) make
(6) sudo make install
(7) sudo ldconfig
(8) protoc -h （验证是否安装成功：正常输出指令，没有报error，则成功）
```

３、安装Qt
(1)cd　到.run文件的目录下，右键打开终端，执行 sudo chmod -R 777 qt-opensource-linux-x64-5.11.1.run
(2)注册，安装选项除了android不选，其他的都选择，然后直接点下一步即可；

---
## 安装依赖
```bash
# qt
sudo apt install ros-melodic-qt-create ros-melodic-qt-build
sudo apt install qtmultimedia5-dev
# glob
sudo apt-get install libgflags-dev libgoogle-glog-dev
# pcap
sudo apt install libpacp-dev
```

