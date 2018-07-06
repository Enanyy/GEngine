#!/bin/bash
# 64 Bit Version

mingw32-make clean

mingw32-make BUILDMODE=static CC="gcc -m64 -O2"


echo 按任意键继续
read -n 1
echo 继续运行