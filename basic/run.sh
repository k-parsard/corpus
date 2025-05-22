#! /bin/bash
set -e

cd ..
make

if [ $? -ne 0 ]; then
  echo "make failed, exiting"
  exit 1
fi

cd basic
dc-tool-ser -t /dev/ttyUSB0 -x lightgun.elf