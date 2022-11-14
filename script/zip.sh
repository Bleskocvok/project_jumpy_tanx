#!/bin/sh

[ -n "$1" ] || { echo "usage: $0 <version>"; exit 1; }

version="$1"

echo "version: '$version'"

folder="jumpy_tanx_v$version"
file="jumpy_tanx_v$version.zip"

rm -rf "$folder"

mkdir "$folder"

cp -r bin-lin/* "$folder/"
cp -r bin-win/* "$folder/"

rm "$file"

zip -r "$file" \
    "$folder" \
    -x *.a \
        '*/CMakeFiles/*' \
        '*/libraries/*' \
        "./$folder/CMakeCache.txt" \
        "./$folder/CMakeDoxyfile.in" \
        "./$folder/CMakeDoxygenDefaults.cmake" \
        "./$folder/cmake_install.cmake" \
        "./$folder/Makefile"

echo "Check:"

zip -sf "$file"
