#!/usr/bin/env bash

echo "ONLY RUN FROM ROOT OF THE PROJECT OR I WILL BITE YOU"
echo "Seriously, you still can ^C"

echo -n "3..."
sleep 1;
echo -n "2..."
sleep 1;
echo -n "1..."
sleep 1;
echo "Beginning to bundle your thing"

echo "Removing ./package/"
rm -rf ./package
echo "Creating ./package/"
mkdir -p ./package
echo "Removing build directory"
rm -rf ./build
cmake -B build

pushd build
make -j12
popd

echo "Copying binary and libraries"
cp ./build/my-game ./package/
cp ./build/lib* ./package/

# TODO: fix glfw

# FIXME it shouldn't store them raw
echo "Copying assets"
cp -r assets ./package

echo "Making tar archive"
tar czf package.tar.gz ./package/

echo "Your bundle is available at $(pwd)/package/ or $(pwd)/package.tar.gz UwU"
