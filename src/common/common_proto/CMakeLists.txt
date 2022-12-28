cmake_minimum_required(VERSION 2.8.3)
project(proto)


list(APPEND PROTO_SRC_FILE header.proto )
list(APPEND PROTO_SRC_FILE pnc_point.proto )
list(APPEND PROTO_SRC_FILE error_code.proto )
list(APPEND PROTO_SRC_FILE config_schema.proto )
list(APPEND PROTO_SRC_FILE can_protocol.proto )

include(${CMAKE_SOURCE_DIR}/common/cmake/protobuf.cmake)
