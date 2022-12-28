std_msgs/Header  header
int32            mission_seq
uint8	         mission_id ##自动驾驶子任务id。1:自动驾驶; 2:标定; 3:地图采集
uint8            mission_cmd #任务命令。0：无；1：开始；2：结束
ivselfdriving[]	 selfdriving
ivcalibration	 calibration
ivmapacq         mapacq
ivota	         ota
