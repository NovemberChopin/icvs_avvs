int32 	mission_id
int8 	mission_type   #三种类型（0:停车  1:自行走  2: 寻迹）
int8 	command  		#任务启停（0:停止任务  1:启动任务  ）
int8 	autodriveentry
int8 	autodriveexit
int8 	car_fault_level
VehicleDriveStatus 	drive_status
PathConstraint[] 	constraints