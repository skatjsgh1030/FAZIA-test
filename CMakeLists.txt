cmake_minimum_required(VERSION 3.16)
project(PROJECT_neuSIM4)

#----------------------------------------------------------------------------

set(useROOT false)
set(neuSIM4_SOURCE_DIRECTORIES src)
set(neuSIM4_HEADER_DIRECTORIES include)
set(neuSIM4_EXECUTABLES example_1)
set(neuSIM4_SCRIPTS run.mac vis_1.mac)

#----------------------------------------------------------------------------

option(WITH_GEANT4_UIVIS "Build neuSIM4 with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

if(useROOT)
  list(APPEND neuSIM4_SOURCE_DIRECTORIES)
  list(APPEND neuSIM4_HEADER_DIRECTORIES)
  list(APPEND neuSIM4_EXECUTABLES)
endif(useROOT)

set(neuSIM4_SOURCES_DIRECTORIES_FULL)
foreach(_directory ${neuSIM4_SOURCE_DIRECTORIES})
  list(APPEND neuSIM4_SOURCE_DIRECTORIES_FULL ${PROJECT_SOURCE_DIR}/${_directory})
endforeach(_directory)

set(neuSIM4_HEADERS_DIRECTORIES_FULL)
foreach(_directory ${neuSIM4_HEADER_DIRECTORIES})
  list(APPEND neuSIM4_HEADER_DIRECTORIES_FULL ${PROJECT_SOURCE_DIR}/${_directory})
endforeach(_directory)

set(neuSIM4_SOURCES)
foreach(_directory ${neuSIM4_SOURCE_DIRECTORIES_FULL})
  list(APPEND neuSIM4_SOURCES ${_directory}/*.cc)
endforeach(_directory)

set(neuSIM4_HEADERS)
foreach(_directory ${neuSIM4_HEADER_DIRECTORIES_FULL})
  list(APPEND neuSIM4_HEADERS ${_directory}/*.hh)
endforeach(_directory)

include(${Geant4_USE_FILE})
include_directories(${neuSIM4_SOURCE_DIRECTORIES_FULL})
include_directories(${neuSIM4_HEADER_DIRECTORIES_FULL})
if(useROOT)
  include_directories(${ROOT_INCLUDE_DIR})
endif(useROOT)

if(useROOT)
  find_package(ROOT REQUIRED)
  EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
endif(useROOT)

file(GLOB sources ${neuSIM4_SOURCES})
file(GLOB headers ${neuSIM4_HEADERS})

foreach(_executable ${neuSIM4_EXECUTABLES})
  add_executable(${_executable} ${_executable}.cc ${sources} ${headers})
if(useROOT)
  EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
  target_link_libraries(${_executable} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
endif(useROOT)
  target_link_libraries(${_executable} ${Geant4_LIBRARIES})
endforeach(_executable)

foreach(_script ${neuSIM4_SCRIPTS})
  configure_file( ${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_BINARY_DIR}/${_script} COPYONLY)
endforeach(_script)

add_custom_target(PROJECT_neuSIM4 DEPENDS ${neuSIM4_EXECUTABLES})

foreach(_executable ${neuSIM4_EXECUTABLES})
  install(TARGETS ${_executable} DESTINATION bin)
endforeach(_executable)
