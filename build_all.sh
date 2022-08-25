if [ -d "build" ]; then
   rm -rf ./build
fi

mkdir build

cd build

cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j4