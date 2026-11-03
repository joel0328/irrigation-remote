cmake_minimum_required(VERSION 3.20)
project(irrigation_remote_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(IRR_WERROR "Treat warnings as errors" ON)
option(IRR_SANITIZE "Build host tests with ASan and UBSan" OFF)

# Portable core: no hardware calls. Shared by host tests and the STM32WL port.
add_subdirectory(core)

if(NOT CMAKE_CROSSCOMPILING)
  enable_testing()
  add_subdirectory(tests)
endif()
