# STM32WL port. Builds only with an arm-none-eabi toolchain:
#
#   cmake -S firmware/port/stm32wl -B build/stm32wl \
#         -DCMAKE_TOOLCHAIN_FILE=$PWD/firmware/port/stm32wl/arm-none-eabi.cmake
#   (use an absolute path; a relative one resolves against the build or
#   source directory, not the repository root)
#   cmake --build build/stm32wl
#
# UNTESTED: nothing here has run on a board. The board layer is stubs.
cmake_minimum_required(VERSION 3.20)

project(irrigation_remote_stm32wl C ASM)

# The toolchain file is only read by project(), so check after it.
if(NOT CMAKE_CROSSCOMPILING)
  message(FATAL_ERROR
    "The STM32WL port needs the arm-none-eabi toolchain file. "
    "Pass -DCMAKE_TOOLCHAIN_FILE=<repo>/firmware/port/stm32wl/arm-none-eabi.cmake")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(IRR_WERROR "Treat warnings as errors" ON)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../core ${CMAKE_BINARY_DIR}/core)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/stm32wl55jc_cm4.ld)

add_executable(receiver.elf
  src/startup.c
  src/board_stub.c
  src/main_receiver.c
)
target_include_directories(receiver.elf PRIVATE src)
target_link_libraries(receiver.elf PRIVATE irr_core)
target_compile_options(receiver.elf PRIVATE -Wall -Wextra)
target_link_options(receiver.elf PRIVATE
  -T${LINKER_SCRIPT}
  -Wl,-Map=${CMAKE_BINARY_DIR}/receiver.map
  -Wl,--gc-sections
  -Wl,--print-memory-usage
)
set_target_properties(receiver.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_command(TARGET receiver.elf POST_BUILD
  COMMAND ${CMAKE_SIZE} receiver.elf
  COMMAND ${CMAKE_OBJCOPY} -O ihex receiver.elf receiver.hex
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
