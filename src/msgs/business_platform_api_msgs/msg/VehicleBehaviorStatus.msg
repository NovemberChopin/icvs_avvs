int8 	 forward_backward	#（0：默认  1：前进   2：后退）前进或后退
int8 	driving_status #	0.循迹；1.换道；2.避障
int8  	left_right #	（0：默认  1：左转   2：右转）左转或右转
int8 	is_emergency_stop #	（0：默认  1：急停）
float32  	speed_limit	#道路限速(m/s)
#"  行为约束
#0-0:default
#0-1:不允许避障
#0-2:允许避障
#1-0：default
#1-1：不允许换道
#1-2：允许换道"
int8[] 	behaviorlimit 	
#	"0-0.default
#0-1.跟车激进程度保守
#0-2.跟车激进程度激进
#1-0.default
#1-1.远距避障
#1-2:近距避障
#2-0:default
#2-1:远距换道
#2-2:近距换道"
int8[] 	behaviorexecutethreshold 
