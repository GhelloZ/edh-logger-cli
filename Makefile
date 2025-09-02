CC = gcc
AR = ar
CFLAGS = -Wall -I./lib

BUILD_DIR := build
BUILD_LIB_DIR := $(BUILD_DIR)/lib
BUILD_OBJ := $(patsubst lib/%.c,$(BUILD_LIB_DIR)/%.o,$(wildcard lib/*.c))
# static archive name is libedh.a so -ledh will link it
LIB_ARCHIVE := $(BUILD_DIR)/libedh.a
BIN := $(BUILD_DIR)/edh

.PHONY: all clean

all: $(BIN)

# Archive of C objects (placed in build/)
$(LIB_ARCHIVE): $(BUILD_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(AR) rcs $@ $^

# Compile C sources into object files under build/lib/
# rule: build/lib/<name>.o <- lib/<name>.c
$(BUILD_LIB_DIR)/%.o: lib/%.c
	@mkdir -p $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build Go CLI, linking against the static C library in build/
# Set cgo linker flags so the linker can find libedh.a
$(BIN): $(LIB_ARCHIVE)
	@mkdir -p $(BUILD_DIR)
	# Tell cgo/linker to look in build/ for libedh.a and link with -ledh
	CGO_LDFLAGS="-L$(BUILD_DIR) -ledh" go build -o $(BIN)

clean:
	rm -rf $(BUILD_DIR)

