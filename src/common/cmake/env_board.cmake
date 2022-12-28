
find_package(PCL 1.7 REQUIRED)

#include(/opt/ros/melodic/share/catkin/cmake/env/CMakeLists.txt)

###########################set(SERIAL_LIBRARIES serial)

set(PROTOBUF_LIBRARIES_CROSS /work/crosscompile/usr/local/lib/libprotobuf.so)
#set(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES_CROSS})

include_directories(
     ${catkin_INCLUDE_DIRS}
     )

