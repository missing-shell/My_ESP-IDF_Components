 
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS "../../task_list")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(task_list_test)

