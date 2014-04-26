#!/bin/bash
astyle --options=./format_style.astyle --recursive "./*.h" "./*.cpp"
#Unused option
# --exclude=libs
