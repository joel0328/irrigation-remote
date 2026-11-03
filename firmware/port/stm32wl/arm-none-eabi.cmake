# CMake toolchain file for the STM32WL Cortex-M4 core with arm-none-eabi-gcc.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

find_program(ARM_GCC arm-none-eabi-gcc REQUIRED)
get_filename_component(ARM_BIN ${ARM_GCC} DIRECTORY)

set(CMAKE_C_COMPILER   ${ARM_BIN}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${ARM_BIN}/arm-none-eabi-gcc)
set(CMAKE_OBJCOPY      ${ARM_BIN}/arm-none-eabi-objcopy CACHE FILEPATH "")
set(CMAKE_SIZE         ${ARM_BIN}/arm-none-eabi-size CACHE FILEPATH "")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft")
set(CMAKE_C_FLAGS_INIT   "${MCU_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS_INIT "${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${MCU_FLAGS} --specs=nano.specs --specs=nosys.specs")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
