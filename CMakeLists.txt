cmake_minimum_required(VERSION 3.10)

project(Timer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)



add_executable(timer
 clock.c
 driver.c
 timer.c
 stdinout.c)

target_compile_definitions(timer PRIVATE STDINPUT)
