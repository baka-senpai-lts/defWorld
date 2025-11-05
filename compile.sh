#!/usr/bin/env bash

rm -rf build_linux
rm -rf build_windows

cmake -B build_linux
cmake -B build_windows -DCMAKE_TOOLCHAIN_FILE=mingw-toolchain.cmake

pushd build_linux || exit
make -j12
popd || exit

pushd build_windows || exit
make -j12
popd || exit

echo "Build complete!"
echo "Binaries are located in:"
echo -e "\t$(pwd)/build_linux/my-game"
echo -e "\t$(pwd)/build_windows/my-game.exe"
