float64 speed
float64 ego_speed
float64 speed_limit
float64 curvature
int32 behavior_fsm_state  # smaller or equal to 0 is slow down to stop, larger than 0 means go
int32 fault_level
int32 emergency_flag
int32 path_direction    # 0 means drive, 1 means reverse
int32 shift_pos_match   # if vehicle shift pos is match with path direction, it is 1, else 0
int32 delay_to_start    # if lat long coupling success, it is 1, else 0
int32 deviate_path_flag   # when car is deviate from path, should stop immediately
int32 is_passable     # determin by decision, 1 means reachable, 0 means unreachable
int32 replan_flag
int32 free_drive_replan
int32 path_update_flag    # when rewrite path in map engine happend, is true
float64 safe_dist
float64 preview_dist
float64 roi_type      # 1:front, 2:beside, 3:near, 0.5:predict front, 1.5: predict beside, 2.5: predict near
float64 roi_id        # obj id after add predict
float64 front_s
float64 front_l
float64 beside_s
float64 beside_l
float64 crashed_cell_s    # crashed with cell at longtitude distance s
float64 cell_preview_dist
float64 cell_limit_index
float64 steer_angle
float64 target_angle    # approximate target steer angle
float64 cost_time
float64 temp1
float64 temp2
float64 temp3
