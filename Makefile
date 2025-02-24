# Compiler and flags
CXX = g++
CXXFLAGS =-std=c++20 -Wl,-Bstatic -lclicky -Wl,-Bdynamic -lcurl
ROOT_DIR = .
SRC_DIR = $(ROOT_DIR)/src
HEADERS = $(ROOT_DIR)/include
BUILD = dist/compiled
DIST = dist/build

EXEC = $(DIST)/servy
INSTALL_PATH = /usr/local/bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/%.o, $(SRCS))

# Default target
all: create_dirs $(EXEC)

# Create necessary directories
create_dirs:
	mkdir -p $(BUILD) && mkdir -p $(DIST)

# Clean target
clean:
	rm -rf dist/

# Compile the executable from object files
$(EXEC): $(OBJS)
	cp template/template.service /tmp/template.service
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC) -lcurl

# Compile each .cpp file into a .o file
$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c $< -o $@

# Check compilation (no output, just verify no errors)
check:
	$(CXX) $(CXXFLAGS) -I$(HEADERS) $(SRCS) -o /dev/null -lcurl
	@echo "Check complete: No errors found."

# Create the distribution tarball
dist: create_dirs
	mkdir -p $(DIST)/src
	mkdir -p $(DIST)/include
	cp $(EXEC) $(DIST)
	cp $(SRC_DIR)/* $(DIST)/src
	cp $(HEADERS)/* $(DIST)/include
	tar -czvf $(DIST)/servy.tar.gz -C $(DIST) servy src include

# Verify that the distribution works
distcheck: dist
	@mkdir -p /tmp/servy_distcheck
	@tar -xzf $(DIST)/servy.tar.gz -C /tmp/servy_distcheck
	@cd /tmp/servy_distcheck && $(MAKE) && $(MAKE) check
	@rm -rf /tmp/servy_distcheck
	@echo "Distcheck complete: Distribution builds and passes check successfully."

# Install the executable
install: $(EXEC)
	cp template/template.service /tmp/template.service
	install -m 755 $(EXEC) $(INSTALL_PATH)

# Uninstall the executable
uninstall:
	rm -f $(INSTALL_PATH)/servy

# Reset the build (clean and rebuild)
reset:
	$(MAKE) clean
	$(MAKE)

# Phony targets to prevent conflicts with files
.PHONY: all create_dirs clean check dist distcheck install uninstall reset

