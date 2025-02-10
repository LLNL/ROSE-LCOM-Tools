#!/bin/bash

# This script is designed to work as a drop-in replacement for calls to g++.
# It functions as a wrapper, calling g++ as per normal, but also calling the
# LCOM tool, with the same arguments under the hood.

# Log file location
GPPLOG="$LCOM_HOME/testcases/g++_log.txt"
LCOMLOG="$LCOM_HOME/testcases/lcom_log.txt"

# Log the date, time, and passed arguments to the log file
echo "$(date) - g++ $@" >> "$GPPLOG"

# Call g++ with all the passed arguments
"$GNAT_HOME/bin/g++" "$@"

# Call the LCOM tool too
"$LCOM_HOME/build/lcom.out" "$@" >> "$LCOMLOG" 2>&1 &

# Example usage for a cmake project:
# cd "$LCOM_HOME/testcases/<project-to-analyze>"
# cmake -DCMAKE_CXX_COMPILER="$LCOM_HOME/script/gcc_replace.sh" -S . -B build
# cmake --build build --parallel $(nproc)