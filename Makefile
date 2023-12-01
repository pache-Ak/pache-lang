BUILD_DIR	:= build
SRC_DIR		:= src
AST_DIR		:= src/ast
IR_DIR 		:= src/IRbuild

BOOST_INCLUDE_DIR	:= /home/pache/Boost/boost_1_83_0
BOOST_LIBRARY_DIR	:= /home/pache/Boost/boost_1_83_0/stage/lib

# Flags
CFLAGS := -Wall -std=c11
CPPFLAGS := -Wall -fno-exceptions -I$(SRC_DIR) -I$(BOOST_INCLUDE_DIR) -L$(BOOST_LIBRARY_DIR) -D BOOST_NO_EXCEPTIONS -Wno-register -std=c++17 `llvm-config --cxxflags --ldflags --system-libs --libs core`

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


$(BUILD_DIR)/lexer.o: $(BUILD_DIR)/lexer.cpp $(BUILD_DIR)/parser.tab.hpp
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/lexer.cpp -c -o $(BUILD_DIR)/lexer.o

$(BUILD_DIR)/parser.o: $(BUILD_DIR)/parser.tab.cc
		$(CPP) $(CPPFLAGS) $(BUILD_DIR)/parser.tab.cc -c -o $(BUILD_DIR)/parser.o

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(BUILD_DIR)/libast.a
		$(CPP) $(CPPFLAGS) $(SRC_DIR)/main.cpp $(BUILD_DIR)/libast.a -c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/libast.a: $(AST_DIR)/*.cpp $(BUILD_DIR)/libIR.a
		$(CPP) $(CPPFLAGS) -c $(AST_DIR)/*.cpp  $(AST_DIR)/
		mv ./*.o $(BUILD_DIR)/
		ar -r $(BUILD_DIR)/libast.a $(BUILD_DIR)/*.o
$(BUILD_DIR)/libIR.a : $(IR_DIR)/*.cpp
		$(CPP) $(CPPFLAGS) -c $(IR_DIR)/*.cpp  $(IR_DIR)/
		mv ./*.o $(BUILD_DIR)/
		ar -r $(BUILD_DIR)/libIR.a $(BUILD_DIR)/*.o

$(BUILD_DIR)/lexer.cpp: $(SRC_DIR)/lexer.lpp
		flex -o $(BUILD_DIR)/lexer.cpp $(SRC_DIR)/lexer.lpp

$(BUILD_DIR)/parser.tab.hpp: $(SRC_DIR)/parser.y $(AST_DIR)/*.h
		bison -d -o $(BUILD_DIR)/parser.tab.cc $(SRC_DIR)/parser.y

$(BUILD_DIR)/parser.tab.cc: $(SRC_DIR)/parser.y $(AST_DIR)/*.h $(AST_DIR)/*.cpp
		bison -d -o $(BUILD_DIR)/parser.tab.cc $(SRC_DIR)/parser.y

clean:
	-rm -rf $(BUILD_DIR)/
	mkdir $(BUILD_DIR)
