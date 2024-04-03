CXX = g++

SRC = src/main.cpp
SRC_SUB = src/**/*.cpp

INCLUDE = -I./include

CXXFLAGS = -Wall -std=c++17  $(INCLUDE)

TARGET = img_metadata

.PHONY: build clean

all: build/$(TARGET)

build: # Create a build directory if it doesn't exist
	@if [ ! -d "build" ]; then mkdir -p build; fi 

build/$(TARGET): build $(SRC) $(SRC_SUB)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(SRC_SUB)

clean:
	rm -rf build/*
