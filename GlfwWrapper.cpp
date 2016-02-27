#include "GlfwWrapper.h"

__GlfwWrapper::__GlfwWrapper()
{
    window = NULL;
    PreContextSwitchCallback = NULL;
    PostContextSwitchCallback = NULL;
    Init(false, 1, 848, 480);
}

__GlfwWrapper::__GlfwWrapper(bool fullscreen, unsigned int samples, int width, int height)
{
    window = NULL;
    PreContextSwitchCallback = NULL;
    PostContextSwitchCallback = NULL;
    Init(fullscreen, samples, width, height);
}

int __GlfwWrapper::Init(bool fullscreen, unsigned int samples, int width, int height)
{
    IsFullscreen = fullscreen;
    if( !glfwInit() )
    {
        return 1;
    }

    CreateWindow(fullscreen, samples, width, height);


    if( window == NULL )
    {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        return 0;
    }

    glfwSetTime(0);

    return 1;
}

bool __GlfwWrapper::IsInRenderLoop()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
        return true;
    else
        return false;
}

float __GlfwWrapper::GetTimeElapsed()
{
    return (float)glfwGetTime();
}

void __GlfwWrapper::ResetTimer()
{
    glfwSetTime(0);
}

int __GlfwWrapper::SwapBuffers()
{
    glfwPollEvents();
    glfwSwapBuffers(window);
    return 1;
}

int __GlfwWrapper::CreateWindow(bool fullscreen, unsigned int samples, int width, int height)
{
    glfwWindowHint(GLFW_SAMPLES, samples);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(fullscreen)
        window = glfwCreateWindow( width, height, "Simulation Window", glfwGetPrimaryMonitor(), NULL);
    else
        window = glfwCreateWindow( width, height, "Simulation Window", NULL, NULL);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPosCallback(window, __GlfwWrapper::cursor_position_callback);
    glfwSetMouseButtonCallback(window, __GlfwWrapper::mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);


    if(!window)
        return 0;
    else
        return 1;
}

int __GlfwWrapper::ContextSwitch(bool fullscreen, unsigned int samples, int width, int height)
{
    IsFullscreen = fullscreen;

    if(PreContextSwitchCallback)
        (*PreContextSwitchCallback)(fullscreen, samples, width, height);

    if(window)
        glfwDestroyWindow(window);

    CreateWindow(fullscreen, samples, width, height);
    glfwMakeContextCurrent(window);

    // Initialize GLEW 
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {   
        return 1;
    }

    if(PostContextSwitchCallback)
        (*PostContextSwitchCallback)(fullscreen, samples, width, height);
    
    glfwSetTime(0);

    return 1;
}

void __GlfwWrapper::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(mousePosFunc)
        (*mousePosFunc)(xpos, ypos);
}

void __GlfwWrapper::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(mouseButtonFunc)
        (*mouseButtonFunc)(button, action, mods);
}

void __GlfwWrapper::window_size_callback(GLFWwindow* window, int width, int height)
{
    if(windowSizeFunc)
        (*windowSizeFunc)(width, height);
}

bool __GlfwWrapper::IfKeyPressed(int key)
{
    if(glfwGetKey(window, key) == GLFW_PRESS)
        return true;
    else
        return false;
}

void __GlfwWrapper::BeginMouseRelativeMove()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    refX = x;
    refY = y;
}

void __GlfwWrapper::GetMouseRelativeMovement(float* x, float* y)
{
    double posX, posY;
    glfwGetCursorPos(window, &posX, &posY);
    *x = float(posX) - refX;
    *y = float(posY) - refY;
    glfwSetCursorPos(window, (double)refX, (double)refY);
}

__GlfwWrapper::~__GlfwWrapper()
{
    if(window)
        glfwDestroyWindow(window);
    mousePosFunc = NULL;
    mouseButtonFunc = NULL;
    windowSizeFunc = NULL;
    glfwTerminate();
}

float __GlfwWrapper::refX = 0;
float __GlfwWrapper::refY = 0;
void (*__GlfwWrapper::mousePosFunc)(double, double) = NULL;
void (*__GlfwWrapper::mouseButtonFunc)(int, int, int) = NULL;
void (*__GlfwWrapper::windowSizeFunc)(int, int) = NULL;

