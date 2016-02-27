#include "GLSystemWrapper.h"
#include <iostream>


__GLSystemWrapper::__GLSystemWrapper()
{
    programID = 0;
    MatrixID = 0;
    VertexArrayID = 0;
    ViewMatrix = __Matrix4x4Identity();
}

int __GLSystemWrapper::Init()
{
    if( !glfwInit() )
    {
        return __ERR_GLFW_INIT_FAIL;
    }

    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 800, 480, "Simulation Window", NULL, NULL);
    if( window == NULL )
    {
        glfwTerminate();
        return __ERR_GLFW_WINDOW_FAIL;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) 
    {
        return __ERR_GLEW_INIT_FAIL;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "SimpleTransform.vertexshader", "SingleColor.fragmentshader" );

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");

    InitGUI();

    glfwSetTime(0);
}

int __GLSystemWrapper::InitGUI()
{
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(800, 480);
    glfwSetCursorPosCallback(window, __GLSystemWrapper::cursor_position_callback);
    glfwSetMouseButtonCallback(window, __GLSystemWrapper::mouse_button_callback);
    glfwSetWindowSizeCallback(window, __GLSystemWrapper::window_size_callback);
    return __SUCCESS;
}

__scalar __GLSystemWrapper::GetTimeElapsed()
{
    return deltaTime;
}

int __GLSystemWrapper::RenderGUI()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    TwDraw();
    return __SUCCESS;
}

bool __GLSystemWrapper::IsInRenderLoop()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
        return true;
    else
        return false;
}

int __GLSystemWrapper::PreRender()
{
    deltaTime = (__scalar)glfwGetTime();
    glfwSetTime(0);

    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);

    return __SUCCESS;
}

int __GLSystemWrapper::PostRender()
{
    RenderGUI();
    glfwPollEvents();
    glfwSwapBuffers(window);
    return __SUCCESS;
}

//callbacks 
void __GLSystemWrapper::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(!RotateCamera)
    TwMouseMotion((int)xpos,(int)ypos);
}

void __GLSystemWrapper::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        if(!TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT))
        {
            RotateCamera = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(window, &refX, &refY);
        }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        if(!TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT))
        {
            RotateCamera = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
}

void __GLSystemWrapper::window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
    ProjectionMat = __Matrix4x4PerspectiveFov(__PI/4, (__scalar)width/height, 1, 100);
    TwWindowSize(width, height);
}

void __GLSystemWrapper::GetMouseDeltaPos(__scalar* x, __scalar* y)
{
    double deltaX, deltaY;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    *x = (__scalar)(refX - xpos);
    *y = (__scalar)(refY - ypos);
    glfwSetCursorPos(window, refX, refY);
}



__GLSystemWrapper::~__GLSystemWrapper()
{
    glDeleteProgram(programID);
    TwTerminate();
    glDeleteVertexArrays(1, &VertexArrayID);
    glfwTerminate();
}

__Matrix4x4 __GLSystemWrapper::ProjectionMat = __Matrix4x4PerspectiveFov(__PI/4, (__scalar)16/9, 1, 100);
bool __GLSystemWrapper::RotateCamera = false;
double __GLSystemWrapper::refX = 800/2;
double __GLSystemWrapper::refY = 480/2;
