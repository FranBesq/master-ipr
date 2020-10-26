# A cmake file to get started with for new YARP projects.
# It assumes you want to build an executable from source code in 
# the current directory.

cmake_minimum_required(VERSION 3.0)
project(main)

# Search for source code.
file(GLOB folder_source *.cpp *.cc *.c)
file(GLOB folder_header *.h)

# Automatically add include directories if needed.
foreach(header_file ${folder_header})
  get_filename_component(p ${header_file} PATH)
  include_directories(${p})
endforeach(header_file ${folder_header})

# Set up our main executable.
if(folder_source)
  add_executable(${PROJECT_NAME} ${folder_source} ${folder_header})
  target_link_libraries(${PROJECT_NAME})
else()
  message(FATAL_ERROR "No source code files found. Please add something")
endif()
