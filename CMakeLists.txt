CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
# make project_source_dir, project_binary_dir, project_name available
PROJECT(orthoPoly)
# enable/disable testing with 'cmake -Dtest=ON/OFF'
OPTION(test "Build all tests" ON)
option(debug "Build with debugger" OFF)
# setting include dir and compile flags
if(NOT debug)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Iinclude -O3")
elseif(debug)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Iinclude -O3 -g -DDEBUG")
endif()
INCLUDE_DIRECTORIES(include)
FILE(GLOB H_FILES include/*.H)
FILE(GLOB h_FILES include/*.h)
FILE(GLOB hpp_FILES include/*.hpp)
SET(orthoPoly_INC_FILES ${H_FILES} ${h_FILES} ${hpp_FILES})
INSTALL(FILES ${orthoPoly_INC_FILES} DESTINATION include)
# setting src files
SET(SRCS src/orthoPoly1D.C src/orthoPoly3D.C)

# set output paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# building lib
ADD_LIBRARY(orthoPolyLib SHARED ${SRCS})
INSTALL(TARGETS orthoPolyLib LIBRARY DESTINATION lib)

# building executable
ADD_EXECUTABLE(orthoPoly utils/orthoPolyRun.C)
TARGET_LINK_LIBRARIES(orthoPoly orthoPolyLib)
INSTALL(TARGETS orthoPoly RUNTIME DESTINATION bin)

#### Testing #####

if(test)
  if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
  endif (APPLE)

  # adding gtest subdir with its own cmake file
  add_subdirectory(testing/lib/gtest-1.6.0)
  enable_testing()
  # include gtest library. gtest_SOURCE_DIR is availble from subdir addition
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}/include/gtest ${gtest_SOURCE_DIR})
  
  #### Unit Tests ####  
  SET(OLD_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/testing)
  add_executable(runUnitTests ${CMAKE_SOURCE_DIR}/testing/testOrthoPoly.C)
  target_link_libraries(runUnitTests gtest gtest_main orthoPolyLib)
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OLD_RUNTIME_OUTPUT_DIRECTORY})
  add_subdirectory(testing)
  INSTALL(TARGETS runUnitTests RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/testing)
  #add_test(NAME orthoPolyTest COMMAND runUnitTests) 

endif(test)



