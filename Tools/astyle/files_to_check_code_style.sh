#!/usr/bin/env bash
set -eu

PATTERN="-e ."

if [ $# -gt 0 ]; then
    PATTERN="$1"
fi

exec find Bootloader \
    -path Bootloader/Utility -prune -o \
    -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) | grep $PATTERN

exec find Tests \
    -path Tests/Unity -prune -o \
    -path Tests/CMock -prune -o \
    -path Tests/mocks -prune -o \
    -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) | grep $PATTERN
