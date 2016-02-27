all:
	g++ -o opgl -I /usr/include/GLFW -lGL -lGLEW -lglfw -lAntTweakBar  mathematics/linear_algebra.cpp shader.cpp GLSystemWrapper.cpp Objects.cpp main.cpp
