#
# Nehe02 example adapted from KGL
# (c)2013-2014 Josh Pearson
# (c)2001 Benoit Miller
#   

TARGET = game.elf
OBJS = game.o romdisk.o
KOS_ROMDISK_DIR = romdisk

all: clean $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(TARGET) $(OBJS)
	-rm -f romdisk.o romdisk.img

rm-elf:
	-rm -f $(TARGET)

$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -L$(KOS_BASE)/lib -lGL

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
	-rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)

