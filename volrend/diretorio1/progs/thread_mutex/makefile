INCLUDE   = ./include
SRC       = ./source
BUILD     = ./build
OBJ_DIR   = $(BUILD)/objects
APP_DIR   = $(BUILD)/app
APPS      = ./app
OUTPUT    = ./output

TARGET_TM = $(APP_DIR)/main

FLAGS     = -O3 -Wall -std=c++0x -ltiff -pthread

all: libed apps

libed:                     \
	build                  \
	$(OBJ_DIR)/util.o      \
	$(OBJ_DIR)/option.o    \
	$(OBJ_DIR)/normal.o    \
	$(OBJ_DIR)/opacity.o   \
	$(OBJ_DIR)/octree.o    \
	$(OBJ_DIR)/file.o      \
	$(OBJ_DIR)/map.o       \
	$(OBJ_DIR)/view.o      \
	$(OBJ_DIR)/render.o    \
	$(OBJ_DIR)/adaptive.o  \
	$(OBJ_DIR)/raytrace.o

apps:                      \
	$(TARGET_TM)

build:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OUTPUT)

$(OBJ_DIR)/%.o: $(SRC)/%.cpp 
	g++ -c $< -I $(INCLUDE) $(FLAGS) -o $@

$(APP_DIR)/%: $(APPS)/%.cpp
	g++ -g $< $(OBJ_DIR)/*.o -I $(INCLUDE) $(FLAGS) -o $@

clean:
	rm -rf $(BUILD)
	rm -rf $(OUTPUT)