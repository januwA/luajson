cmake_minimum_required(VERSION 3.0.0)
project(luajson VERSION 0.1.0)

set( CMAKE_CXX_STANDARD 20)
set( CMAKE_CXX_STANDARD_REQUIRED ON)

set(MY_LUA_DIR_NAME lua-5.4.4)
file(GLOB LUA_SRC extern/${MY_LUA_DIR_NAME}/src/l*.c)
list(FILTER LUA_SRC EXCLUDE REGEX "luac.c$")
list(FILTER LUA_SRC EXCLUDE REGEX "lua.c$")

add_library(${PROJECT_NAME} SHARED luajson.cpp ${LUA_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PRIVATE extern/${MY_LUA_DIR_NAME}/src extern/json-3.11.2/include)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  "${PROJECT_SOURCE_DIR}/example.lua"
  "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)