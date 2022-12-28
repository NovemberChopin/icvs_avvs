float64 		 start_time #	任务开始时间
int32  		 current_mission_id #	当前的mission id号
#"  -1：任务执行失败
# 0：任务执行成功
# 1：任务运行中
#2：任务暂停中
#3：任务准备失败"
int8 		 status	
string 	addition_message #	失败原因
float32 		 passed_distance	 #当前任务的行进距离
float32 		  totol_distance #	当前任务的总距离
int32  		 passed_mission_num #	已经完成的任务数量
int32  		 totol_mission_num #	总共的任务数量
AutomaticBehaviorLimit behavior_limit
