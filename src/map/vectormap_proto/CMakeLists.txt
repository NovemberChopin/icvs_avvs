cmake_minimum_required(VERSION 2.8.3)
project(vectormap_proto)


list(APPEND PROTO_SRC_FILE geometry.proto)
list(APPEND PROTO_SRC_FILE map_clear_area.proto)
list(APPEND PROTO_SRC_FILE map_overlap.proto)
list(APPEND PROTO_SRC_FILE map_speed_bump.proto)
list(APPEND PROTO_SRC_FILE map_crosswalk.proto)
list(APPEND PROTO_SRC_FILE map_parking.proto)
list(APPEND PROTO_SRC_FILE map_speed_control.proto)
list(APPEND PROTO_SRC_FILE map_geometry.proto)
list(APPEND PROTO_SRC_FILE map.proto)
list(APPEND PROTO_SRC_FILE map_stop_sign.proto)
list(APPEND PROTO_SRC_FILE map_id.proto)
list(APPEND PROTO_SRC_FILE map_roadnet.proto)
list(APPEND PROTO_SRC_FILE map_yield_sign.proto)
list(APPEND PROTO_SRC_FILE map_junction.proto)
list(APPEND PROTO_SRC_FILE map_road.proto)
list(APPEND PROTO_SRC_FILE map_lane.proto)
list(APPEND PROTO_SRC_FILE map_signal.proto)

include(${CMAKE_SOURCE_DIR}/common/cmake/protobuf.cmake)

