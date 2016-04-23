#ifndef __OBJECTS_H_
#define __OBJECTS_H_

#include "mathematics/mathematica.h"
#include <GL/glew.h>
#include "shader.hpp"
#include "render/Render.h"
#include <math.h>


class __Object 
{
    protected:
        //all objects are defined by a position in a 4 dimensional space
        __Vector4 position;
        __Matrix4x4 Orientation_3D;
        __Matrix4x4 Orientation_4D;

        //Here are angles that define orientation in 4 dimensional space
        //First three angles define orientation in 3 dimensional space
        __scalar angleXY;
        __scalar angleXZ;
        __scalar angleZY;
        //The second three angles define orientation in 4 dimensional space
        __scalar angleXW;
        __scalar angleYW;
        __scalar angleZW;
    public:
        __Object(__Vector4 m_pos);

        __Vector4 Getpos(){return position;}

        __scalar GetAngleXY(){return angleXY;}
        __scalar GetAngleXZ(){return angleXZ;}
        __scalar GetAngleZY(){return angleZY;}

        __scalar GetAngleXW(){return angleXW;}
        __scalar GetAngleYW(){return angleYW;}
        __scalar GetAngleZW(){return angleZW;}

        void RotateXY(__scalar angle);
        void RotateXZ(__scalar angle);
        void RotateZY(__scalar angle);

        void RotateXW(__scalar angle);
        void RotateYW(__scalar angle);
        void RotateZW(__scalar angle);

        //functions to move across basis vectors
        void MoveBasisX(__scalar amount);
        void MoveBasisY(__scalar amount);
        void MoveBasisZ(__scalar amount);

        //function to compute the transform Matrix. Will be different across 
        //different objects
        virtual __Matrix4x4 ComputeTransformMatrix()=0;
};

//Camera Object
class __Camera: public __Object
{
    private:
        //speed of the camera in unit/sec. Default is 1
        __scalar moveSpeed;
        __Matrix4x4 ProjectionMatrix;
    public:
        __Camera(__Vector4 _pos);
        void UpdatePos(__scalar amount);
        void Strafe(__scalar amount);
        void SetSpeed(__scalar _speed){moveSpeed = _speed;}
        void SetProjectionMatrix(__Matrix4x4 proj){ProjectionMatrix = proj;}
        __Matrix4x4 ComputeTransformMatrix();
};


//Base class for Geometry
class __Geometry
{
    protected:
        //vertices in 4D space
        __Vector4* vertices;
        //indices for triangle mesh
        unsigned short* triIndices;
        //indices for polygon/quad wireframe
        unsigned short* wireframeIndices;

        int numVertices;
        int numTriIndices;
        int numWireIndices;
    public:
        __Geometry();
        virtual int Init()=0;
        virtual int InitVertices(){return 1;}
        virtual int InitTriIndices(){return 1;}
        virtual int InitWireframeIndices(){return 1;}

        __Vector4* GetVerticesPtr(){return vertices;}
        unsigned short* GetTriIndicesPtr(){return triIndices;}
        unsigned short* GetWireframeIndicesPtr(){return wireframeIndices;}
        int GetNumVertices(){return numVertices;}
        int GetNumTriIndices(){return numTriIndices;}
        int GetNumWireIndices(){return numWireIndices;}

        //Transform point with a 4x4 matrix. Translation matrix will not work, since the w coordinate is 0
        void Transform(__Matrix4x4 mat);
        //Translate by a 4Vector
        void Translate(__Vector4 trans);

        virtual ~__Geometry();
};

class __LineGeometry: public __Geometry
{
    protected:
        int subX;
    public:
        __LineGeometry(int _subX);
        int Init();
        int InitVertices();
        int InitTriIndices();
        int InitWireframeIndices();
        virtual ~__LineGeometry();
};


class __PlaneGeometry: public __Geometry
{
    protected:
        int subX, subZ;
    public:
        //the constructor, subX and subZ are the subdivisions 
        __PlaneGeometry(int _subX, int _subZ);
        int Init();
        int InitVertices();
        int InitTriIndices();
        int InitWireframeIndices();

};

class __CubeGeometry: public __Geometry
{
    protected:
        int subX;
        int subY;
        int subZ;
    public:
        __CubeGeometry(int m_subX, int m_subY, int m_subZ);
        int Init();
};

class __SphereGeometry: public __Geometry
{
    protected:
        //the radius of the sphere
        int radius;
        //subdivisions along inclination angle
        int subTheta;
        //subdivisions along the azimuth angle
        int subPhi;
    public:
        __SphereGeometry(float m_radius, int m_subTheta, int m_subPhi);
        int Init(){return 1;}
        int InitVertices();
        int InitWireframeIndices();
};

class __GeometryObject: public __Object
{
    protected:
        __Render* render;
        __Geometry* geom;
        __Buffer* vertexbuffer;
        __Buffer* TriElementBuffer;
        __Buffer* WireElementBuffer;
        //Width of the lines used to render wireframe. Default is 3
        __scalar LineWidth;
        __Vector4 scale;
    public:
        __GeometryObject(__Render* m_render, __Vector4 _pos);
        virtual int InitVertexBuffer();
        virtual int InitWireIndexBuffer();
        virtual int Render(__Camera* camera, bool wireframe);
        virtual int Init();
        __Geometry* GetGeomPtr(){return geom;}

        void SetWireframeLineWidth(__scalar width){LineWidth = width;}
        void SetScale(__Vector4 _scale){scale = _scale;}
        void SetScaleX(__scalar scaleX){scale.SetX(scaleX);}
        void SetScaleY(__scalar scaleY){scale.SetY(scaleY);}
        void SetScaleZ(__scalar scaleZ){scale.SetZ(scaleZ);}
        void SetScaleW(__scalar scaleW){scale.SetW(scaleW);}

        virtual __Matrix4x4 ComputeTransformMatrix();

        virtual ~__GeometryObject();
};

class __Line: public __GeometryObject
{
    public:
        __Line(__Render* m_render, __Vector4 _pos, int subX);
        int Render(__Camera* camera, bool wireframe);
        virtual ~__Line();
};

class __Plane: public __GeometryObject
{
    public:
        __Plane(__Render* m_render, __Vector4 _pos, int subX, int subZ);
        virtual ~__Plane();
};

class __Cube: public __GeometryObject
{
    public:
        __Cube(__Render* m_render, __Vector4 _pos, int subX, int subY, int subZ);
        virtual ~__Cube();
};

class __Sphere: public __GeometryObject
{
    public:
        __Sphere(__Render* m_render, __Vector4 _pos, float m_radius, int m_subTheta, int m_subPhi);
        virtual ~__Sphere();
};

class __HypercubeFaceProjection: public __GeometryObject
{
    public:
        float angle;
        __HypercubeFaceProjection(__Render* m_render, __Vector4 m_pos);
        int Render(__Camera* camera, bool wireframe);
        virtual ~__HypercubeFaceProjection();
};

#endif
