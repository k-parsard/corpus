#! /bin/bash
set -e

cd inspo
cd nehe02
make 

if [ $? -ne 0 ]; then
  echo "make failed, exiting"
  exit 1
fi

dc-tool-ser -t /dev/ttyUSB0 -x nehe02.elf