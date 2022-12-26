cmake_minimum_required(VERSION 3.0.0)
project(luajson VERSION 0.1.0)

option(LUA_INCLUDE_DIR "set lua include path" "")
option(LUA_LIB_DIR "set lua lib path" "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED luajson.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")

find_path(lua_INCLUDE_PATH 
  lua.h 
  PATHS /usr/local/include /usr/include ${LUA_INCLUDE_DIR}
)

if(NOT lua_INCLUDE_PATH)
  message(FATAL_ERROR "not find lua include path")
endif()

find_library(lua_LIB_PATH
  lua 
  PATHS /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64 ${LUA_LIB_DIR}
)

if(NOT lua_LIB_PATH)
  message(FATAL_ERROR "not find lua lib path")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
  extern/json-3.11.2/include
  ${lua_INCLUDE_PATH}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_link_libraries(${PROJECT_NAME} PUBLIC ${lua_LIB_PATH})
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  "${PROJECT_SOURCE_DIR}/example.lua"
  "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)