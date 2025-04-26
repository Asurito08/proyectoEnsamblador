# Makefile para proyecto GTK4 en C

CC = gcc
SRC = src/main.c
BUILD_DIR = build
TARGET = $(BUILD_DIR)/mi_app

# GTK4 flags
CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

# Compilar
all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Ejecutar
run: $(TARGET)
	./$(TARGET)

# Limpiar
clean:
	rm -rf $(BUILD_DIR)
