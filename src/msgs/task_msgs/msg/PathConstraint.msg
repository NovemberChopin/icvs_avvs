std_msgs/String 	id #	道路id号		
float32 	speed_limit #	道路限速	m/s	
int8[] 	behaviorlimit # 	  行为约束		1.是否允许避障；2.是否允许自由换道
int8[] 	behavioractivethreshold #  	行为激活条件激进程度		"1.自由换道触发激进程度（频繁/不频繁）；2.避障触发距离远近（远/近）"
int8[] 		behaviorexecutethreshold # 	行为执行激进程度		"1.跟车激进程度（激进/不激进） 2.避障激进程度（避障路径距离障碍物的远近） 3.换道激进程度（激进/不激进）"
