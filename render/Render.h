#ifndef __RENDER_H_
#define __RENDER_H_

#include <stdio.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <string.h>
#include "../mathematics/mathematica.h"


//That will be mostly GL buffers
class __Resource
{
    protected:
        virtual int Init()=0;
    public:
        virtual int PreContextSwitch()=0;
        virtual int PostContextSwitch()=0;
        virtual ~__Resource(){}
        
};

//Buffer type. For the moment vertex buffer or index(element) buffer
enum __BufferType 
{
    __VertexBuffer,
    __ElementBuffer
};

class __Buffer: public __Resource
{
    protected:
        //all resources have unique id number so it's easier to keep track.
        unsigned int id; 
        //size of the buffer. This is needed for reset if the context is changed.
        unsigned int size;
        //Technically the buffer can reside in the RAM memory. I haven't implemented this yet. Nevertheless
        //the pointer will be used to store the data when context switching.
        bool  IsInRam;
        void* dataPtr;
        //if the buffer is static. A static buffer cannot be changed after initialization.
        bool IsStatic;
        __BufferType type;
    public:
        virtual int ChangeType(__BufferType _type)=0;
        virtual int SetBufferData(__BufferType _type, unsigned int _size, void* data)=0;
        virtual int GetBufferData(void* data)=0;
        virtual int BindBuffer()=0;
        virtual ~__Buffer(){};
};

//That would be the rendering pipeline e.g. the vertex shader and the fragment shader
class __Program: public __Resource
{
    public:
        virtual int UseProgram()=0;
        virtual ~__Program(){}
};

class __BasicProgram: public __Program
{
    public:
        virtual int SetWorldMatrix(__Matrix4x4 mat)=0;
        virtual int SetViewProjMatrix(__Matrix4x4 mat)=0;
};

class __ProjectionProgram: public __Program
{
    public:
        virtual int SetViewProjMatrix(__Matrix4x4 mat)=0;
        virtual int SetWorldMatrix(__Matrix4x4 mat)=0;
        virtual int SetTranslationVector4(__Vector4 vec)=0;
        virtual int SetRotationMatrix4(__Matrix4x4 mat)=0;
};


class __Render
{
    protected:
        std::vector<__Buffer*> buffers;

    public:
        virtual int Init()=0;
        virtual int ViewPort(int x, int y, int width, int height)=0;
        virtual int ClearScreen()=0;
        virtual __Buffer* GetBuffer(__BufferType type, bool m_static)=0;
        virtual int DeleteBuffer(__Buffer* buffer)=0;
        virtual __BasicProgram* GetBasicProgram()=0;
        virtual __ProjectionProgram* GetProjectionProgram()=0;
        virtual int UseBasicProgram()=0;
        virtual int SetBasicProgramWorldMatrix(__Matrix4x4 mat)=0;
        virtual int SetBasicProgramViewProjMatrix(__Matrix4x4 mat)=0;
        virtual int DrawIndexedLineLoop(int numIndices, const void* indices)=0;
        virtual int DrawIndexedLines(int numIndices, const void* indices)=0;
        virtual int PreContextSwitch()=0;
        virtual int PostContextSwitch()=0;
};

#endif
