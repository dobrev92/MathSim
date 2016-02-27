#ifndef __GLRender_H_
#define __GLRender_H_

#include "Render.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <iostream>

class __GLBuffer: public __Buffer
{
    protected:
        GLuint buffer;
        GLenum GetGLTarget();
        GLenum GetGLUsage();
    public:
        __GLBuffer(bool m_static);
        int Init();
        int ChangeType(__BufferType _type);
        int SetBufferData(__BufferType _type, unsigned int _size, void* data);
        int GetBufferData(void* data);
        int BindBuffer();
        int PreContextSwitch();
        int PostContextSwitch();
        virtual ~__GLBuffer();
};

//This is the base shader class for the OpenGL render
class __GLProgram: public __Program
{
    protected:
        GLuint VertexShaderID;
        GLuint FragmentShaderID;

        GLuint ProgramID;
        int InitVertexShaderFromFile(const char* vertex_file_path);
        int InitFragmentShaderFromFile(const char* fragment_file_path);
        int LinkProgram();
    public:
        virtual ~__GLProgram();

};

//this shader is concerned with rendering simple wireframes
class __GLBasicProgram: public __GLProgram, public __BasicProgram
{
    protected:
        GLuint WorldMatrixLocation;
        GLuint ViewProjMatrixLocation;
        int Init(){return 1;}
    public:
        __GLBasicProgram();
        int SetWorldMatrix(__Matrix4x4 mat);
        int SetViewProjMatrix(__Matrix4x4 mat);
        int UseProgram();
        int PreContextSwitch(){return 1;}
        int PostContextSwitch(){return 1;}
        virtual ~__GLBasicProgram(){}
};

//This shader renders wireframe 3D projections of 4D geometry
class __GLProjectionProgram: public __GLProgram, public __ProjectionProgram
{
    protected:
        GLuint WorldMatrixLocation;
        GLuint trans4Location;
        GLuint Rotation4Location;
        GLuint ViewProjMatrixLocation; 
        int Init();
    public:
        __GLProjectionProgram();
        int SetWorldMatrix(__Matrix4x4 mat);
        int SetViewProjMatrix(__Matrix4x4 mat);
        int SetTranslationVector4(__Vector4 vec);
        int SetRotationMatrix4(__Matrix4x4 mat);
        int UseProgram();
        int PreContextSwitch(){return 1;}
        int PostContextSwitch(){return 1;}
        virtual ~__GLProjectionProgram(){}
};

class __GLRender: public __Render
{
    protected:
        GLuint VertexArrayID;
        __BasicProgram* basicProg;
        __ProjectionProgram* projProg;
    public:
        __GLRender();
        int Init();
        int ClearScreen();

        __Buffer* GetBuffer(__BufferType type, bool m_static);
        int DeleteBuffer(__Buffer* buffer);
        
        __BasicProgram* GetBasicProgram(){return basicProg;}
        __ProjectionProgram* GetProjectionProgram(){return projProg;}
        int UseBasicProgram();
        int SetBasicProgramWorldMatrix(__Matrix4x4 mat);
        int SetBasicProgramViewProjMatrix(__Matrix4x4 mat);
        
        int DrawIndexedLineLoop(int numIndices, const void* indices);
        int DrawIndexedLines(int numIndices, const void* indices);

        int ViewPort(int x, int y, int width, int height);

        int PreContextSwitch();
        int PostContextSwitch();
        ~__GLRender();
};

#endif

