#include "render/GLRender.h"
#include "GlfwWrapper.h"
#include "TwBarWrapper.h"
#include "Objects.h"

#define GUI

__GlfwWrapper* glfw;
__GLRender* render;
#ifdef GUI
__AntTweakBarWrapper* twBar;
#endif
__Camera* camera;
bool rotating;

void mouseMove(double x, double y);
void mouseButton(int button, int action, int mods);
void onSizeChange(int width, int height);
void onPreContextSwitch(bool fullscreen, unsigned int samples, int width, int height);
void onPostContextSwitch(bool fullscreen, unsigned int samples, int width, int height);
void ToggleFullscreen(unsigned int samples, int width, int height);
void UpdateCamera(__Camera* camera, float deltaTime);

int main()
{
    rotating = false;
    glfw = new __GlfwWrapper();
    render = new __GLRender();
    __scalar angVelocity = 0;
#ifdef GUI
    twBar = new __AntTweakBarWrapper();
    twBar->SizeChange(848, 480);

    twBar->CreateSystemSettingsBar(&ToggleFullscreen);

    __TwBar* bar = new __TwBar("Rotation");
    __TwVariable* rotation = new __TwVariable(bar, "Angular Velocity", TW_TYPE_FLOAT, &angVelocity, "step=0.01");
    bar->AddVariable(rotation);
    twBar->AddBar(bar);
#endif
    glfw->SetMousePosFunc(&mouseMove);
    glfw->SetMouseButtonFunc(&mouseButton);
    glfw->SetWindowSizeFunc(&onSizeChange);
    glfw->SetPreContextSwitchCallback(&onPreContextSwitch);
    glfw->SetPostContextSwitchCallback(&onPostContextSwitch);
    
    camera = new __Camera(__Vector4(0,0,10,1));
    camera->SetProjectionMatrix(__Matrix4x4PerspectiveFov(__PI/4, (__scalar)16/9, 1, 1000));
    //__Line* line = new __Line(render, __Vector4(0, 3, 0, 1),50);
    __HypercubeProjection* cube = new __HypercubeProjection(render, __Vector4(0,0,0,1));
    __Plane* plane = new __Plane(render, __Vector4(0,-2,0,1), 10, 10);
    plane->SetScale(__Vector4(10,10,10,0));
    //__Sphere* sphere = new __Sphere(render, __Vector4(0,0,0,1),5,20,20);
    //sphere->SetWireframeLineWidth(3);
    cube->SetWireframeLineWidth(1);
    
    float deltaTime = 0;
    while(glfw->IsInRenderLoop())
    {
        deltaTime = glfw->GetTimeElapsed();
        glfw->ResetTimer();
        UpdateCamera(camera, deltaTime);
        render->ClearScreen();
        cube->RotateXZ(deltaTime*angVelocity);
        //line->Render(camera, true);
        plane->Render(camera, true);
        cube->Render(camera, true);
        //sphere->Render(camera, true);
#ifdef GUI
        twBar->Render();
#endif
        glfw->SwapBuffers();
    }
    delete cube;
    delete plane;
    //delete sphere;
#ifdef GUI
    delete twBar;
#endif
    delete render;
    delete glfw;
    return 0;
}

void mouseMove(double x, double y)
{
#ifdef GUI
    if(!rotating)
        if(twBar->MouseMotion(x,y))
        {
        }
    else
    {
    }
#endif

}

void mouseButton(int button, int action, int mods)
{
#ifdef GUI
    if(action==GLFW_PRESS)
    {
        if(!twBar->MouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT))
        {
            glfwSetInputMode(glfw->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfw->BeginMouseRelativeMove();
            rotating = true;
        }   
    }   
    if(action==GLFW_RELEASE)
    {
        if(!twBar->MouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT))
        {
            glfwSetInputMode(glfw->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            rotating = false;
        }   
    }
#endif
}

void onSizeChange(int width, int height)
{
    render->ViewPort(0, 0, width, height);
#ifdef GUI
    twBar->SizeChange(width, height);
#endif
    camera->SetProjectionMatrix(__Matrix4x4PerspectiveFov(__PI/4, (__scalar)width/height, 1, 1000));
}

void onPreContextSwitch(bool fullscreen, unsigned int samples, int width, int height)
{
#ifdef GUI
    twBar->PreContextSwitch();
#endif
    render->PreContextSwitch();
    camera->SetProjectionMatrix(__Matrix4x4PerspectiveFov(__PI/4, (__scalar)width/height, 1, 1000));
}

void onPostContextSwitch(bool fullscreen, unsigned int samples, int width, int height)
{
    render->PostContextSwitch();
#ifdef GUI
    twBar->PostContextSwitch();
    twBar->SizeChange(width, height);
#endif
}

void ToggleFullscreen(unsigned int samples, int width, int height)
{
    if(glfw->GetFullscreen())
        glfw->ContextSwitch(false, samples, width, height);
    else
        glfw->ContextSwitch(true, samples, width, height);
}

void UpdateCamera(__Camera* camera, float deltaTime)
{
    if(glfw->IfKeyPressed(GLFW_KEY_W))
        camera->UpdatePos(deltaTime*20);
    if(glfw->IfKeyPressed(GLFW_KEY_S))
        camera->MoveBasisZ(-deltaTime*20);
    if(glfw->IfKeyPressed(GLFW_KEY_A))
        camera->MoveBasisX(-deltaTime*20);
    if(glfw->IfKeyPressed(GLFW_KEY_D))
        camera->MoveBasisX(deltaTime*20);

    if(rotating)
    {
        float x, y;
        glfw->GetMouseRelativeMovement(&x, &y);
        camera->RotateXZ(deltaTime*x*0.1);
        camera->RotateZY(deltaTime*y*0.1);
    }
}


