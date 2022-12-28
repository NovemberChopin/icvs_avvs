float64		 start_time #	建图开始时间
string		 map_name  #地图名称 
#"  -1：建图执行失败
# 0：建图执行成功
# 1：建图运行中
#2：建图暂停中
#3：无建图任务"
int8 		 status
string[]		 addition_message #	失败原因
string       map_id
float32 		 map_area	#当前建图面积
