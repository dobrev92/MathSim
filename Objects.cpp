#include "Objects.h"
#include <iostream>
#include "debug.h"

__Object::__Object(__Vector4 m_pos)
{
    position = m_pos;
    Orientation_3D = __Matrix4x4Identity();
    Orientation_4D = __Matrix4x4Identity();
    angleXY = 0;
    angleXZ = 0;
    angleZY = 0;
    angleXW = 0;
    angleYW = 0;
    angleZW = 0;
}

void __Object::RotateXY(__scalar angle)
{
    angleXY += angle;
    if (angleXY > 2*__PI)
        angleXY -= 2*__PI;
    Orientation_3D = __Matrix4x4RotationZ(angle) * Orientation_3D;
}

void __Object::RotateXZ(__scalar angle)
{
    angleXZ += angle;
    if (angleXZ > 2*__PI)
        angleXZ -= 2*__PI;
    Orientation_3D = __Matrix4x4RotationY(angle) * Orientation_3D;
}

void __Object::RotateZY(__scalar angle)
{
    angleZY += angle;
    if (angleZY > 2*__PI)
        angleZY -= 2*__PI;
    Orientation_3D = __Matrix4x4RotationX(angle) * Orientation_3D;
}

void __Object::RotateXW(__scalar angle)
{
    angleXW += angle;
    if(angleXW > 2*__PI)
        angleXW -= 2*__PI;
    Orientation_4D = __Matrix4x4RotationXW(angle) * Orientation_4D;
}

void __Object::RotateYW(__scalar angle)
{
    angleXW += angle;
    if(angleXW > 2*__PI)
        angleXW -= 2*__PI;
    Orientation_4D = __Matrix4x4RotationYW(angle) * Orientation_4D;
}

void __Object::RotateZW(__scalar angle)
{
    angleXW += angle;
    if(angleXW > 2*__PI)
        angleXW -= 2*__PI;
    Orientation_4D = __Matrix4x4RotationZW(angle) * Orientation_4D;
}

void __Object::MoveBasisX(__scalar amount)
{
    __Vector4 XBasis = ComputeTransformMatrix().GetRow(0);
    position = position + XBasis*amount;
}

void __Object::MoveBasisY(__scalar amount)
{
    __Vector4 YBasis = ComputeTransformMatrix().GetRow(1);
    position = position + YBasis*amount;
}

void __Object::MoveBasisZ(__scalar amount)
{
    __Vector4 ZBasis = ComputeTransformMatrix().GetRow(2);
    position = position + ZBasis*amount;
}



//__Camera*******************************************************************************************************
__Camera::__Camera(__Vector4 m_pos) : __Object( m_pos)
{
    moveSpeed = 1;
}
__Matrix4x4 __Camera::ComputeTransformMatrix()
{
    __Matrix4x4 result = Orientation_3D;
    __Matrix4x4 trans = __TranslationMatrix4x4(-position.X(), -position.Y(), -position.Z());
    result = result * trans;
    result = ProjectionMatrix * result;
    return result;
}

__Matrix4x4 __Camera::ComputeTransformNoRot()
{
	__Matrix4x4 result = __TranslationMatrix4x4(-position.X(), -position.Y(), -position.Z());
	result = ProjectionMatrix * result;
	return result;
}

void __Camera::UpdatePos(__scalar amount)
{
    __Vector4 look = ComputeTransformMatrix().GetRow(2);
    __Vector3 look3 = __Vector3(look.X(), look.Y(), look.Z());
    __Vector3 pos = __Vector3(position.X(), position.Y(), position.Z());
    pos = pos + look3*amount;
    position = __Vector4(pos.X(), pos.Y(), pos.Z(), 1);
}

void __Camera::Strafe(__scalar amount)
{
    __Vector3 side = __Vector4DropW(ComputeTransformMatrix().GetRow(0));
    __Vector3 pos = __Vector3(position.X(), position.Y(), position.Z());
    pos = pos + side*amount;
    position = __Vector5(pos.X(), pos.Y(), pos.Z(), 0, 1); 
}


__Geometry::__Geometry()
{
	vertices = 0;
	triIndices = 0;
	wireframeIndices = 0;

	dbg_info("__Geometry instance created\n");
}

__Geometry::~__Geometry()
{
	if(vertices)
		delete[] vertices;

	if(triIndices)
		delete[] triIndices;

	if(wireframeIndices)
		delete[] wireframeIndices;

	dbg_info("__Geometry instance destroyed\n");
}

void __Geometry::Transform(__Matrix4x4 mat)
{
	for(int i = 0; i < numVertices; i++) {
		vertices[i] = __Vec4Transform(vertices[i], mat);
	}
}

void __Geometry::Translate(__Vector4 trans)
{
    for(int i=0; i<numVertices; i++)
    {
        vertices[i] = vertices[i] + trans;
    }
}

//*************************************************************************LINE GEOMETRY**************************************************************
__LineGeometry::__LineGeometry(int _subX)
{
    subX = _subX;
    numVertices = subX+1;
    Init();
    std::cout<<"__LineGeometry instance created\n";
}

int __LineGeometry::Init()
{
    InitVertices();
    InitWireframeIndices();
    return 1;
}

int __LineGeometry::InitVertices()
{
    vertices = new __Vector4[numVertices];

    __scalar stepX = 2/__scalar(subX);

    for(int i=0; i<numVertices; i++)
    {
        vertices[i].SetX(-1+(i)*stepX);
        vertices[i].SetY(0);
        vertices[i].SetZ(0);
        vertices[i].SetW(0);
    }
    return 1;
}

int __LineGeometry::InitTriIndices()
{
    return 1;
}

int __LineGeometry::InitWireframeIndices()
{
    numWireIndices = numVertices;
    wireframeIndices = new unsigned short[numWireIndices];
    for(int i=0; i<numWireIndices; i++)
    {
        wireframeIndices[i] = i;
    }
    return 1;
}

__LineGeometry::~__LineGeometry()
{
    std::cout<<"__LineGeometry instance destroyed\n";
}

//***********************************************************************PLANE GEOMETRY******************************************************
__PlaneGeometry::__PlaneGeometry(int _subX, int _subZ)
{
    subX = _subX;
    subZ = _subZ;

    numVertices = (1 + subX)*(1 + subZ);
    Init();
}

int __PlaneGeometry::Init()
{
    InitVertices();
    InitWireframeIndices();
    return 1;
}

int __PlaneGeometry::InitVertices()
{
    vertices = new __Vector4[numVertices];
    __scalar stepX = 2/__scalar(subX);
    __scalar stepZ = 2/__scalar(subZ);

    for(int i=0; i<(subX+1); i++)
    {
        for(int j=0; j<(subZ+1); j++)
        {
            vertices[i+j*(subX+1)].SetX(-1+(i)*stepX);
            vertices[i+j*(subX+1)].SetY(0);
            vertices[i+j*(subX+1)].SetZ(-1+(j)*stepZ);
            vertices[i+j*(subX+1)].SetW(0);
        }
    }
    return 1;
}

int __PlaneGeometry::InitTriIndices()
{
    return 1;
}

int __PlaneGeometry::InitWireframeIndices()
{
    numWireIndices = 2*subX*(subZ+1) +2*subZ*(subX+1);
    wireframeIndices = new unsigned short[numWireIndices];
    for (int i=0; i<subZ+1; i++)
    {
        for(int j=0; j<subX; j++)
        {
            wireframeIndices[j*2+i*(subX*2)] = j+i*(subX+1);
            wireframeIndices[j*2+i*(subX*2)+1] = j+i*(subX+1)+1;
        }
    }
    for(int i=0; i<subX+1; i++)
    {
        for (int j=0; j<subZ; j++)
        {
            wireframeIndices[j*2+i*(subZ*2)+subX*2*(subZ+1)] = j*(subX+1)+i;
            wireframeIndices[j*2+i*(subZ*2)+subX*2*(subZ+1)+1] = (j+1)*(subX+1)+i;
        }   
    }
    return 1;
}

//__CubeGeometry************************************************************************************************************************
__CubeGeometry::__CubeGeometry(int m_subX, int m_subY, int m_subZ)
{
    subX = m_subX;
    subY = m_subY;
    subZ = m_subZ;
    Init();
    std::cout<<"__CubeGeometry instance destroyed\n";
}

int __CubeGeometry::Init()
{
    //number of vertices for all the 6 faces
    numVertices = 2*(1 + subX)*(1 + subZ) + 2*(1 + subX)*(1 + subY) + 2*(1 + subZ)*(1 + subY);

    //number of indices for all the 6 faces
    numWireIndices = 4*subX*(subZ+1) +4*subZ*(subX+1) + 4*subX*(subY+1) +4*subY*(subX+1) + 4*subZ*(subY+1) +4*subY*(subZ+1);

    //temporary for the first two faces - top and bottom
    __PlaneGeometry* face = new __PlaneGeometry(subX, subZ);

    int vertOffset = 0;
    int WireIndicesOffset = 0;

    face->Translate(__Vector4(0,1,0,0));

    vertices = new __Vector4[numVertices];
    wireframeIndices = new unsigned short[numWireIndices];

    //top face, write vertices
    for(int i=0; i<face->GetNumVertices(); i++)
    {
        vertices[vertOffset + i] = face->GetVerticesPtr()[i];
    }
    //top face, write indices
    for(int i=0; i<face->GetNumWireIndices(); i++)
    {
        wireframeIndices[WireIndicesOffset + i] = face->GetWireframeIndicesPtr()[i];
    }
    
    //save offset
    vertOffset += face->GetNumVertices();
    WireIndicesOffset += face->GetNumWireIndices();

    //this now becomes bottom face
    face->Translate(__Vector4(0,-2,0,0));

    //bottom face, write vertices
    for(int i=0; i<face->GetNumVertices(); i++)
    {   
        vertices[vertOffset + i] = face->GetVerticesPtr()[i];
    }   
    //bottom face, write indices
    for(int i=0; i<face->GetNumWireIndices(); i++)
    {   
        wireframeIndices[WireIndicesOffset + i] = face->GetWireframeIndicesPtr()[i] + vertOffset;    
    }
    
    //save offset
    vertOffset += face->GetNumVertices();
    WireIndicesOffset += face->GetNumWireIndices();
    
    //we don't need that anymore
    delete face;

    //front face
    face = new __PlaneGeometry(subX, subY);
    face->Transform(__Matrix4x4RotationX(__PI/2));
    face->Translate(__Vector4(0,0,1,0));
    
    //front face, write vertices
    for(int i=0; i<face->GetNumVertices(); i++)
    {
        vertices[vertOffset + i] = face->GetVerticesPtr()[i];
    }
    //front face, write indices
    for(int i=0; i<face->GetNumWireIndices(); i++)
    {
        wireframeIndices[WireIndicesOffset + i] = face->GetWireframeIndicesPtr()[i] + vertOffset;
    }
    
    //save offset
    vertOffset += face->GetNumVertices();
    WireIndicesOffset += face->GetNumWireIndices();

    //back face
    face->Translate(__Vector4(0,0,-2,0));
    
    //back face, write vertices
    for(int i=0; i<face->GetNumVertices(); i++)
    {
        vertices[vertOffset + i] = face->GetVerticesPtr()[i];
    }   
    //back fave, write indices
    for(int i=0; i<face->GetNumWireIndices(); i++)
    {   
        wireframeIndices[WireIndicesOffset + i] = face->GetWireframeIndicesPtr()[i] + vertOffset;
    }

    //save offset
    vertOffset += face->GetNumVertices();
    WireIndicesOffset += face->GetNumWireIndices();

    //deallocate
    delete face;

    //right face
    face = new __PlaneGeometry(subY, subZ);
    face->Transform(__Matrix4x4RotationZ(__PI/2));
    face->Translate(__Vector4(1,0,0,0));

    //right face, write vertices
    for(int i=0; i<face->GetNumVertices(); i++)
    {   
        vertices[vertOffset + i] = face->GetVerticesPtr()[i];
    }   
    //right face, write indices
    for(int i=0; i<face->GetNumWireIndices(); i++)
    {   
        wireframeIndices[WireIndicesOffset + i] = face->GetWireframeIndicesPtr()[i] + vertOffset;
    }   
    
    //save offset
    vertOffset += face->GetNumVertices();
    WireIndicesOffset += face->GetNumWireIndices();

    //left face
    face->Translate(__Vector4(-2,0,0,0));
    
    //left face, write vertices
    for(int i=0; i<face->GetNumVertices(); i++)
    {   
        vertices[vertOffset + i] = face->GetVerticesPtr()[i];
    }   
    //left face, write indices
    for(int i=0; i<face->GetNumWireIndices(); i++)
    {   
        wireframeIndices[WireIndicesOffset + i] = face->GetWireframeIndicesPtr()[i] + vertOffset;
    }

    //delete temporary face object
    delete face;
    return 1;
}

//__SphereGeometry********************************************************************************************************
__SphereGeometry::__SphereGeometry(float m_radius, int m_subTheta, int m_subPhi)
{
    radius = m_radius;
    subTheta = m_subTheta;
    subPhi = m_subPhi;
    InitVertices();
    InitWireframeIndices();
}

int __SphereGeometry::InitVertices()
{
    numVertices = (subTheta-1)*subPhi + 2;
    vertices = new __Vector4[numVertices];

    vertices[0] = __Vector4(0,radius,0,0);


    __scalar thetaIncrement = (__scalar)__PI/(subTheta);
    __scalar phiIncrement = (__scalar)2*__PI/subPhi;
    
    for(int i=0; i<(subTheta-1); i++)
    {
        for(int j=0; j<subPhi; j++)
        {
            vertices[1+j+i*(subPhi)].SetX(radius*sin((i+1)*thetaIncrement)*sin(j*phiIncrement));
            vertices[1+j+i*(subPhi)].SetY(radius*cos((i+1)*thetaIncrement));
            vertices[1+j+i*(subPhi)].SetZ(radius*sin((i+1)*thetaIncrement)*cos(j*phiIncrement));
            vertices[1+j+i*(subPhi)].SetW(0);
        }
    }

    vertices[numVertices-1] = __Vector4(0,-radius,0,0);

    return 1;
}

int __SphereGeometry::InitWireframeIndices()
{
    numWireIndices = 2*(subTheta-1)*subPhi + 2*(subTheta-2)*subPhi + 4*subPhi;
    wireframeIndices = new unsigned short[numWireIndices];

    for(int i=0; i<(subTheta-1); i++)
    {   
        for(int j=0; j<subPhi; j++)
        {
            wireframeIndices[j*2+i*(subPhi*2)]=1 + j+i*subPhi;
            if(!(j==(subPhi-1)))
                wireframeIndices[j*2+i*(subPhi*2)+1]=1 + j+i*subPhi +1;
            else
                wireframeIndices[j*2+i*(subPhi*2)+1]=1 + j+i*subPhi-subPhi+1;
        }
    }
    int offset = 2*(subTheta-1)*subPhi;
    int offset2 = 2*(subTheta-2)*subPhi;

    for(int i=0; i<subPhi; i++)
    {
        for(int j=0; j<(subTheta-2); j++)
        {
            wireframeIndices[offset+j*2+i*(subTheta-2)*2]=1+j*(subPhi)+i;
            wireframeIndices[offset+j*2+i*(subTheta-2)*2+1]=1+(j+1)*(subPhi)+i;
        }
    }

    for(int i=0; i<subPhi; i++)
    {
        wireframeIndices[offset+offset2+4*i]=0;
        wireframeIndices[offset+offset2+4*i+1]=1+i;
        wireframeIndices[offset+offset2+4*i+2]=numVertices-1-(i+1);
        wireframeIndices[offset+offset2+4*i+3]=numVertices-1;
    }

    return 1;
}

//__QuadGeometry******************************************************************************
__QuadGeometry::__QuadGeometry()
{
	InitVertices();
	InitTriIndices();
}

int __QuadGeometry::InitVertices()
{
	numVertices = 4;
	vertices = new __Vector4[numVertices];

	vertices[0] = __Vector4(-1.0f, 1.0f, 0.0f, 0.0f);
	vertices[1] = __Vector4(1.0f, 1.0f, 0.0f, 0.0f);
	vertices[2] = __Vector4(-1.0f, -1.0f, 0.0f, 0.0f);
	vertices[3] = __Vector4(1.0f, -1.0f, 0.0f, 0.0f);

	return 1;
}

int __QuadGeometry::InitTriIndices()
{
	numWireIndices = 6;
	wireframeIndices = new unsigned short[numWireIndices];

	wireframeIndices[0] = 0;
	wireframeIndices[1] = 1;
	wireframeIndices[2] = 2;
	wireframeIndices[3] = 2;
	wireframeIndices[4] = 1;
	wireframeIndices[5] = 3;

	return 1;
}

//__GeometryObject*********************************************************************************************************
__GeometryObject::__GeometryObject(__Render* m_render, __Vector4 _pos): __Object(_pos)
{
	render = m_render;
	scale = __Vector4(1,1,1,1);
	LineWidth = 1;
	geom = 0;
	vertexbuffer = 0;
	TriElementBuffer = 0;
	WireElementBuffer = 0;
	dbg_info("__GeometryObject instance created\n");
}

int __GeometryObject::InitVertexBuffer()
{
    int numVerts = geom->GetNumVertices();
    float* gl_vertices = new float[numVerts*3];
    __Vector4* vertices = geom->GetVerticesPtr();

    for(int i=0; i<numVerts; i++)
    {
        gl_vertices[(i*3)-1+1] = vertices[i].X();
        gl_vertices[(i*3)-1+2] = vertices[i].Y();
        gl_vertices[(i*3)-1+3] = vertices[i].Z();
    }

    vertexbuffer = render->GetBuffer(__VertexBuffer, true);
    vertexbuffer->BindBuffer();
    vertexbuffer->SetBufferData(__VertexBuffer, numVerts*3*sizeof(float), gl_vertices);
    delete[] gl_vertices;
    return 1;
}

int __GeometryObject::InitWireIndexBuffer()
{
    int NumWireIndices = geom->GetNumWireIndices();

    WireElementBuffer = render->GetBuffer(__ElementBuffer, true);
    WireElementBuffer->BindBuffer();
    WireElementBuffer->SetBufferData(__ElementBuffer, NumWireIndices*sizeof(unsigned short), geom->GetWireframeIndicesPtr());
    std::cout<<"Element Buffer Initialized\n";
    return 1;
}

int __GeometryObject::Init()
{
    InitVertexBuffer();
    InitWireIndexBuffer();
    return 1;
}

int __GeometryObject::Render(__Camera* camera, bool wireframe)
{

    vertexbuffer->BindBuffer();
    WireElementBuffer->BindBuffer();
    __Matrix4x4 trans = __TranslationMatrix4x4(position.X(),position.Y(),position.Z()) * ComputeTransformMatrix() * __ScaleMatrix4x4(scale.X(), scale.Y(), scale.Z());
    __BasicProgram* prog = render->GetBasicProgram();
    prog->UseProgram();
    prog->SetWorldMatrix(trans);
    __Matrix4x4 mat = camera->ComputeTransformMatrix();
    prog->SetViewProjMatrix(mat);
    glLineWidth(LineWidth);
    glPointSize(10);
    render->DrawIndexedLines(geom->GetNumWireIndices(), 0);
    glDisableVertexAttribArray(0);
    return 1;
}



__Matrix4x4 __GeometryObject::ComputeTransformMatrix()
{
	return Orientation_3D;
}

__GeometryObject::~__GeometryObject()
{
	if (vertexbuffer) {
		render->DeleteBuffer(vertexbuffer);
		vertexbuffer = 0;
		dbg_info("vertexbuffer deleted\n");
	}

	if (TriElementBuffer) {
		render->DeleteBuffer(TriElementBuffer); 
		TriElementBuffer = 0;
		dbg_info("TriElementBuffer deleted\n");
	}

	if (WireElementBuffer) {
		render->DeleteBuffer(WireElementBuffer);
		WireElementBuffer = 0;
		dbg_info("WireElementBuffer deleted\n");
	}

	if (geom)
		delete geom;

	dbg_info("__GeometryObject instance destroyed\n");
}
//*******************************************************************Line Class************************************************************

__Line::__Line(__Render* m_render, __Vector4 _pos, int subX): __GeometryObject(m_render, _pos)
{
    geom = new __LineGeometry(subX);
    Init();
    std::cout<<"__Line instance created\n";
}

int __Line::Render(__Camera* camera, bool wireframe)
{
    vertexbuffer->BindBuffer();
    WireElementBuffer->BindBuffer();
    render->UseBasicProgram();
    __Matrix4x4 trans = __TranslationMatrix4x4(position.X(),position.Y(),position.Z()) * ComputeTransformMatrix() * __ScaleMatrix4x4(scale.X(), scale.Y(), scale.Z());
    render->SetBasicProgramWorldMatrix(trans);
    __Matrix4x4 mat = camera->ComputeTransformMatrix();
    render->SetBasicProgramViewProjMatrix(mat);
    glLineWidth(LineWidth);
    glPointSize(5);
    render-> DrawIndexedLineLoop(geom->GetNumWireIndices(), 0);
    glDisableVertexAttribArray(0);
    return 1;
}


__Line::~__Line()
{
    std::cout<<"__Line instance destroyed\n";
}

//******************************************************************__PLANE*******************************************************
__Plane::__Plane(__Render* m_render, __Vector4 _pos, int subX, int subZ): __GeometryObject(m_render, _pos)
{
    geom = new __PlaneGeometry(subX, subZ);
    Init();
    std::cout<<"__Plane instance created\n";
}

__Plane::~__Plane()
{
    std::cout<<"__Plane instance destroyed\n";
}

//__Cube***************************************************************************************************************************
__Cube::__Cube(__Render* m_render, __Vector4 _pos, int subX, int subY, int subZ): __GeometryObject(m_render, _pos)
{
    geom = new __CubeGeometry(subX, subY, subZ);
    Init();
    std::cout<<"__Cube instance created\n";
}

__Cube::~__Cube()
{
    std::cout<<"__Cube innstance destroyed\n";
}

//__Sphere**********************************************************************************************************************
__Sphere::__Sphere(__Render* m_render, __Vector4 _pos, float m_radius, int m_subTheta, int m_subPhi):__GeometryObject(m_render, _pos)
{
     geom = new __SphereGeometry(m_radius, m_subTheta, m_subPhi);
    Init();
    std::cout<<"__Sphere instance created\n";
}

__Sphere::~__Sphere()
{
    std::cout<<"__Sphere innstance destroyed\n";
}

//__HypercubeProjection********************************************************************************************************

__HypercubeFaceProjection::__HypercubeFaceProjection(__Render* m_render, __Vector4 m_pos):__GeometryObject(m_render, m_pos)
{
    angle = 0;
    geom = new __CubeGeometry(1, 1, 1);
    Init();
    std::cout<<"__HypercubeFaceProjection instance created\n";
}

int __HypercubeFaceProjection::Render(__Camera* camera, bool wireframe)
{
    vertexbuffer->BindBuffer();
    WireElementBuffer->BindBuffer();
    //render->UseBasicProgram();
    __Matrix4x4 trans = __TranslationMatrix4x4(position.X(),position.Y(),position.Z()) * ComputeTransformMatrix() * __ScaleMatrix4x4(10, 10, 10);
    __ProjectionProgram* prog = render->GetProjectionProgram();
    prog->UseProgram();
    prog->SetTranslationVector4(__Vector4(0,0,0,10));
    prog->SetRotationMatrix4(Orientation_4D);
    prog->SetWorldMatrix(trans);
    //render->SetBasicProgramWorldMatrix(trans);
    __Matrix4x4 mat = camera->ComputeTransformMatrix();
    prog->SetViewProjMatrix(mat);
    //render->SetBasicProgramViewProjMatrix(mat);
    glLineWidth(LineWidth);
    glPointSize(10);
    //glDrawArrays(GL_POINTS, 0, geom->GetNumVertices());
    render->DrawIndexedLines(geom->GetNumWireIndices(), 0);
    glDisableVertexAttribArray(0);
    return 1;
}

__HypercubeFaceProjection::~__HypercubeFaceProjection()
{
    std::cout<<"__HypercubeFaceProjection instance destroyed\n";
}

//__ParticleSystem***************************************************************************
__ParticleSystem::__ParticleSystem(__Render* m_render, __Vector4 m_pos):__GeometryObject(m_render, m_pos)
{
	maxParticles = 3;
	particles = new __Particle[maxParticles];
	geom = new __QuadGeometry();
	Init();
	InitPositionBuffer();
	dbg_info("__ParticleSystem instance created\n");
}

int __ParticleSystem::InitPositionBuffer(void)
{
	float pos[] = { -5.0f, 10.0f, 0.0f,
			0.0f, 10.0f, 0.0f,
			5.0f, 10.0f, 0.0f, };
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(float), pos, GL_STREAM_DRAW);	
	return 1;
}

int __ParticleSystem::Render(__Camera* camera, bool wireframe)
{
	vertexbuffer->BindBuffer();
	WireElementBuffer->BindBuffer();
	__ParticleProgram* prog = render->GetParticleProgram();
	prog->UseProgram();

	__Matrix4x4 mat = camera->ComputeTransformMatrix();
	prog->setProjMatrix(mat);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(1,	// attribute.
			3,		// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,		// stride
			(void*)0	// array buffer offset
			);
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 3);
	glVertexAttribDivisor(1,0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	return 1;
}

__ParticleSystem::~__ParticleSystem()
{
	delete[] particles;

	if (positionBuffer) {
		glDeleteBuffers(1, &positionBuffer);
		positionBuffer = 0;
		dbg_info("positionBuffer deleted\n");
	}
	dbg_info("__ParticleSystem instance destroyed\n");
}

