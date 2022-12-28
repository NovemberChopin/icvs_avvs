std_msgs/Header  header
uint8	         cal_type ##标定任务id。1:激光雷达标定; 2:激光与相机标定; 3:GNSS基线、航向与轮速计标定; 4:激光雷达与轮速计标定; 5:EPS标定; 6:激光雷达与定位联合标定
uint8            cal_times ##标定次数，第几次标定
params[]         cal_params ##标定参数格式
