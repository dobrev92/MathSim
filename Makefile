GLFW_INC_PATH = -I /usr/include/GLFW

CC = g++
CFLAGS = -Wall -c
OBJS = GLRender.o linear_algebra.o TwBarWrapper.o test1.o GlfwWrapper.o Objects.o
LIBS = -lGL -lGLEW -lglfw -lAntTweakBar 

test1: $(OBJS)
	$(CC) -o test1 -I /usr/include/GLFW $(LIBS) $(OBJS) shader.cpp

tesseract_demo: $(OBJS)
	$(CC) -o tesseract_demo -I /usr/include/GLFW $(LIBS) $(OBJS) shader.cpp

render/Render.h: mathematics/mathematica.h

render/GLRender.h: render/Render.h

test1.o: render/GLRender.h Objects.h GlfwWrapper.h test1.cpp
	$(CC) -c -I /usr/include/GLFW test1.cpp

tesseract_demo.o: render/GLRender.h Objects.h GlfwWrapper.h test1.cpp
	$(CC) -c -I /usr/include/GLFW test1.cpp

GLRender.o: render/Render.h render/GLRender.h render/GLRender.cpp shader.hpp
	$(CC) $(CFLAGS) -I /usr/include/GLFW render/GLRender.cpp

linear_algebra.o: mathematics/linear_algebra.h mathematics/linear_algebra.cpp
	$(CC) $(CFLAGS) mathematics/linear_algebra.cpp

TwBarWrapper.o: TwBarWrapper.h TwBarWrapper.cpp
	$(CC) $(CFLAGS) TwBarWrapper.cpp

GlfwWrapper.o: GlfwWrapper.h GlfwWrapper.cpp
	$(CC) $(CFLAGS) $(GLFW_INC_PATH) GlfwWrapper.cpp

Objects.o: Objects.h render/Render.h Objects.cpp shader.hpp
	$(CC) $(CFLAGS)  Objects.cpp 

clean:
	rm *.o test1
