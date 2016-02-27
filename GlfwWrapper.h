#ifndef __GlfwWrapper_H_
#define __GlfwWrapper_H_

#include <GL/glew.h>
#include <glfw3.h>

class __GlfwWrapper
{
    protected:
        GLFWwindow* window;
        bool IsFullscreen;
        static float refX;
        static float refY;
        int CreateWindow(bool fullscreen, unsigned int samples, int width, int height);
        static void (*mousePosFunc)(double, double);
        static void (*mouseButtonFunc)(int, int, int);
        static void (*windowSizeFunc)(int, int);
        void (*PreContextSwitchCallback)(bool, unsigned int, int, int);
        void (*PostContextSwitchCallback)(bool, unsigned int, int, int);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void window_size_callback(GLFWwindow* window, int width, int height);
    public:
        __GlfwWrapper();
        __GlfwWrapper(bool fullscreen, unsigned int samples, int width, int height);
        int Init(bool fullscreen, unsigned int samples, int width, int height);
        bool IsInRenderLoop();
        bool GetFullscreen(){return IsFullscreen;}
        int SwapBuffers();
        float GetTimeElapsed();
        GLFWwindow* GetWindow(){return window;}
        void ResetTimer();
        int ContextSwitch(bool fullscreen, unsigned int samples, int width, int height);
        void SetPreContextSwitchCallback(void (*m_PreContextSwitchCallback)(bool, unsigned int, int, int)){PreContextSwitchCallback = m_PreContextSwitchCallback;}
        void SetPostContextSwitchCallback(void (*m_PostContextSwitchCallback)(bool, unsigned int, int, int)){PostContextSwitchCallback = m_PostContextSwitchCallback;}
        void SetMousePosFunc(void (*m_mousePosFunc)(double, double)){mousePosFunc = m_mousePosFunc;}
        void SetMouseButtonFunc(void (*m_mouseButtonFunc)(int, int, int)){mouseButtonFunc = m_mouseButtonFunc;}
        void SetWindowSizeFunc(void (*m_windowSizeFunc)(int, int)){windowSizeFunc = m_windowSizeFunc;}
        void BeginMouseRelativeMove();
        void GetMouseRelativeMovement(float* x, float* y);
        bool IfKeyPressed(int key);
        ~__GlfwWrapper();

        
};

#endif
