CC=g++
CFLAGS=--std=c++11
SOURCE_DIR=./src
BIN_DIR=./

port_scanner: $(SOURCE_DIR)/port_scanner.cpp
	$(CC) $(CFLAGS) -o $(BIN_DIR)/port_scanner $(SOURCE_DIR)/port_scanner.cpp

puzzlesolver: $(SOURCE_DIR)/puzzlesolver.cpp
	$(CC) $(CFLAGS) -o $(BIN_DIR)/puzzlesolver $(SOURCE_DIR)/puzzlesolver.cpp