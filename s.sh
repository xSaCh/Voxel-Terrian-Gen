#!/bin/bash

CXX="g++"
CXX_FLAGS="-std=c++17" #-Wextra -Wall

SRC=`ls src/*.cpp src/**/*.cpp`
IMGUI_INCLUDE="./includes/imgui"
INCLUDE="./includes"
LIB="./lib"

LIBRARIES="-lglad -lglfw3dll -lassimp"
EXECUTABLE="main"

c=0
for i in $@; do
	if [[ $i != $1 ]]; then
		ARGS[$c]=$i
		c=$((c + 1))
	fi
done

Compile()
{
	echo $CXX -c -g $CXX_FLAGS ${ARGS[@]} -I$INCLUDE
	$CXX -c -g $CXX_FLAGS ${ARGS[@]} -I$INCLUDE
	mv *.o obj/
}

Build()
{
	echo $CXX -o bin/$EXECUTABLE obj/*.o imgui.a -L$LIB $LIBRARIES
	$CXX -o bin/$EXECUTABLE obj/*.o imgui.a -L$LIB $LIBRARIES
}

CompileR()
{
	echo $CXX -c -O2 $CXX_FLAGS ${ARGS[@]} -I$INCLUDE
	$CXX -c -O2 $CXX_FLAGS ${ARGS[@]} -I$INCLUDE
	mv *.o objR/
}

BuildR()
{
	echo $CXX -o bin/mainRe objR/*.o -L$LIB $LIBRARIES
	$CXX -o bin/mainRe objR/*.o -L$LIB $LIBRARIES
}

if [[ $i == "-cb" ]]; then
	Compile
	Build
elif [[ $1 == "-c" ]]; then
	Compile
elif [[ $i == "-b" ]]; then
	Build
elif [[ $1 == "-cr" ]]; then
	CompileR
elif [[ $i == "-br" ]]; then
	BuildR
fi