int8 	emergency_stop_btn_status #	急停按钮状态
int8 	battery_info	
uint8   bag_record_key_state
uint8   clean_water_state     #清水状态		"0：无清水  1：有清水"
uint8   sewage_state         #污水状态		"0：无污水  1：有污水"
uint8   temper_sensor_1      # 温度1	摄氏度
uint8   temper_sensor_2      # 温度2	摄氏度
uint8   temper_sensor_3      #温度3	摄氏度
uint8   turn_light_left_state   #左转向灯状态		"0：关闭  1：开启"
uint8   turn_light_right_state   #右转向灯状态		"0：关闭  1：开启"
uint8   head_lamp_low_beam_status  
uint8   head_lamp_high_beam_status
uint8   drv_light_state  #行车灯状态		"0：关闭  1：开启"
uint8   horn_state      #喇叭状态		"0：关闭  1：开启"
uint8   up_lidar_state    #顶激光状态		"0：关闭 1：开启"
uint8   front_lidar_state   #前激光状态		"0：关闭  1：开启"
uint8   water_level_state    #水量档位反馈状态		0~3档
uint8   brush_pressure_state   #刷盘压力反馈状态		0~3档
uint8   speed_level_state   #速度档位反馈状态		0~3档
uint8   suction_state   #吸水马达吸力反馈状态		0~3档
uint8   side_brush_state   # 边刷状态反馈(预留)
uint8   scrubbing_instruc   #洗地状态反馈		"0：无效   1：洗地   2：吸水   3：洗地+吸水"

