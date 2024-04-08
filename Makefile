CXX := g++
CXX_FLAGS   := -g -std=c++17 #-Wextra -Wall

SRC := src/*.cpp src/**/*.cpp
INCLUDE := ./includes
IMGUI_INCLUDE := ./includes/imgui
LIB := ./lib

LIBRARIES   :=  -lglfw3dll
EXECUTABLE  := main

build:
	$(CXX) -c $(CXX_FLAGS) $(SRC) -I$(INCLUDE)
	mv *.o obj/

a:
	echo $(SRC)

run:
	$(CXX) -o bin/$(EXECUTABLE) obj/*.o -L$(LIB) $(LIBRARIES)

buildRun:
	$(CXX) $(CXX_FLAGS) $(SRC) -I$(INCLUDE) -L$(LIB) $(LIBRARIES) -o bin/$(EXECUTABLE)

buildImgui:
	$(CXX) -c $(IMGUI_INCLUDE)/imgui*.cpp -I$(INCLUDE) -L$(LIB) $(LIBRARIES)
	ar rvs imgui.a imgui*.o
	rm imgui*.o
