rm -rf build
mkdir build
cd build
cmake -DPICO_SDK_PATH=/home/pi/pico/pico-sdk ..
make -j
