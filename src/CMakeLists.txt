cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

project(DemonFire)

# set up VTK and ITK
find_package(VTK REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ITK REQUIRED)
include (${VTK_USE_FILE})
include(${ITK_USE_FILE})

# Core application library
include_directories("../include/")
add_library(DemonFireCore
Utility.cxx
ImageProcessing.cxx
registration/Registration.cxx
segmentation/Segmentation.cxx)

# set properties for build targets
add_executable(DemonFire DemonFire.cxx)
target_link_libraries(DemonFire DemonFireCore ${VTK_LIBRARIES} ${ITK_LIBRARIES})
