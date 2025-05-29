#! /bin/bash
set -e

make

if [ $? -ne 0 ]; then
  echo "make failed, exiting"
  exit 1
fi

dc-tool-ser -t /dev/ttyUSB0 -x nehe26.elf