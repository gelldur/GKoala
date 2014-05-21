#!/bin/bash
mkdir -p bin
cd bin
mkdir -p debug
cd debug

echo""
echo""
cmake -DDEBUG=ON -DPLATFORM=LINUX ../../..
echo ""
echo""
echo "Build with $1"
time make $1
echo ""
echo ""
