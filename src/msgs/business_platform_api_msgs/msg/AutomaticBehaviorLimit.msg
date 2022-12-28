float32 		 speed_limit #	道路限速(m/s) (1000 default  > 0 valid)
int8 brake_command  #0-default 1:slow brake  2:emergency brake 
int8 lane_change_command  #0-default 1:left change  2:right chanege
# 	"  行为约束
#0-0:default
#0-1:不允许避障
#0-2:允许避障
#1-0：default
#1-1：不允许换道
#1-2：允许换道"
int8[] 		 behaviorlimit
#	"0-0.default
#0-1.跟车激进程度保守
#0-2.跟车激进程度激进
#1-0.default
#1-1.远距避障
#1-2:近距避障
#2-0:default
#2-1:远距换道
#2-2:近距换道"
int8[]  		 behaviorexecutethreshold 