cmake_minimum_required(VERSION 3.16)
project(LinuxSandbox VERSION 0.1 LANGUAGES CXX)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "LinuxSandbox requires Linux")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

add_executable(test_tcp tests/test_tcp.cpp)
add_executable(test_udp tests/test_udp.cpp)

# Содержимое бинарников фиксируется при сборке и проверяется перед запуском.
set(allowed_tests "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/AllowedTests.h")
add_custom_command(
    OUTPUT "${allowed_tests}"
    COMMAND "${CMAKE_COMMAND}"
        "-DTCP=$<TARGET_FILE:test_tcp>" "-DUDP=$<TARGET_FILE:test_udp>"
        "-DOUTPUT=${allowed_tests}" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedTests.cmake"
    DEPENDS test_tcp test_udp cmake/EmbedTests.cmake
    VERBATIM
)

add_executable(LinuxSandbox
    src/main.cpp src/Sandbox.cpp src/ProcessManager.cpp src/IsolationManager.cpp
    src/NetworkMonitor.cpp src/ReportManager.cpp src/ConsoleUI.cpp "${allowed_tests}"
)
target_include_directories(LinuxSandbox PRIVATE include "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
target_include_directories(test_tcp PRIVATE include)
target_include_directories(test_udp PRIVATE include)
foreach(target LinuxSandbox test_tcp test_udp)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()
