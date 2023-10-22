
PROJ_NAME = cannons

INCLUDE_DIRS = include
LIB_DIRS = lib
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin


# Names of tools to use when building
CC = g++

C_FLAGS = -std=c++17 -I"$(INCLUDE_DIRS)"
L_FLAGS = -lfreeglut -lopengl32 -lglu32 -lstdc++ -L"$(LIB_DIRS)

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:src/%.cpp=obj/%.o)

.PHONY: all clean

all: $(BIN_DIR)/$(PROJ_NAME).exe
	
clean:
	@if exist $(OBJ_DIR)\*		del /Q $(OBJ_DIR)\*
	@if exist $(OBJ_DIR)			rmdir $(OBJ_DIR)
	@if exist $(BIN_DIR)\*		del /Q $(BIN_DIR)\*
	@if exist $(BIN_DIR)			rmdir $(BIN_DIR)
	
$(OBJ_DIR):
	@if not exist "$@" mkdir "$@"
	
$(BIN_DIR):
	@if not exist "$@" mkdir "$@"
	
$(BIN_DIR)/$(PROJ_NAME).exe: ${OBJS} | $(BIN_DIR)
	$(CC) -o "$@" ${OBJS} ${L_FLAGS}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(C_FLAGS)



