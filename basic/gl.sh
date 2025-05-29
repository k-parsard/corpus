#! /bin/bash
set -e

cd inspo
cd gl
dc-tool-ser -t /dev/ttyUSB0 -x gltest.elf