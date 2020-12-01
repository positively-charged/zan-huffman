# Zandronum Huffman library Makefile

# NOTE: Execute the `make` command from the directory which contains this
# Makefile. This Makefile will create a build directory in the current
# directory. I would have preferred to have the file paths be relative to the
# Makefile directory, but I can't figure out how to do that in GNU make. 

CC=gcc
CFLAGS=-std=c99 -fPIC
BUILD_DIR=build
SRC_DIR=src
INCLUDES=-I$(SRC_DIR)

$(BUILD_DIR)/huffman.so: \
	$(BUILD_DIR) \
	$(BUILD_DIR)/huffman.o \
	$(BUILD_DIR)/bitreader.o \
	$(BUILD_DIR)/bitwriter.o \
	$(BUILD_DIR)/huffcodec.o
	$(CC) -shared -o $@ $(filter %.o,$^)

# Sets up build directory.
$(BUILD_DIR):
	@if ! [ -d "$(BUILD_DIR)" ]; then \
		mkdir "$(BUILD_DIR)"; \
	fi

# Compile files in src directory.
$(BUILD_DIR)/huffman.o: \
	$(SRC_DIR)/huffman.c \
	$(SRC_DIR)/huffman.h \
	$(SRC_DIR)/huffcodec.h \
	$(SRC_DIR)/codec.h \
	$(SRC_DIR)/bitwriter.h \
	$(SRC_DIR)/def.h \
	$(SRC_DIR)/bitreader.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/bitreader.o: \
	$(SRC_DIR)/bitreader.c \
	$(SRC_DIR)/bitreader.h \
	$(SRC_DIR)/def.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/bitwriter.o: \
	$(SRC_DIR)/bitwriter.c \
	$(SRC_DIR)/bitwriter.h \
	$(SRC_DIR)/def.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/huffcodec.o: \
	$(SRC_DIR)/huffcodec.c \
	$(SRC_DIR)/huffcodec.h \
	$(SRC_DIR)/codec.h \
	$(SRC_DIR)/bitwriter.h \
	$(SRC_DIR)/bitreader.h \
 	$(SRC_DIR)/def.h
	$(CC) -c $(CFLAGS) $< -o $@

# Test.
test: tests/test.c $(BUILD_DIR)/huffman.so
	$(CC) -std=c99 $(INCLUDES) $^ -o $(BUILD_DIR)/test
	$(BUILD_DIR)/test