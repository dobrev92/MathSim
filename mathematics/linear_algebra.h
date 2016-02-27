#ifndef __linear_algebra_H_
#define __linear_algebra_H_

#ifdef __DOUBLE_PRECISION
typedef double __scalar;
#else 
typedef float __scalar;
#endif

#define __PI 3.14159265359

//**********************************************
//**********************VECTORS*****************
//**********************************************


//**********************Vector2*****************

class __Vector2
{
    protected:
        __scalar x, y;

    public:
        __Vector2();
        __Vector2(__scalar _x, __scalar y);

        __scalar X(){return x;}
        __scalar Y(){return y;}

        void SetX(__scalar _x){x = _x;}
        void SetY(__scalar _y){y = _y;}

        virtual __scalar length();

        virtual void normalize();

};

//**********************Vector3*****************
        
class __Vector3 : public __Vector2
{
    protected:
        __scalar z;

    public:
       __Vector3();
       __Vector3(__scalar _x, __scalar _y, __scalar _z);

       __scalar Z(){return z;}

       void SetZ(__scalar _z){z = _z;}
       
       virtual __scalar length();

       virtual void normalize();

};

//**********************Vector4*****************

class __Vector4 : public __Vector3
{
    protected:
        __scalar w;

    public:
        __Vector4();
        __Vector4(__Vector3 vec, __scalar _w);
        __Vector4(__scalar _x, __scalar _y, __scalar _z, __scalar _w);

        __scalar W(){return w;} 

        void SetW(__scalar _w){w = _w;}

        virtual __scalar length();

        virtual void normalize();

};

//**********************Vector5*****************

class __Vector5 : public __Vector4
{
    protected:
        __scalar v;

    public:
        __Vector5();
        __Vector5(__scalar _x, __scalar _y, __scalar _z, __scalar _w, __scalar _v);

        __scalar V(){return v;}

        void SetV(__scalar _v){v = _v;}

        virtual __scalar length();

        virtual void normalize();

};

//**********************************************
//**********************Matrices****************
//**********************************************

//**********************Matrix2x2***************

class __Matrix2x2
{
    private:
        __scalar _11, _12,
                 _21, _22;

    public:
        __Matrix2x2();
};



class __Matrix3x3
{
    private:
        __scalar _11, _12, _13,
                 _21, _22, _23,
                 _31, _32, _33;
    public:
        __Matrix3x3();

};

//**********************************************************Matrix4x4**********************************************************************

class __Matrix4x4 
{
    private:
        __scalar elements[4][4];
    public:
        //Default constructor sets all elements to zero
        __Matrix4x4();
        __Matrix4x4(__Vector4 column1, __Vector4 column2, __Vector4 column3, __Vector4 column4);
        
        //Get matrix element
        __scalar GetElement(int row, int column);
        //Set matrix element
        void SetElement(int row, int column, __scalar value);
        //Get Rows, from 0 to 3 for a 4x4 Matrix
        __Vector4 GetRow(int row);
        //Get Columns, from 0 to 3
        __Vector4 GetColumn(int column);

        //Get pointer to the first element; Useful for passing the matrix to OpenGL shaders
        __scalar* GetPointer();
};

//***********************************************************************Matrix5x5*********************************

class __Matrix5x5
{
    private:
        __scalar elements[5][5];
    public:
        __Matrix5x5();

        __scalar GetElement(int row, int column);
        void SetElement(int row, int column, __scalar value);

        __Vector5 GetRow(int row);
        __Vector5 GetColumn(int column);

        __scalar* GetPointer();
};

//***************************************************************************************************************
//*****************************************OPERATOR OVERLOADING**************************************************
//***************************************************************************************************************

//Vector2
__Vector2 operator+(const __Vector2 & left, const __Vector2 & right);
__Vector2 operator-(const __Vector2 & left, const __Vector2 & right);
__Vector2 operator*(const __Vector2 & left, const __scalar & right);

//Vector3
__Vector3 operator+(const __Vector3 & left, const __Vector3 & right);
__Vector3 operator-(const __Vector3 & left, const __Vector3 & right);
__Vector3 operator*(const __Vector3 & left, const __scalar & right);

//vector4
__Vector4 operator+(const __Vector4 & left, const __Vector4 & right);
__Vector4 operator-(const __Vector4 & left, const __Vector4 & right);
__Vector4 operator*(const __Vector4 & left, const __scalar & right);

//Vector5
__Vector5 operator+(const __Vector5 & left, const __Vector5 & right);
__Vector5 operator-(const __Vector5 & left, const __Vector5 & right);
__Vector5 operator*(const __Vector5 & left, const __scalar & right);

//Matrix4x4
__Matrix4x4 operator*(const __Matrix4x4 & left, const __Matrix4x4 & right);

//Matrix5x5 
__Matrix5x5 operator*(const __Matrix5x5 & left, const __Matrix5x5 & right);



//***************************************************************************************************************
//******************************************FUNCTIONS************************************************************
//***************************************************************************************************************

//Vector arithmetic functions
__Vector2 __AddVector_2D(__Vector2 first, __Vector2 second);
__Vector3 __AddVector_3D(__Vector3 first, __Vector3 second);
__Vector4 __AddVector_4D(__Vector4 first, __Vector4 second);
__Vector5 __AddVector_5D(__Vector5 first, __Vector5 second);

//substract second from first
__Vector2 __SubstractVector_2D(__Vector2 first, __Vector2 second);
__Vector3 __SubstractVector_3D(__Vector3 first, __Vector3 second);
__Vector4 __SubstractVector_4D(__Vector4 first, __Vector4 second);
__Vector5 __SubstractVector_5D(__Vector5 first, __Vector5 second);

//Vector multiplication by a scalar
__Vector2 __VecScalarMultiply_2D(__Vector2 vec, __scalar s);
__Vector3 __VecScalarMultiply_3D(__Vector3 vec, __scalar s);
__Vector4 __VecScalarMultiply_4D(__Vector4 vec, __scalar s);
__Vector5 __VecScalarMultiply_5D(__Vector5 vec, __scalar s);

//Dot products for all dimensions 
__scalar __DotProduct_2D(__Vector2 first, __Vector2 second);
__scalar __DotProduct_3D(__Vector3 first, __Vector3 second);
__scalar __DotProduct_4D(__Vector4 first, __Vector4 second);
__scalar __DotProduct_5D(__Vector5 first, __Vector5 second);

//Perspective division
__Vector4 __Vector5PerspectiveDivide(__Vector5 vec);
__Vector3 __Vector4PerspectiveDivide(__Vector4 vec);

//Reducing dimensionality
__Vector4 __Vector5DropV(__Vector5 vec);
__Vector3 __Vector4DropW(__Vector4 vec);

//cross product
__Vector3 __VectorX_3D(__Vector3 first, __Vector3 second);
//cross product 4D analogue 
__Vector4 __VectorX_4D(__Vector4 first, __Vector4 second, __Vector4 third);

//returns the Identity Matrix in the respective dimensions
__Matrix3x3 __Matrix3x3Identity();
__Matrix4x4 __Matrix4x4Identity();
__Matrix5x5 __Matrix5x5Identity();

//Matrix Transposition
__Matrix4x4 __Matrix4x4Transpose(__Matrix4x4 mat);
__Matrix5x5 __Matrix4x4Transpose(__Matrix5x5 mat);

//returns a scaling matrix in the respective dimensions
__Matrix4x4 __ScaleMatrix4x4(__scalar x, __scalar y, __scalar z);
__Matrix5x5 __ScaleMatrix5x5(__scalar x, __scalar y, __scalar z, __scalar w);

//Translation Matrices
//3D 4x4 Translation matrix
__Matrix4x4 __TranslationMatrix4x4(__scalar x, __scalar y, __scalar z);
//4D 4x4 Translation matrix
__Matrix5x5 __TranslationMatrix5x5(__scalar x, __scalar y, __scalar z, __scalar w);

//3D 4x4 rotation matrices, ARGUMENT IS IN RADIANS!!!
__Matrix4x4 __Matrix4x4RotationX(__scalar value);
__Matrix4x4 __Matrix4x4RotationY(__scalar value);
__Matrix4x4 __Matrix4x4RotationZ(__scalar value);
__Matrix4x4 __Matrix4x4RotationYawPitchRoll(__scalar yaw_angle, __scalar pitch_angle, __scalar roll_angle);

//4D 4x4 single plane rotation matrices, argument in radians
__Matrix4x4 __Matrix4x4RotationXW(__scalar value);
__Matrix4x4 __Matrix4x4RotationYW(__scalar value);
__Matrix4x4 __Matrix4x4RotationZW(__scalar value);

//4D 5x5 single plane rotation matrices
__Matrix5x5 __Matrix5x5RotationXW(__scalar value);

//View Transformation Matrices
__Matrix4x4 __Matrix4x4ViewLookAt(__Vector3 position, __Vector3 lookAt, __Vector3 up);

//Projection Matrices for the respective dimensions
__Matrix4x4 __Matrix4x4PerspectiveFov(__scalar fov, __scalar ar, __scalar z_near, __scalar z_far);
__Matrix5x5 __Matrix5x5PerspectiveFov(__scalar fov);

__Matrix2x2 __Matrix2x2_Multiply(__Matrix2x2 first, __Matrix2x2 second);
__Matrix3x3 __Matrix3x3_Multiply(__Matrix3x3 first, __Matrix3x3 second);
__Matrix4x4 __Matrix4x4_Multiply(__Matrix4x4 first, __Matrix4x4 second);
__Matrix5x5 __Matrix5x5_Multiply(__Matrix5x5 first, __Matrix5x5 second);

//Vector transformations in the respective dimensions
__Vector2 __Vec2Transform(__Vector2 vec, __Matrix2x2 mat);
__Vector3 __Vec3Transform(__Vector3 vec, __Matrix3x3 mat);
__Vector3 __Vec3Transform(__Vector3 vec, __Matrix4x4 mat);
__Vector4 __Vec4Transform(__Vector4 vec, __Matrix4x4 mat);
__Vector5 __Vec5Transform(__Vector5 vec, __Matrix5x5 mat);

#endif
