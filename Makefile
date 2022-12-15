BUILD_DIR	:= build
SRC_DIR		:= src
AST_DIR		:= ast


# Flags
CFLAGS := -Wall -std=c11
CPPFLAGS := -Wall -Wno-register -std=c++17
#`llvm-config --cxxflags --ldflags --system-libs --libs core`
FFLAGS :=
BFLAGS := -d
LDFLAGS :=

# Compilers
C := clang
CPP := clang++
FLEX := flex
BISON := bison

$(BUILD_DIR)/compiler: clean $(BUILD_DIR)/type.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/lexer.o
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/type.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/lexer.o -ldl -o $(BUILD_DIR)/compiler


$(BUILD_DIR)/lexer.o: $(BUILD_DIR)/lexer.cpp $(BUILD_DIR)/parser.hpp
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/lexer.cpp -c -o $(BUILD_DIR)/lexer.o

$(BUILD_DIR)/parser.o: $(BUILD_DIR)/parser.cpp
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/parser.cpp -c -o $(BUILD_DIR)/parser.o

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(BUILD_DIR)/type.o
		$(CPP) $(CPPFLAGS) $(SRC_DIR)/main.cpp $(BUILD_DIR)/type.o -c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/type.o: $(AST_DIR)/type.cpp
		$(CPP) $(CPPFLAGS) $(AST_DIR)/type.cpp -c -o $(BUILD_DIR)/type.o

$(BUILD_DIR)/lexer.cpp: $(SRC_DIR)/lexer.l
		flex -o $(BUILD_DIR)/lexer.cpp $(SRC_DIR)/lexer.l

$(BUILD_DIR)/parser.hpp: $(SRC_DIR)/parser.y $(AST_DIR)/*.h
		bison -d -o $(BUILD_DIR)/parser.cpp $(SRC_DIR)/parser.y

$(BUILD_DIR)/parser.cpp: $(SRC_DIR)/parser.y $(AST_DIR)/*.h $(AST_DIR)/*.cpp
		bison -d -o $(BUILD_DIR)/parser.cpp $(SRC_DIR)/parser.y

clean:
	-rm -rf $(BUILD_DIR)/
	mkdir $(BUILD_DIR)
