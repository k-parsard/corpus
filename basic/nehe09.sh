#! /bin/bash
set -e

cd inspo
cd nehe09
make

if [ $? -ne 0 ]; then
  echo "make failed, exiting"
  exit 1
fi

dc-tool-ser -t /dev/ttyUSB0 -x nehe09.elf