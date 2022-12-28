float64		 start_time #	标定开始时间
#"  -1：标定执行失败
# 0：标定执行成功
# 1：标定运行中
#2：标定暂停中
#3：无标定任务
#4:标定成功，但需要再次标定"
int8 		 status
#"0:default
#1:顶视摄像头标定
#2:RGBD摄像头标定
#10:顶长距激光雷达标定
#11:顶短距激光雷达标定
#12:底短距激光雷达标定"
uint8		 calibration_type
uint8		 calibration_progress #	标定进度
uint8 	current_step
uint8 	totol_step
uint8 	reserved_value1
uint8 	reserved_value2
uint8 	reserved_value3
uint8 	reserved_value4
uint8 	reserved_value5
string		 addition_message #	失败原因
AvosConfigParam[] 	calibration_param
