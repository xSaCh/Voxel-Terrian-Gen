#!/bin/bash

#CXX="g++"
CXX="clang++-12"
CXX_FLAGS="-std=c++17" #-Wextra -Wall

SRC=`ls src/*.cpp src/**/*.cpp`
IMGUI_INCLUDE="./includes/imgui"
INCLUDE="./includes"
LIB="./lib"

LIBRARIES="-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl"
EXECUTABLE="mainLC"
#OBJDIR="./objL"
OBJDIR="./objLC"

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
	mv *.o $OBJDIR
}

Build()
{
	echo $CXX -o bin/$EXECUTABLE $OBJDIR/*.o imgui.a -L$LIB $LIBRARIES
	$CXX -o bin/$EXECUTABLE $OBJDIR/*.o imgui.a -L$LIB $LIBRARIES
	
	# echo $CXX -o bin/$EXECUTABLE $OBJDIR/*.o $LIBRARIES
	# $CXX -o bin/$EXECUTABLE $OBJDIR/*.o $LIBRARIES
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

if [[ $1 == "-cb" ]]; then
	Compile
	Build
elif [[ $1 == "-c" ]]; then
	Compile
elif [[ $1 == "-b" ]]; then
	Build
elif [[ $1 == "-cr" ]]; then
	CompileR
elif [[ $1 == "-br" ]]; then
	BuildR
fi
