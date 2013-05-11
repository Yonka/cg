cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(motion-example)

add_executable(motion motion.cpp)
target_link_libraries(motion find_path visualization ${GMP_LIBRARIES})
