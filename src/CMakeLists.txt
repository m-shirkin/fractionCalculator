cmake_minimum_required(VERSION 3.20)
project(fractions C CXX)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 20)

# Output a .wasm file rather than the default executable extension
add_executable(app main.cpp stream.cpp factors.cpp gcd.cpp)
set_target_properties(app PROPERTIES SUFFIX ".wasm")

target_compile_options(app PRIVATE -O2)