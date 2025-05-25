#!/bin/bash

# Path to the file listing source files
SOURCE_LIST="file_path_names.txt"

# Read source files from the list
SRC_FILES=""
while IFS= read -r line; do
    SRC_FILES+="$line "
done < "$SOURCE_LIST"

# Compile using g++
g++ $SRC_FILES -o output -std=c++11 -I/opt/local/include -L/opt/local/lib -lraylib -framework OpenGL -lm -lpthread -ldl

# Run the output
if [ $? -eq 0 ]; then
    ./output
fi
