#!/bin/bash
cmake -DPLATFORM=LINUX -DCMAKE_BUILD_TYPE=Debug -D CMAKE_ECLIPSE_VERSION=4.3 -G"Eclipse CDT4 - Unix Makefiles" ..
