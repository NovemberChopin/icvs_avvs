#"0：default
#1：准备自动驾驶
#2：开始任务
#3：结束任务
uint8	 cmd	
#"0:default
#1:顶视摄像头标定
#2:RGBD摄像头标定
#10:顶长距激光雷达标定
#11:顶短距激光雷达标定
#12:底短距激光雷达标定"
#20: EPS 标定
uint8 	calibration_type
AvosConfigParam[] 	calibration_param	
