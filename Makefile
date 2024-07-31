TARGET    := oast
SRC_DIR   := src
BUILD_DIR := build
CFLAGS    := -Werror -Wall -Wextra -Wmissing-declarations -pedantic -std=c11 -ggdb

SRCS      := $(wildcard $(SRC_DIR)/*.c)
HEADERS   := $(wildcard $(SRC_DIR)/*.h)
OBJS      := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all
all: $(TARGET)

$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR):
	@mkdir -p $@

.PHONY: clean
clean:
	@rm -rf build/ $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

