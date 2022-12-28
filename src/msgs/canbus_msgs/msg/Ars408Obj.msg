#                x axis  ^
#                        | longitude_dist
#                        |
#                        |
#                        |
#          lateral_dist  |
#          y axis        |
#        <----------------
#        ooooooooooooo   //radar front surface

int32         id                    # obstacle ID.
# longitude distance to the radar; (+) = forward; unit = m
float32       x
# lateral distance to the radar; (+) = left; unit = m
float32       y
# longitude velocity to the radar; (+) = forward; unit = m/s
float32       vxrel
# lateral velocity to the radar; (+) = left; unit = m/s
float32       vyrel
# longitude distance standard deviation to the radar; (+) = forward; unit = m
float32       dist_long_rms
# lateral distance standard deviation to the radar; (+) = left; unit = m
float32       dist_lat_rms
# longitude velocity standard deviation to the radar; (+) = forward; unit = m/s
float32       vrel_long_rms             
# lateral velocity standard deviation to the radar; (+) = left; unit = m/s
float32       vrel_lat_rms
# 0: moving, 1: stationary, 2: oncoming, 3: stationary candidate
# 4: unknown, 5: crossing stationary, 6: crossing moving, 7: stopped
uint8         dyn_prop
# obstacle Radar Cross-Section; unit = dBsm
float32       rcs
# obstacle probability of existence
float32       prob_of_exist

# The following is only valid for the track object message
# longitude acceleration to the radar; (+) = forward; unit = m/s2
float32       arel_long
# lateral acceleration to the radar; (+) = left; unit = m/s2
float32       arel_lat
# longitude acceleration standard deviation to the radar; (+) = forward; unit = m/s2
float32       arel_long_rms
# lateral acceleration standard deviation to the radar; (+) = left; unit = m/s2
float32       arel_lat_rms
# orientation angle to the radar; (+) = counterclockwise; unit = degree
float32       orientation_angle
# orientation angle standard deviation to the radar; (+) = counterclockwise; unit = degree
float32       orientation_rms
# 0: point; 1: car; 2: truck; 3: pedestrian; 4: motorcycle; 5: bicycle; 6: wide; 7: unknown
uint8         object_class
# 0: deleted; 1: new; 2: measured; 3: predicted; 4: deleted for; 5: new from merge
uint8         meas_state
# obstacle length; unit = m
float32       length
# obstacle width; unit = m
float32       width

# The following is only valid for the cluster object message
# 0: invalid; 1: ambiguous; 2: staggered ramp; 3: unambiguous; 4: stationary candidates
uint8         ambig_state
# 0: Valid; 1: Invalid due to low RCS; 2: Invalid due to near-field artefact; 
# 3: Invalid far range Cluster because not confirmed in near range; 4....
uint8         invalid_state