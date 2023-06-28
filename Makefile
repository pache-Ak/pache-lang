BUILD_DIR	:= build
SRC_DIR		:= src
AST_DIR		:= src/ast


# Flags
CFLAGS := -Wall -std=c11
CPPFLAGS := -Wall  -Wno-register -std=c++17 `llvm-config --cxxflags --ldflags --system-libs --libs core`
#`llvm-config --cxxflags --ldflags --system-libs --libs core`
FFLAGS :=
BFLAGS := -d
LDFLAGS :=

# Compilers
C := clang
CPP := clang++
FLEX := flex
BISON := bison

$(BUILD_DIR)/compiler:  $(BUILD_DIR)/libast.a $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/lexer.o
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/lexer.o  $(BUILD_DIR)/libast.a -ldl -o $(BUILD_DIR)/compiler


$(BUILD_DIR)/lexer.o: $(BUILD_DIR)/lexer.cpp $(BUILD_DIR)/parser.hpp
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/lexer.cpp -c -o $(BUILD_DIR)/lexer.o

$(BUILD_DIR)/parser.o: $(BUILD_DIR)/parser.cpp
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/parser.cpp -c -o $(BUILD_DIR)/parser.o

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(BUILD_DIR)/libast.a
		$(CPP) $(CPPFLAGS) $(SRC_DIR)/main.cpp $(BUILD_DIR)/libast.a -c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/libast.a: $(AST_DIR)/*.cpp
		$(CPP) $(CPPFLAGS) -c $(AST_DIR)/*.cpp  $(AST_DIR)/
		mv ./*.o $(BUILD_DIR)/
		ar -r $(BUILD_DIR)/libast.a $(BUILD_DIR)/*.o

$(BUILD_DIR)/lexer.cpp: $(SRC_DIR)/lexer.lpp
		flex -o $(BUILD_DIR)/lexer.cpp $(SRC_DIR)/lexer.lpp

$(BUILD_DIR)/parser.hpp: $(SRC_DIR)/parser.y $(AST_DIR)/*.h
		bison -d -o $(BUILD_DIR)/parser.cpp $(SRC_DIR)/parser.y

$(BUILD_DIR)/parser.cpp: $(SRC_DIR)/parser.y $(AST_DIR)/*.h $(AST_DIR)/*.cpp
		bison -d -o $(BUILD_DIR)/parser.cpp $(SRC_DIR)/parser.y

clean:
	-rm -rf $(BUILD_DIR)/
	mkdir $(BUILD_DIR)
