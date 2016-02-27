#include "TwBarWrapper.h"

//__TwVariable***********************************************************************************************************

__TwVariable::__TwVariable(__TwBar* m_bar, const char* m_name, TwType m_type, void* m_var, const char* m_def)
{
    name = (char*)m_name;
    def = (char*)m_def;
    type = m_type;
    var = m_var;
    bar = m_bar;
    Init();
}

int __TwVariable::Init()
{
    std::cout<<"Inititalizing __TwVariable...\n";
    TwAddVarRW(bar->GetTwBarPtr(), (const char*)name, type, var, (const char*)def);
    std::cout<<"Initialization of __TwVariable completed\n";
    return 1;
}
__TwBar::__TwBar(const char* m_name)
{   
    name = (char*)m_name;
    Init();
}

//__TwButton*************************************************************************************************************

__TwButton::__TwButton(__TwBar* m_bar, const char* m_name, TwButtonCallback m_callback, void* m_clientData, const char* m_def)
{
    name = (char*)m_name;
    def = (char*)m_def;
    clientData = m_clientData;
    bar = m_bar;
    callback = m_callback;
    Init();
}

int __TwButton::Init()
{
    std::cout<<"Initializing __TwButton...\n";
    TwAddButton(bar->GetTwBarPtr(), (const char*)name, callback, clientData, (const char*)def);
    std::cout<<"Inititalization of __TwButton completed\n";
    return 1;
}

//__TwBar***************************************************************************************************************************************

int __TwBar::Init()
{
    std::cout<<"Initilizing __TwBar...\n";
    bar = TwNewBar(name);
    for (unsigned int i=0; i<variables.size(); i++)
    {
        variables[i]->Init();
    }
    std::cout<<"Initialization of __TwBar completed\n";
    return 1;
}

int __TwBar::AddVariable(__TwObject* var)
{
    variables.push_back(var);
    return 1;
}

int __TwBar::DeleteVariable(__TwObject* var)
{
    for(unsigned int i=0; i<variables.size(); i++)
    {
        if(var==variables[i])
        {
            delete variables[i];
            break;
        }
    }
    return 1;
}

__TwBar::~__TwBar()
{
    TwDeleteBar(bar);
}

//__AntTweakBarWrapper**********************************************************************************************************************************

__AntTweakBarWrapper::__AntTweakBarWrapper()
{
    initialized = false;
    Init();
}

int __AntTweakBarWrapper::Init()
{
    std::cout<<"Initializing __AntTweakBarWrapper...\n";
    TwInit(TW_OPENGL_CORE, NULL);
    initialized = true;
    for(unsigned int i=0; i<bars.size(); i++)
    {
        bars[i]->Init();
    }
    std::cout<<"Initialization of __AntTweakBarWrapper completed\n";
    return 1;
}

int __AntTweakBarWrapper::AddBar(__TwBar* bar)
{
    bars.push_back(bar);
    return 1;
}

int __AntTweakBarWrapper::Render()
{
    if(initialized)
    {
        TwDraw();
        return 1;
    }
    else
        return 0;
}

int __AntTweakBarWrapper::MouseMotion(int x, int y)
{
    if(initialized)
        return TwMouseMotion(x, y);
    else
        return 0;
}

int __AntTweakBarWrapper::MouseButton(TwMouseAction action, TwMouseButtonID button)
{
    if(initialized)
        return TwMouseButton(action, button);
    else
        return 0;
}

int __AntTweakBarWrapper::SizeChange(int m_width, int m_height)
{
    width = m_width;
    height = m_height;
    if(initialized)
        TwWindowSize(m_width, m_height);
    return 1;
}

int __AntTweakBarWrapper::PreContextSwitch()
{
    initialized = false;
    TwTerminate();
    return 1;
}

int __AntTweakBarWrapper::PostContextSwitch()
{
    if(!initialized)
    {
        Init();
    }
    return 1;
}

void TW_CALL __AntTweakBarWrapper::Callback(void *clientData)
{
    std::cout<<"Toggling fullscreen...\n"<<"width = "<<width<<"\nheight = "<<height<<"\nsamples = "<<samples<<std::endl;
    if(ToggleFullscreenFunc)
        (*ToggleFullscreenFunc)(samples, width, height);
}

int __AntTweakBarWrapper::CreateSystemSettingsBar(void (*m_ToggleFullscreenFunc)(unsigned int, int, int))
{
    ToggleFullscreenFunc = m_ToggleFullscreenFunc;
    __TwBar* bar = new __TwBar("System Settings");
    __TwVariable* v_width = new __TwVariable(bar, "width", TW_TYPE_INT32, &width, " ");
    __TwVariable* v_height = new __TwVariable(bar, "height", TW_TYPE_INT32, &height, " ");
    __TwVariable* v_samples = new __TwVariable(bar, "samples", TW_TYPE_UINT32, &samples, " ");
    __TwButton* button = new __TwButton(bar, "Toggle fullscreen", Callback, NULL, " ");
    bar->AddVariable(v_width);
    bar->AddVariable(v_height);
    bar->AddVariable(v_samples);
    bar->AddVariable(button);
    AddBar(bar);
    return 1;
}

__AntTweakBarWrapper::~__AntTweakBarWrapper()
{
    for(unsigned int i=0; i<bars.size(); i++)
    {
        delete bars[0];
        bars.erase(bars.begin());
    }
    TwTerminate();

}

void (*__AntTweakBarWrapper::ToggleFullscreenFunc)(unsigned int, int, int) = NULL;
unsigned int __AntTweakBarWrapper::samples = 1;
int __AntTweakBarWrapper::width = 848;
int __AntTweakBarWrapper::height = 480;
