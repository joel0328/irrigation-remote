add_library(irr_core STATIC
  src/aes128.c
  src/counter.c
  src/frame.c
  src/handheld.c
  src/receiver.c
  src/runtimer.c
)
target_include_directories(irr_core PUBLIC include)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(irr_core PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion
                                          -Wstrict-prototypes -Wmissing-prototypes)
  if(IRR_WERROR)
    target_compile_options(irr_core PRIVATE -Werror)
  endif()
endif()
