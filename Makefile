TARGET=main
COMPILER=g++
FLAGS=-Wall -std=c++17
INCLUDE=-I/Developer/Boost/boost_1_70_0 -Ilib

$(TARGET): src/main.cpp
	$(COMPILER) -o $(TARGET) $^ $(FLAGS)
