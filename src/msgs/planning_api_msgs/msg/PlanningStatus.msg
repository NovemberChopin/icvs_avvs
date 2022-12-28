int8      status #-1:异常, 0:无任务状态, 1:任务运行中
int32     mission_id # 当前执行任务id
int8      mission_type # 0:停车, 1:自行走, 2:循迹, 3:找定位
float32   speed_limit # 道路限速(m/s)
int8 brake_status  #0-default 1:slow brake  2:emergency brake 
int8 lane_change_status #0-default 1:left change  2:right chanege
PlanningResult lane_change_feedback  
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