std_msgs/Header header
uint8	        acq_type ##采图任务id。1:正常采图; 2:快速采图;
uint8 stage ## 0:无 1:slow speed 2:free speed
std_msgs/String seg_name ##seg文件名
uint8           add_funcpoint ## 0:无 1:添加功能点
uint8 	        func_type ##功能点类型  1:返回点; 2:充电点; 3:倒垃圾点; 4:停靠点; 5: 循迹点; 6: 循迹起点; 7:循迹终点

