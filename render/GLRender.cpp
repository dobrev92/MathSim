#include "GLRender.h"

__GLBuffer::__GLBuffer(bool m_static)
{
    dataPtr = 0;
    buffer = 0;
    IsStatic = m_static;
    Init();
}

int __GLBuffer::Init()
{
    glGenBuffers(1, &buffer);
    return 1;
}

int __GLBuffer::ChangeType(__BufferType _type)
{
    type = _type;
    return 1;
}

GLenum __GLBuffer::GetGLTarget()
{
    switch (type)
    {
        case __VertexBuffer:
            return GL_ARRAY_BUFFER;
            break;
        case __ElementBuffer:
            return GL_ELEMENT_ARRAY_BUFFER;
            break;
    }
}

GLenum __GLBuffer::GetGLUsage()
{
    if(IsStatic)
        return GL_STATIC_DRAW;
    else
        return GL_DYNAMIC_DRAW;
}
    

int __GLBuffer::SetBufferData(__BufferType _type, unsigned int _size, void* data)
{
    std::cout<<"function SetBufferData(__BufferType _type, unsigned int _size, void* data)\n";
    size = _size;
    type = _type;

    std::cout<<"attempting glBindBuffer(GetGLTarget(), buffer) and glBufferData(GetGLTarget(), size, data, GetGLUsage())...";
    glBindBuffer(GetGLTarget(), buffer);
    glBufferData(GetGLTarget(), size, data, GetGLUsage());
    std::cout<<"completed\n";

    return 1;
}

int __GLBuffer::GetBufferData(void* data)
{
    glBindBuffer(GetGLTarget(), buffer);
    glGetBufferSubData(GetGLTarget(), 0, size, data);
    return 1;
}

int __GLBuffer::BindBuffer()
{
    glBindBuffer(GetGLTarget(), buffer);
    return 1;
}

int __GLBuffer::PreContextSwitch()
{
    std::cout<<"__GLBuffer::PreContextSwitch(), ";
    if(buffer)
    {
        std::cout<<"OpenGL buffer initialized, allocating RAM...";
        dataPtr = malloc(size);
        std::cout<<"completed\n"<<"attempting GetBufferData(dataPtr)...";
        GetBufferData(dataPtr);
        std::cout<<"completed\n"<<"glDeleteBuffers(1, &buffer)...";
        glDeleteBuffers(1, &buffer);
        buffer = 0;
        std::cout<<"completed\n";
    }
    else
        std::cout<<"OpenGL buffer not initialized, nothing to do\n";
    std::cout<<"__GLBuffer::PreContextSwitch() completed\n";
    return 1;
}

int __GLBuffer::PostContextSwitch()
{
    std::cout<<"__GLBuffer::PostContextSwitch(), ";
    if(dataPtr)
    {
        std::cout<<"buffer detected in RAM, proceding with glGenBuffers(1, &buffer)...";
        glGenBuffers(1, &buffer);
        std::cout<<"completed\n";
        std::cout<<"attempting glBindBuffer(GetGLTarget(), buffer) and glBufferData(GetGLTarget(), size, data, GetGLUsage())...";
        glBindBuffer(GetGLTarget(), buffer);
        glBufferData(GetGLTarget(), size, dataPtr, GetGLUsage());
        std::cout<<"completed\n"<<"Deleting buffer from RAM...";
        free(dataPtr);
        dataPtr = 0;
        std::cout<<"completed\n";
    }
    else
        std::cout<<"buffer not detected in RAM, nothing to do\n";
    return 1;
}

__GLBuffer::~__GLBuffer()
{
    if(buffer)
    {
        std::cout<<"OpenGL buffer initialized, proceding with glDeleteBuffers(1, &buffer)...";
        glDeleteBuffers(1, &buffer);
        buffer = 0;
        std::cout<<"completed\n";
    }
    else
        std::cout<<"OpenGL buffer not initialized, nothing to do\n";
    std::cout<<"__GLBuffer destructor completed, __GLBuffer object destroyed\n";
}

//__GLProgram********************************************************************************************************************************

int __GLProgram::InitVertexShaderFromFile(const char* vertex_file_path)
{
    VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = ""; 
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else
    {
        printf("Can't open %s. No such file or directory\n", vertex_file_path);
        //getchar();
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    return 1;
}

int __GLProgram::InitFragmentShaderFromFile(const char* fragment_file_path)
{
    FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else
    {
        printf("Can't open %s. No such file or directory\n", fragment_file_path);
        //getchar();
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    return 1;
}


int __GLProgram::LinkProgram()
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    printf("Linking program\n");
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    return 1;
}

__GLProgram::~__GLProgram()
{
    if(ProgramID)
        glDeleteProgram(ProgramID);
    std::cout<<"__GLProgram destructor completed, __GLProgram Object destroyed\n";
}

//__BasicProgram*************************************************************************************************************************
__GLBasicProgram::__GLBasicProgram()
{
    
    if(InitVertexShaderFromFile("shaders/SimpleTransform.vertexshader"))
        std::cout<<"Vertex Shader Compiled\n";  
    if(InitFragmentShaderFromFile("shaders/SingleColor.fragmentshader"))
        std::cout<<"Fragment Shader Compiled\n";
    if(LinkProgram())
        std::cout<<"Program Linked\n";
    
    //ProgramID = LoadShaders( "SimpleTransform.vertexshader", "SingleColor.fragmentshader" );
    WorldMatrixLocation = glGetUniformLocation(ProgramID, "World");
    ViewProjMatrixLocation = glGetUniformLocation(ProgramID, "ViewProj");

    std::cout<<"__BasicProgram created\n";
    glUseProgram(ProgramID);
}

int __GLBasicProgram::UseProgram()
{
    glUseProgram(ProgramID);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
         0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
         3,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,                  // stride
         (void*)0            // array buffer offset
    );
    
    return 1;
}


int __GLBasicProgram::SetWorldMatrix(__Matrix4x4 mat)
{
    glUniformMatrix4fv(WorldMatrixLocation, 1, GL_TRUE, mat.GetPointer());
    return 1;
}

int __GLBasicProgram::SetViewProjMatrix(__Matrix4x4 mat)
{
    glUniformMatrix4fv(ViewProjMatrixLocation, 1, GL_TRUE, mat.GetPointer());
    return 1;
}
//GLProjectionProgram*************************************************************************************************************************
__GLProjectionProgram::__GLProjectionProgram()
{
    Init();
}

int __GLProjectionProgram::Init()
{
    if(InitVertexShaderFromFile("shaders/ProjectionTransform.vertexshader"))
        std::cout<<"Vertex Shader Compiled\n";  
    if(InitFragmentShaderFromFile("shaders/SingleColor.fragmentshader"))
        std::cout<<"Fragment Shader Compiled\n";
    if(LinkProgram())
        std::cout<<"Program Linked\n";
    
    //ProgramID = LoadShaders( "SimpleTransform.vertexshader", "SingleColor.fragmentshader" );
    WorldMatrixLocation = glGetUniformLocation(ProgramID, "World");
    ViewProjMatrixLocation = glGetUniformLocation(ProgramID, "ViewProj");
    trans4Location = glGetUniformLocation(ProgramID, "trans4");
    Rotation4Location = glGetUniformLocation(ProgramID, "Rotation4");

    std::cout<<"__ProjectionProgram created\n";
    glUseProgram(ProgramID);
    
    return 1;
}

int __GLProjectionProgram::UseProgram()
{
    glUseProgram(ProgramID);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
         0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
         3,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,                  // stride
         (void*)0            // array buffer offset
    );
    
    return 1;
}



int __GLProjectionProgram::SetWorldMatrix(__Matrix4x4 mat)
{
    glUniformMatrix4fv(WorldMatrixLocation, 1, GL_TRUE, mat.GetPointer());
    return 1;
}

int __GLProjectionProgram::SetViewProjMatrix(__Matrix4x4 mat)
{
    glUniformMatrix4fv(ViewProjMatrixLocation, 1, GL_TRUE, mat.GetPointer());
    return 1;
}

int __GLProjectionProgram::SetTranslationVector4(__Vector4 vec)
{
    float vector[4] = {vec.X(), vec.Y(), vec.Z(), vec.W(),};
    glUniform4fv(trans4Location, 1, vector);
    return 1;
}

int __GLProjectionProgram::SetRotationMatrix4(__Matrix4x4 mat)
{
    glUniformMatrix4fv(Rotation4Location, 1, GL_TRUE, mat.GetPointer());
    return 1;
}


//************************************************************************GLRender************************************************************
__GLRender::__GLRender()
{
    Init();
    basicProg = new __GLBasicProgram();
    projProg = new __GLProjectionProgram();
}

int __GLRender::Init()
{
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return 1;
}

int __GLRender::ClearScreen()
{
    glClear( GL_COLOR_BUFFER_BIT );
    return 1;
}

int __GLRender::ViewPort(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    return 1;
}

__Buffer* __GLRender::GetBuffer(__BufferType type, bool m_static)
{
    __Buffer* buff = new __GLBuffer(m_static);
    buff->ChangeType(type);
    buffers.push_back(buff);
    return buff;
}

int __GLRender::DeleteBuffer(__Buffer* buffer)
{
    for(unsigned int i=0; i<buffers.size(); i++)
    {
        if(buffers[i] == buffer)
        {
            delete buffers[i];
            buffers.erase(buffers.begin()+i);
            break;
        }
    }
    return 1;
}

int __GLRender::UseBasicProgram()
{
    basicProg->UseProgram();
    return 1;
}

int __GLRender::SetBasicProgramWorldMatrix(__Matrix4x4 mat)
{
    basicProg->SetWorldMatrix(mat);
    return 1;
}

int __GLRender::SetBasicProgramViewProjMatrix(__Matrix4x4 mat)
{
    basicProg->SetViewProjMatrix(mat);
    return 1;
}

int __GLRender::DrawIndexedLineLoop(int numIndices, const void* indices)
{
    glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_SHORT, indices);
    return 1;
}

int __GLRender::DrawIndexedLines(int numIndices, const void* indices)
{
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_SHORT, indices);
    return 1;
}

int __GLRender::PreContextSwitch()
{
    for(unsigned int i=0; i<buffers.size(); i++)
    {
        buffers[i]->PreContextSwitch();
    }
    delete basicProg;
    delete projProg;
    glDeleteVertexArrays(1, &VertexArrayID);
    return 1;
}

int __GLRender::PostContextSwitch()
{
    Init();
    basicProg = new __GLBasicProgram();
    projProg = new __GLProjectionProgram();
    for(unsigned int i=0; i<buffers.size(); i++)
    {
        buffers[i]->PostContextSwitch();
    }
    return 1;
}

__GLRender::~__GLRender()
{
    for(unsigned int i=0; i<buffers.size(); i++)
    {
        delete buffers[i];
    }
    delete basicProg;
    delete projProg;
    glDeleteVertexArrays(1, &VertexArrayID);

}
