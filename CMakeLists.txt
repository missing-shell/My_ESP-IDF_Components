 
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(My_ESP-IDF_Component)

# 添加 components 目录
#add_subdirectory(components/task_list)