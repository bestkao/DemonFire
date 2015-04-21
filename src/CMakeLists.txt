# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(DemonFire)

# Find ITK.
find_package(ITK REQUIRED)
find_package(VTK REQUIRED)
find_package(OpenGL REQUIRED)
include(${ITK_USE_FILE})
include (${VTK_USE_FILE})
include_directories("../include/")
set(HEADER_FILES ../include/DemonFire.h)
set(SOURCE DemonFire.cxx registration/Registration.cxx segmentation/Segmentation.cxx)

add_executable(DemonFire ${SOURCE} ${HEADER_FILES})

target_link_libraries(DemonFire ${ITK_LIBRARIES} ${VTK_LIBRARIES})
