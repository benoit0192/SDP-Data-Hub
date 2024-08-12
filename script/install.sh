mkdir -p build
cd build
cmake .. -DENABLE_TESTING=OFF
cmake --build .
#cmake --build . --target install
#cmake --build . -DENABLE_TESTING=OFF --target install
