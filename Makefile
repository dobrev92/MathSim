GLFW_INC_PATH = -I /usr/include/GLFW

CC = g++
CFLAGS = -Wall -c
TESSERACT_DEMO_OBJS = GLRender.o linear_algebra.o TwBarWrapper.o GlfwWrapper.o Objects.o tesseract_demo.o
LIBS = -lGL -lGLEW -lglfw -lAntTweakBar 

tesseract_demo: $(TESSERACT_DEMO_OBJS)
	$(CC) -o tesseract_demo -I /usr/include/GLFW $(LIBS) $(TESSERACT_DEMO_OBJS) shader.cpp
	mkdir -p out
	mv *.o tesseract_demo ./out/
	cp -r shaders out/

render/Render.h: mathematics/mathematica.h

render/GLRender.h: render/Render.h

tesseract_demo.o: render/GLRender.h Objects.h GlfwWrapper.h tesseract_demo.cpp
	$(CC) -c -I /usr/include/GLFW tesseract_demo.cpp

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
	rm -r out
