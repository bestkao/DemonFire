# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.1

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.1.1_1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.1.1_1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello

# Include any dependencies generated for this target.
include CMakeFiles/HelloWorld.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HelloWorld.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HelloWorld.dir/flags.make

CMakeFiles/HelloWorld.dir/HelloWorld.o: CMakeFiles/HelloWorld.dir/flags.make
CMakeFiles/HelloWorld.dir/HelloWorld.o: HelloWorld.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HelloWorld.dir/HelloWorld.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HelloWorld.dir/HelloWorld.o -c /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello/HelloWorld.cxx

CMakeFiles/HelloWorld.dir/HelloWorld.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HelloWorld.dir/HelloWorld.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello/HelloWorld.cxx > CMakeFiles/HelloWorld.dir/HelloWorld.i

CMakeFiles/HelloWorld.dir/HelloWorld.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HelloWorld.dir/HelloWorld.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello/HelloWorld.cxx -o CMakeFiles/HelloWorld.dir/HelloWorld.s

CMakeFiles/HelloWorld.dir/HelloWorld.o.requires:
.PHONY : CMakeFiles/HelloWorld.dir/HelloWorld.o.requires

CMakeFiles/HelloWorld.dir/HelloWorld.o.provides: CMakeFiles/HelloWorld.dir/HelloWorld.o.requires
	$(MAKE) -f CMakeFiles/HelloWorld.dir/build.make CMakeFiles/HelloWorld.dir/HelloWorld.o.provides.build
.PHONY : CMakeFiles/HelloWorld.dir/HelloWorld.o.provides

CMakeFiles/HelloWorld.dir/HelloWorld.o.provides.build: CMakeFiles/HelloWorld.dir/HelloWorld.o

# Object files for target HelloWorld
HelloWorld_OBJECTS = \
"CMakeFiles/HelloWorld.dir/HelloWorld.o"

# External object files for target HelloWorld
HelloWorld_EXTERNAL_OBJECTS =

HelloWorld: CMakeFiles/HelloWorld.dir/HelloWorld.o
HelloWorld: CMakeFiles/HelloWorld.dir/build.make
HelloWorld: /usr/local/lib/libitkdouble-conversion-4.7.a
HelloWorld: /usr/local/lib/libitksys-4.7.a
HelloWorld: /usr/local/lib/libitkvnl_algo-4.7.a
HelloWorld: /usr/local/lib/libitkvnl-4.7.a
HelloWorld: /usr/local/lib/libitkv3p_netlib-4.7.a
HelloWorld: /usr/local/lib/libITKCommon-4.7.a
HelloWorld: /usr/local/lib/libitkNetlibSlatec-4.7.a
HelloWorld: /usr/local/lib/libITKStatistics-4.7.a
HelloWorld: /usr/local/lib/libITKIOImageBase-4.7.a
HelloWorld: /usr/local/lib/libITKMesh-4.7.a
HelloWorld: /usr/local/lib/libitkzlib-4.7.a
HelloWorld: /usr/local/lib/libITKMetaIO-4.7.a
HelloWorld: /usr/local/lib/libITKSpatialObjects-4.7.a
HelloWorld: /usr/local/lib/libITKPath-4.7.a
HelloWorld: /usr/local/lib/libITKLabelMap-4.7.a
HelloWorld: /usr/local/lib/libITKQuadEdgeMesh-4.7.a
HelloWorld: /usr/local/lib/libITKOptimizers-4.7.a
HelloWorld: /usr/local/lib/libITKPolynomials-4.7.a
HelloWorld: /usr/local/lib/libITKBiasCorrection-4.7.a
HelloWorld: /usr/local/lib/libITKBioCell-4.7.a
HelloWorld: /usr/local/lib/libITKDICOMParser-4.7.a
HelloWorld: /usr/local/lib/libITKEXPAT-4.7.a
HelloWorld: /usr/local/lib/libITKIOXML-4.7.a
HelloWorld: /usr/local/lib/libITKIOSpatialObjects-4.7.a
HelloWorld: /usr/local/lib/libITKFEM-4.7.a
HelloWorld: /usr/local/lib/libitkopenjpeg-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmDICT-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmMSFF-4.7.a
HelloWorld: /usr/local/lib/libITKznz-4.7.a
HelloWorld: /usr/local/lib/libITKniftiio-4.7.a
HelloWorld: /usr/local/lib/libITKgiftiio-4.7.a
HelloWorld: /usr/local/lib/libitkhdf5_cpp-4.7.a
HelloWorld: /usr/local/lib/libitkhdf5-4.7.a
HelloWorld: /usr/local/lib/libITKIOBMP-4.7.a
HelloWorld: /usr/local/lib/libITKIOBioRad-4.7.a
HelloWorld: /usr/local/lib/libITKIOCSV-4.7.a
HelloWorld: /usr/local/lib/libITKIOGDCM-4.7.a
HelloWorld: /usr/local/lib/libITKIOIPL-4.7.a
HelloWorld: /usr/local/lib/libITKIOGE-4.7.a
HelloWorld: /usr/local/lib/libITKIOGIPL-4.7.a
HelloWorld: /usr/local/lib/libITKIOHDF5-4.7.a
HelloWorld: /usr/local/lib/libitkjpeg-4.7.a
HelloWorld: /usr/local/lib/libITKIOJPEG-4.7.a
HelloWorld: /usr/local/lib/libitktiff-4.7.a
HelloWorld: /usr/local/lib/libITKIOTIFF-4.7.a
HelloWorld: /usr/local/lib/libITKIOLSM-4.7.a
HelloWorld: /usr/local/lib/libITKIOMRC-4.7.a
HelloWorld: /usr/local/lib/libITKIOMesh-4.7.a
HelloWorld: /usr/local/lib/libITKIOMeta-4.7.a
HelloWorld: /usr/local/lib/libITKIONIFTI-4.7.a
HelloWorld: /usr/local/lib/libITKNrrdIO-4.7.a
HelloWorld: /usr/local/lib/libITKIONRRD-4.7.a
HelloWorld: /usr/local/lib/libitkpng-4.7.a
HelloWorld: /usr/local/lib/libITKIOPNG-4.7.a
HelloWorld: /usr/local/lib/libITKIOSiemens-4.7.a
HelloWorld: /usr/local/lib/libITKIOStimulate-4.7.a
HelloWorld: /usr/local/lib/libITKIOTransformBase-4.7.a
HelloWorld: /usr/local/lib/libITKIOTransformHDF5-4.7.a
HelloWorld: /usr/local/lib/libITKIOTransformInsightLegacy-4.7.a
HelloWorld: /usr/local/lib/libITKIOTransformMatlab-4.7.a
HelloWorld: /usr/local/lib/libITKIOVTK-4.7.a
HelloWorld: /usr/local/lib/libITKKLMRegionGrowing-4.7.a
HelloWorld: /usr/local/lib/libITKOptimizersv4-4.7.a
HelloWorld: /usr/local/lib/libITKVTK-4.7.a
HelloWorld: /usr/local/lib/libITKVideoCore-4.7.a
HelloWorld: /usr/local/lib/libITKVideoIO-4.7.a
HelloWorld: /usr/local/lib/libITKVtkGlue-4.7.a
HelloWorld: /usr/local/lib/libITKWatersheds-4.7.a
HelloWorld: /usr/local/lib/libITKPolynomials-4.7.a
HelloWorld: /usr/local/lib/libITKIOXML-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmMSFF-4.7.a
HelloWorld: /usr/local/lib/libitkopenjpeg-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmDICT-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmIOD-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmDSED-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmCommon-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmjpeg8-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmjpeg12-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmjpeg16-4.7.a
HelloWorld: /usr/local/lib/libitkgdcmuuid-4.7.a
HelloWorld: /usr/local/lib/libITKIOTIFF-4.7.a
HelloWorld: /usr/local/lib/libitktiff-4.7.a
HelloWorld: /usr/local/lib/libitkjpeg-4.7.a
HelloWorld: /usr/local/lib/libITKgiftiio-4.7.a
HelloWorld: /usr/local/lib/libITKEXPAT-4.7.a
HelloWorld: /usr/local/lib/libITKMetaIO-4.7.a
HelloWorld: /usr/local/lib/libITKniftiio-4.7.a
HelloWorld: /usr/local/lib/libITKznz-4.7.a
HelloWorld: /usr/local/lib/libITKNrrdIO-4.7.a
HelloWorld: /usr/local/lib/libitkpng-4.7.a
HelloWorld: /usr/local/lib/libITKIOGE-4.7.a
HelloWorld: /usr/local/lib/libITKIOIPL-4.7.a
HelloWorld: /usr/local/lib/libitkhdf5_cpp-4.7.a
HelloWorld: /usr/local/lib/libitkhdf5-4.7.a
HelloWorld: /usr/local/lib/libitkzlib-4.7.a
HelloWorld: /usr/local/lib/libITKIOTransformBase-4.7.a
HelloWorld: /usr/local/lib/libITKOptimizers-4.7.a
HelloWorld: /usr/local/lib/libITKVideoCore-4.7.a
HelloWorld: /usr/local/lib/libITKVTK-4.7.a
HelloWorld: /usr/local/lib/libvtkRenderingOpenGL-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkImagingHybrid-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkIOImage-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkDICOMParser-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkIOCore-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkmetaio-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkpng-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtktiff-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkjpeg-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkRenderingFreeType-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkftgl-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkfreetype-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkzlib-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkInteractionStyle-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkRenderingCore-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkFiltersGeometry-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkFiltersExtraction-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkFiltersStatistics-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkImagingFourier-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkalglib-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkFiltersSources-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkFiltersGeneral-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkFiltersCore-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonComputationalGeometry-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkImagingSources-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkImagingCore-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonExecutionModel-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonDataModel-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonMisc-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonSystem-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtksys-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonTransforms-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonMath-6.1.1.dylib
HelloWorld: /usr/local/lib/libvtkCommonCore-6.1.1.dylib
HelloWorld: /usr/local/lib/libITKSpatialObjects-4.7.a
HelloWorld: /usr/local/lib/libITKIOImageBase-4.7.a
HelloWorld: /usr/local/lib/libITKMesh-4.7.a
HelloWorld: /usr/local/lib/libITKPath-4.7.a
HelloWorld: /usr/local/lib/libITKStatistics-4.7.a
HelloWorld: /usr/local/lib/libITKCommon-4.7.a
HelloWorld: /usr/local/lib/libitkdouble-conversion-4.7.a
HelloWorld: /usr/local/lib/libitksys-4.7.a
HelloWorld: /usr/local/lib/libITKVNLInstantiation-4.7.a
HelloWorld: /usr/local/lib/libitkvnl_algo-4.7.a
HelloWorld: /usr/local/lib/libitkv3p_lsqr-4.7.a
HelloWorld: /usr/local/lib/libitkvnl-4.7.a
HelloWorld: /usr/local/lib/libitkvcl-4.7.a
HelloWorld: /usr/local/lib/libitkNetlibSlatec-4.7.a
HelloWorld: /usr/local/lib/libitkv3p_netlib-4.7.a
HelloWorld: CMakeFiles/HelloWorld.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable HelloWorld"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HelloWorld.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HelloWorld.dir/build: HelloWorld
.PHONY : CMakeFiles/HelloWorld.dir/build

CMakeFiles/HelloWorld.dir/requires: CMakeFiles/HelloWorld.dir/HelloWorld.o.requires
.PHONY : CMakeFiles/HelloWorld.dir/requires

CMakeFiles/HelloWorld.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HelloWorld.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HelloWorld.dir/clean

CMakeFiles/HelloWorld.dir/depend:
	cd /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello /Users/james/Dropbox/projects/621/ITK-Tutorial/Hello/CMakeFiles/HelloWorld.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HelloWorld.dir/depend

