cmake_minimum_required(VERSION 2.8.3)
project(common_math)

find_package(catkin REQUIRED COMPONENTS 
    roscpp 
    genmsg 
    std_msgs
)

include("${CMAKE_SOURCE_DIR}/common/cmake/FindGFLAGS.cmake")  
find_package(Eigen3 REQUIRED)

include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)

include_directories(${EIGEN3_INCLUDE_DIR})
catkin_package(
  INCLUDE_DIRS  
    ${CMAKE_SOURCE_DIR}
    qp_solver
  LIBRARIES 
    ${PROJECT_NAME}_lib
  CATKIN_DEPENDS roscpp roscpp genmsg gencpp genpy std_msgs
)

link_directories(
  ${CMAKE_SOURCE_DIR}/../devel/lib/
  ${CMAKE_SOURCE_DIR}/common/common_lib
)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ./qp_solver/
)
 
set(MATH_SRC
    angle.cpp
    istatistic.cpp
    search.cpp
    line_segment2d.cpp
    polygon2d.cpp
    integral.cpp
    math_utils.cpp
    aabox2d.cpp
    box2d.cpp
    sin_table.cpp
    linear_interpolation.cpp
    dslib.cpp
    vec2d.cpp
    linear_quadratic_regulator.cpp
    qp_solver/qp_solver.cc
    qp_solver/qp_solver_gflags.cc
    qp_solver/active_set_qp_solver.cc
    qp_solver/qpOASES/SparseSolver.cpp
    qp_solver/qpOASES/SubjectTo.cpp
    qp_solver/qpOASES/LAPACKReplacement.cpp
    qp_solver/qpOASES/OQPinterface.cpp
    qp_solver/qpOASES/Constraints.cpp
    qp_solver/qpOASES/QProblemB.cpp
    qp_solver/qpOASES/SolutionAnalysis.cpp
    qp_solver/qpOASES/QProblem.cpp
    qp_solver/qpOASES/SQProblemSchur.cpp
    qp_solver/qpOASES/BLASReplacement.cpp
    qp_solver/qpOASES/Flipper.cpp
    qp_solver/qpOASES/MessageHandling.cpp
    qp_solver/qpOASES/Indexlist.cpp
    qp_solver/qpOASES/Matrices.cpp
    qp_solver/qpOASES/Options.cpp
    qp_solver/qpOASES/SQProblem.cpp
    qp_solver/qpOASES/Bounds.cpp
    qp_solver/qpOASES/Utils.cpp

    # qp_solver/qpOASES/extras/SolutionAnalysis.ipp
    # qp_solver/qpOASES/MessageHandling.ipp
    # qp_solver/qpOASES/SQProblem.ipp
    # qp_solver/qpOASES/Constraints.ipp
    # qp_solver/qpOASES/Bounds.ipp
    # qp_solver/qpOASES/Utils.ipp
    # qp_solver/qpOASES/QProblemB.ipp
    # qp_solver/qpOASES/SQProblemSchur.ipp
    # qp_solver/qpOASES/QProblem.ipp
    # qp_solver/qpOASES/SubjectTo.ipp
    # qp_solver/qpOASES/Indexlist.ipp
  )


add_library(${PROJECT_NAME}_lib 
  ${MATH_SRC}
)

target_link_libraries(${PROJECT_NAME}_lib
    ${catkin_LIBRARIES}
    glog
    gflags
)
