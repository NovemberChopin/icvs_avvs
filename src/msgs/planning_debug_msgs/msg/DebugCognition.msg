string[] reference_lane_ids
string[] reference_target_ids
uint8 mapengine_index
uint8 correction_index
uint8 target_index
bool if_change			# whether change current
int8[] key_line_id		# current_line_id and target_line_id
float32 map_delta_time
float32 imu_delta_time
DebugLine[] current_line
DebugLine[] left_line
DebugLine[] right_line
DebugLine[] rev_line