std_msgs/Header header
string   file_name
uint8    record_status   ##1:接收到请求;2:录包成功;3:录包失败;
uint8    record_type     ##1:云端触发，2：遥控按键触发，3：自动触发 4:AVVS