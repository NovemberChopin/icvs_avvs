string  	avos_version
string      vin
string      ota_version	
string  	mcu_verison	
string  	mcu_s_verison	
string      eps_version
string 		current_map_name
string 		current_map_id
int8 	avos_setup_status	# 0:default   1:启动中   -1 启动失败   2：启动成功
int8 	selfdrive_fault_level	# [0, 1, 2]
int8 	vechile_status	# 0:default  1:初始化中  2:就绪   3:手动驾驶  4:自动驾驶  5:休眠
int8 	business_type	# 0:default  1:空闲状态  3:自动驾驶任务状态  4:标定状态  5:地图采集
int8 	business_status	 # 0:default  1:准备中  2:运行中  3:运行结束
int8 	sub_message	  # 其他
MonitorTable[] monitor_table

