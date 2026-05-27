set(CMAKE_SYSTEM_NAME WASI)
set(CMAKE_SYSTEM_PROCESSOR wasm32)

set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_C_COMPILER_TARGET   wasm32)
set(CMAKE_CXX_COMPILER_TARGET wasm32)

# Skip compiler checks — clang can't link a test executable without a sysroot
set(CMAKE_C_COMPILER_WORKS   1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Freestanding: no libc, no entry point, export everything
set(CMAKE_EXE_LINKER_FLAGS_INIT
    "-nostdlib -Wl,--no-entry")