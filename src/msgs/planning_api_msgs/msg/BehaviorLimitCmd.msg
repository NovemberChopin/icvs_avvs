float32      speed_limit #  道路限速(m/s) default-1000
uint8 brake_cmd # 0-default 1-slow_brake 2-emergency_brake
uint8 lane_change_cmd # 0-default 1-left 2-right
		#   "  行为约束
		#0-0:default
		#0-1:不允许避障
		#0-2:允许避障
		#1-0：default
		#1-1：不允许换道
		#1-2：允许换道"
int8[]     behaviorlimit
		# "0-0.default
		#0-1.跟车激进程度保守
		#0-2.跟车激进程度激进
		#1-0.default
		#1-1.远距避障
		#1-2:近距避障
		#2-0:default
		#2-1:远距换道
		#2-2:近距换道"
int8[]       behaviorexecutethreshold 

float32 user_max_speed
float32 user_follow_time
float32 user_follow_dis
float32 user_stop_dis
float32 user_collision_buff
float32 user_pullover_dis
bool enable_avoid