uint16 id
float32 x         # relative distance of x, unit: m
float32 y         # relative distance of y, unit: m
float32 vxrel     # relative speed of x, unit: m/s
float32 vyrel     # relative speed of y, unit: m/s
float32 vxabs     # absolute speed of x, unit: m/s
float32 vyabs     # absolute speed of y, unit: m/s
float32 width     # unit: m
float32 length    # unit: m
float32 heading
float32 speed     # unit: m/s
uint8    type     # 0-car, 1-truck, 2-pedestrian, 3-cyclist, 4-virtual, 5-unknown
Cell[] cells    # vector of object occupied cells