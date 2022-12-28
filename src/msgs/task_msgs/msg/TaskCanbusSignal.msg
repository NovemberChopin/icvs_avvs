std_msgs/Header 	header
bool 	emergency_stop 		# 0:not care 	1:emergency
bool 	lidar_operation 	# 0:stop 	1:open
uint8   clean_ctl
uint8   dust_ctl
CanbusSignal[] can_signal