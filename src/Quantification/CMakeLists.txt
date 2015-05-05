# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(DemonFire)

# Find ITK.
FIND_PACKAGE(ITK REQUIRED)
FIND_PACKAGE(VTK REQUIRED)
#FIND_PACKAGE(FLTK REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
include(${ITK_USE_FILE})
INCLUDE (${VTK_USE_FILE})

#add_executable(HelloWorld HelloWorld.cxx )
#ADD_EXECUTABLE(DemonGUI DemonGUI.cxx)
#add_executable(LoadDICOM LoadDICOM.cxx)
add_executable(AxialViewsQuantify AxialViewsQuantify.cxx)

#target_link_libraries(HelloWorld ${ITK_LIBRARIES})
#TARGET_LINK_LIBRARIES(DemonGUI ${ITK_LIBRARIES} ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES} ${VTK_LIBRARIES})
#target_link_libraries(LoadDICOM ${ITK_LIBRARIES} ${VTK_LIBRARIES})
target_link_libraries(AxialViews ${ITK_LIBRARIES} ${VTK_LIBRARIES})
