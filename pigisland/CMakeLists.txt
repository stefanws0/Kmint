# Author: Jeroen de Haas <jah.dehaas@avans.nl>
cmake_minimum_required(VERSION 3.1.3)
project(pigisland)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wpedantic)#  -Werror)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

  
add_executable(pigisland
  src/kmint/pigisland/main.cpp
  src/kmint/pigisland/node_algorithm.cpp
  src/kmint/pigisland/resources.cpp
  src/kmint/pigisland/shark.cpp
  src/kmint/pigisland/boat.cpp
  src/kmint/pigisland/steering_behaviors.cpp
  src/kmint/pigisland/matrix.cpp
  src/kmint/pigisland/wall.cpp
  src/kmint/pigisland/pig.cpp
  src/kmint/pigisland/shark_state.cpp
  src/kmint/pigisland/shark_state_wandering.cpp
  src/kmint/pigisland/shark_state_attacking.cpp
  src/kmint/pigisland/shark_state_tired.cpp
  src/kmint/pigisland/genetic_algorithm.cpp
  src/kmint/pigisland/shark_state_scared.cpp)

target_compile_features(pigisland PUBLIC cxx_std_17)
target_link_libraries(pigisland PRIVATE kmint)
target_include_directories(pigisland PRIVATE 
  src
  include)

add_custom_target(pigisland-resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
