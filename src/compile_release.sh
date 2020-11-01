rm CMakeCache.txt
rm -rf CMakeFiles
rm -rf cmake-build-debug

cmake -DCMAKE_BUILD_TYPE=Release .

make
