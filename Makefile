# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS =

# Installation paths
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

# Source and build directories
SRCDIR = src
BUILDDIR = build

# Source and object files
SRC = $(SRCDIR)/dtree.c
OBJ = $(BUILDDIR)/dtree.o
BIN = dtree

# Default target
all: dirs $(BIN)

# Create necessary directories
dirs:
	@mkdir -p $(BUILDDIR)

# Compile source to object file
$(OBJ): $(SRC)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Link object file to create executable
$(BIN): $(OBJ)
	@echo "Linking $@..."
	@$(CC) $(OBJ) $(LDFLAGS) -o $@
	@echo "Build complete!"

# Install the program
install: $(BIN)
	@echo "Installing to $(BINDIR)..."
	@install -d $(DESTDIR)$(BINDIR)
	@install -m 755 $(BIN) $(DESTDIR)$(BINDIR)
	@echo "Installation complete!"

# Uninstall the program
uninstall:
	@echo "Uninstalling from $(BINDIR)..."
	@rm -f $(DESTDIR)$(BINDIR)/$(BIN)
	@echo "Uninstall complete!"

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILDDIR) $(BIN)
	@echo "Clean complete!"

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: clean all

# Run tests (placeholder)
test: $(BIN)
	@echo "Running tests..."
	./$(BIN) .

# Package source for distribution
dist: clean
	@echo "Creating distribution package..."
	@tar -czf dtree-1.0.tar.gz \
		--transform 's,^,dtree-1.0/,' \
		$(SRC) Makefile README.md LICENSE
	@echo "Package created: dtree-1.0.tar.gz"

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  install  - Install the program to $(BINDIR)"
	@echo "  uninstall- Remove the program from $(BINDIR)"
	@echo "  clean    - Remove build files"
	@echo "  debug    - Build with debug symbols"
	@echo "  test     - Run basic tests"
	@echo "  dist     - Create distribution package"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  CC      = $(CC)"
	@echo "  CFLAGS  = $(CFLAGS)"
	@echo "  PREFIX  = $(PREFIX)"

# Declare phony targets
.PHONY: all dirs install uninstall clean debug test dist help

# Build system information for debugging
info:
	@echo "Compiler: $(CC)"
	@echo "Compiler flags: $(CFLAGS)"
	@echo "Source files: $(SRC)"
	@echo "Object files: $(OBJ)"
	@echo "Binary: $(BIN)"
	@echo "Installation prefix: $(PREFIX)"
