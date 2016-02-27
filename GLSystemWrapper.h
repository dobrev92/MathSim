#ifndef __GLSystemWrapper_H_
#define __GLSystemWrapper_H_

#include <GL/glew.h>

//I use GLFW
#include <glfw3.h>

#include "shader.hpp"

#include "mathematics/mathematica.h"
#include <AntTweakBar.h>


//Error definitions

//No error encountered
#define __SUCCESS 0

//Error encountered while initializing GLFW
#define __ERR_GLFW_INIT_FAIL 0x1

//Error encountered while opening GLFW window
//possible reason is that the GPU does not support OpenGL 3.3 context
#define __ERR_GLFW_WINDOW_FAIL 0x2

//Error encountered while initializing GLEW
#define __ERR_GLEW_INIT_FAIL 0x3

class __ShaderManager
{
    private:
    public:
};

class __GLSystemWrapper
{
    private:
        GLFWwindow* window;
        GLuint VertexArrayID;
        GLuint programID;
        GLuint MatrixID;
        __Matrix4x4 ViewMatrix;
        static __Matrix4x4 ProjectionMat;

        __scalar deltaTime;

        int InitGUI();
        int RenderGUI();
        void HandleGUIInput();

        //get the elapsed time from the last call, I use it for measuring the 
        //elapsed time from the last frame
        
        //this one is true when left mouse is pressed and the camera is rotating
        static bool RotateCamera;
        //these are to compute the relative motion of the mouse, again for the camera
        static double refX;
        static double refY;

        //callback functions
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void window_size_callback(GLFWwindow* window, int width, int height);

    public:
        __GLSystemWrapper();
        int Init();

        GLFWwindow* GetWindow(){return window;}
        GLuint GetTransformMatrixID(){return MatrixID;}
        GLuint GetShaderID(){return programID;}

        __Matrix4x4 GetMVP(){return ProjectionMat*ViewMatrix;}
        bool Rotating(){return RotateCamera;}

        //get the change in the mouse position
        void GetMouseDeltaPos(__scalar* x, __scalar* y);

        __scalar GetTimeElapsed();

        //Update View Matrix
        void UpdateViewMatrix(__Matrix4x4 mat){ViewMatrix = mat;}
        //This function is to determine whether it is safe for 
        //OpenGL to render data;
        bool IsInRenderLoop();

        //Clear the screen and read input
        int PreRender();

        //Swap buffers
        int PostRender();

        //Destructor
        ~__GLSystemWrapper();

};

#endif
