# Compiler
CC = clang

# Directories
SRC_DIR = src
BUILD_DIR = build
LIBS_DIR = libs
BIN_DIR = bin

# Flags
CFLAGS = -I$(LIBS_DIR) -Wall -Wextra -pedantic  -std=c11 -Wgnu-zero-variadic-macro-arguments -Wno-gnu-zero-variadic-macro-arguments

# Debug flags
CFLAGS += -g

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))



# Targets
all: errno

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

errno: $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) errno.c $(OBJS) -o $(BIN_DIR)/errno $(CFLAGS) -L$(LIBS_DIR)


# Clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)

.PHONY: all clean