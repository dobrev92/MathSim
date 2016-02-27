#ifndef __TwBarWrapper_H_
#define __TwBarWrapper_H_

#include "AntTweakBar.h"
#include <vector>
#include <iostream>

class __TwObject
{
    protected:
        char* name;
    public:
        virtual int Init()=0;
        virtual ~__TwObject(){}
};

class __TwBar;

class __TwVariable: public __TwObject
{
    protected:
        __TwBar* bar;
        void* var;
        TwType type;
        char* def;
    public:
        __TwVariable(__TwBar* m_bar, const char* m_name, TwType m_type, void* m_var, const char* m_def);
        int Init();
        virtual ~__TwVariable(){};
};

class __TwButton: public __TwObject
{
    protected:
        __TwBar* bar;
        void* clientData;
        char* def;
        TwButtonCallback callback;
    public:
        __TwButton(__TwBar* m_bar, const char* m_name, TwButtonCallback m_callback, void* m_clientData, const char* m_def);
        int Init();
        virtual ~__TwButton(){};
};


class __TwBar: public __TwObject
{
    protected:
        TwBar* bar;
        std::vector<__TwObject*> variables;
    public:
        __TwBar(const char* m_name);
        int AddVariable(__TwObject* var);
        int DeleteVariable(__TwObject* var);
        TwBar* GetTwBarPtr(){return bar;}
        int Init();
        virtual ~__TwBar();
};

class __AntTweakBarWrapper
{
    protected:
        std::vector<__TwBar*> bars;
        bool initialized;
        static unsigned int samples;
        static int height;
        static int width;
        static void (*ToggleFullscreenFunc)(unsigned int, int, int);
        static void TW_CALL Callback(void *clientData);
    public:
        __AntTweakBarWrapper();
        int Init();
        int CreateSystemSettingsBar(void (*m_ToggleFullscreenFunc)(unsigned int, int, int));
        int Render();
        int MouseMotion(int x, int y);
        int MouseButton(TwMouseAction action, TwMouseButtonID button);
        int SizeChange(int m_width, int m_height);
        int AddBar(__TwBar* bar);
        int PreContextSwitch();
        int PostContextSwitch();
        ~__AntTweakBarWrapper();
};

#endif
