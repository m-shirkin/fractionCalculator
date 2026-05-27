cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=toolchain-wasm.cmake -G Ninja
cmake --build build
copy build\app.wasm ..\lib.wasm