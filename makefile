# Makefile settings
headers = -I/usr/lib/include/ -I./include/
libs = `pkg-config --cflags --libs opencv`
build = ./obj/
src = ./src/
detection=./src/detection/
cc = g++

# Example
assignment = main

# Build Rule

# Example Objects
objects := $(patsubst ${src}/%.cpp,${build}/%.o,$(wildcard ${src}/*.cpp))

# Build Rule
${assignment} : ${objects}
	${cc} -o ${assignment} ${objects} ${libs} ${frameworks}

# Object Build Rule
${build}/%.o : ${src}/%.cpp
	$(cc) -o $@ -c $^ ${headers} ${frameworks}



# Clean
clean:
	rm ${build}/*.o
	rm ${assignment}