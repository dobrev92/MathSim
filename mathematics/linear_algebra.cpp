#include "linear_algebra.h"
#include <math.h>
#include <iostream>

//*************************************************************************************************************************
//***********************************************************VECTORS*******************************************************
//*************************************************************************************************************************

//***********************************************************Vector2*******************************************************

__Vector2::__Vector2()
{
    x = 0;
    y = 0;
}

__Vector2::__Vector2(__scalar _x, __scalar _y)
{
    x = _x;
    y = _y;
}

__scalar __Vector2::length()
{
    __scalar l;
    l = sqrt(pow(x,2) + pow(y,2));
    return l;
}

void __Vector2::normalize()
{
    __scalar l;
    l = length();
    x = x/l;
    y = y/l;
}

//*************************************************************************Vector3******************************************************

__Vector3::__Vector3()
{
    x = 0;
    y = 0;
    z = 0;
}

__Vector3::__Vector3(__scalar _x, __scalar _y, __scalar _z)
{
    x = _x;
    y = _y;
    z = _z;
}

__scalar __Vector3::length()
{
    __scalar l = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    return l;
}

void __Vector3::normalize()
{
    __scalar l = length();
    x = x/l;
    y = y/l;
    z = z/l;
}


//***********************************************************************Vector4**********************************************************

__Vector4::__Vector4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

__Vector4::__Vector4(__scalar _x, __scalar _y, __scalar _z, __scalar _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

__Vector4::__Vector4(__Vector3 vec, __scalar _w)
{
    x = vec.X();
    y = vec.Y();
    z = vec.Z();
    w = _w;
}

__scalar __Vector4::length()
{
    __scalar l = sqrt(pow(x,2) + pow(y,2) + pow(z,2) + pow(w,2));
    return l;
}

void __Vector4::normalize()
{
    __scalar l = length();
    x = x/l;
    y = y/l;
    z = z/l;
    w = w/l;
}

//***********************************************************************Vector5************************************************************

__Vector5::__Vector5()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
    v = 0;
}

__Vector5::__Vector5(__scalar _x, __scalar _y, __scalar _z, __scalar _w, __scalar _v)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
    v = _v;
}

__scalar __Vector5::length()
{
    __scalar l = sqrt(pow(x,2) + pow(y,2) + pow(z,2) + pow(w,2) + pow(v,2));
    return l;
}

void __Vector5::normalize()
{
    __scalar l = length();
    x = x/l;
    y = y/l;
    z = z/l;
    w = w/l;
    v = v/l;
}

//*******************************************************************************************************************************************
//*************************************************************************MATRICES**********************************************************
//*******************************************************************************************************************************************

//*************************************************************************Matrix4x4*********************************************************

__Matrix4x4::__Matrix4x4()
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            elements[i][j] = 0;
        }
    }
}

__Matrix4x4::__Matrix4x4(__Vector4 column1, __Vector4 column2, __Vector4 column3, __Vector4 column4)
{
    //first column
    elements[0][0] = column1.X();
    elements[0][1] = column1.Y();
    elements[0][2] = column1.Z();
    elements[0][3] = column1.W();

    //second column
    elements[1][0] = column2.X();
    elements[1][1] = column2.Y();
    elements[1][2] = column2.Z();
    elements[1][3] = column2.W();

    //third column
    elements[2][0] = column3.X();
    elements[2][1] = column3.Y();
    elements[2][2] = column3.Z();
    elements[2][3] = column3.W();
    
    //fourth column
    elements[3][0] = column4.X();
    elements[3][1] = column4.Y();
    elements[3][2] = column4.Z();
    elements[3][3] = column4.W();
}

void __Matrix4x4::SetElement(int row, int column, __scalar value)
{
    if((row > 3) || (column > 3))
        return;

    elements[row][column] = value;
}

__scalar __Matrix4x4::GetElement(int row, int column)
{
    if((row>3) || (column>3))
        return 0;

    return elements[row][column];
}

__Vector4 __Matrix4x4::GetRow(int row)
{
    __Vector4 result;
    if(row > 3)
        return result;

    result.SetX(elements[row][0]);
    result.SetY(elements[row][1]);
    result.SetZ(elements[row][2]);
    result.SetW(elements[row][3]);
    return result;
}

__Vector4 __Matrix4x4::GetColumn(int column)
{
    __Vector4 result;
    if(column > 3)
        return result;

    result.SetX(elements[0][column]);
    result.SetY(elements[1][column]);
    result.SetZ(elements[2][column]);
    result.SetW(elements[3][column]);
    return result;
}

__scalar* __Matrix4x4::GetPointer()
{
    return &elements[0][0];
}

//************************************************************Matrix5x5**********************************************************************

__Matrix5x5::__Matrix5x5()
{
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            elements[i][j] = 0;
        }
    }
}

void __Matrix5x5::SetElement(int row, int column, __scalar value)
{
    if((row > 4) || (column > 4))
        return;

    elements[row][column] = value;
}

__scalar __Matrix5x5::GetElement(int row, int column)
{
    if((row>4) || (column>4))
        return 0;
    
    return elements[row][column];
}

__Vector5 __Matrix5x5::GetRow(int row)
{
    __Vector5 result;
    if(row > 4)
        return result;

    result.SetX(elements[row][0]);
    result.SetY(elements[row][1]);
    result.SetZ(elements[row][2]);
    result.SetW(elements[row][3]);
    result.SetV(elements[row][4]);
    return result;
}

__Vector5 __Matrix5x5::GetColumn(int column)
{
    __Vector5 result;
    if(column > 4)
        return result;

    result.SetX(elements[0][column]);
    result.SetY(elements[1][column]);
    result.SetZ(elements[2][column]);
    result.SetW(elements[3][column]);
    result.SetV(elements[4][column]);
    return result;
}

__scalar* __Matrix5x5::GetPointer()
{
    return &elements[0][0];
}


//*******************************************************************************************************************************************
//************************************************************OVERLOADING OPERATORS**********************************************************
//*******************************************************************************************************************************************

//Vector2
//.
//.

//Vector3
__Vector3 operator+(const __Vector3 & left, const __Vector3 & right)
{
    __Vector3 result = __AddVector_3D(left, right);
    return result;
}

__Vector3 operator-(const __Vector3 & left, const __Vector3 & right)
{
    __Vector3 result = __SubstractVector_3D(left, right);
    return result;
}

__Vector3 operator*(const __Vector3 & left, const __scalar & right)
{
    __Vector3 result = __VecScalarMultiply_3D(left, right);
    return result;
}

//Vector4
__Vector4 operator+(const __Vector4 & left, const __Vector4 & right)
{
    __Vector4 result = __AddVector_4D(left, right);
    return result;
}

__Vector4 operator-(const __Vector4 & left, const __Vector4 & right)
{
    __Vector4 result = __SubstractVector_4D(left, right);
    return result;
}

__Vector4 operator*(const __Vector4 & left, const __scalar & right)
{
    __Vector4 result = __VecScalarMultiply_4D(left, right);
    return result;
}

//Vector5
__Vector5 operator+(const __Vector5 & left, const __Vector5 & right)
{
    __Vector5 result = __AddVector_5D(left, right);
    return result;
}

__Vector5 operator-(const __Vector5 & left, const __Vector5 & right)
{
    __Vector5 result = __SubstractVector_5D(left, right);
    return result;
}

__Vector5 operator*(const __Vector5 & left, const __scalar & right)
{
    __Vector5 result = __VecScalarMultiply_5D(left, right);
    return result;
}

//Matrix4x4
__Matrix4x4 operator*(const __Matrix4x4 & left, const __Matrix4x4 & right)
{
    __Matrix4x4 result;
    result = __Matrix4x4_Multiply(left, right);
    return result;
}

//Matrix5x5 
__Matrix5x5 operator*(const __Matrix5x5 & left, const __Matrix5x5 & right)
{
    __Matrix5x5 result;
    result = __Matrix5x5_Multiply(left, right);
    return result;
}

//*******************************************************************************************************************************************
//*************************************************************************FUNCTIONS*********************************************************
//*******************************************************************************************************************************************

//Vector arithmtic functions

//ADDITION
__Vector2 __AddVector_2D(__Vector2 first, __Vector2 second)
{
    __Vector2 result;
    result.SetX(first.X() + second.X());
    result.SetY(first.Y() + second.Y());
    return result;
}

__Vector3 __AddVector_3D(__Vector3 first, __Vector3 second)
{
    __Vector3 result;
    result.SetX(first.X() + second.X());
    result.SetY(first.Y() + second.Y());
    result.SetZ(first.Z() + second.Z());
    return result;
}

__Vector4 __AddVector_4D(__Vector4 first, __Vector4 second)
{
    __Vector4 result;
    result.SetX(first.X() + second.X());
    result.SetY(first.Y() + second.Y());
    result.SetZ(first.Z() + second.Z());
    result.SetW(first.W() + second.W());
    return result;
}

__Vector5 __AddVector_5D(__Vector5 first, __Vector5 second)
{
    __Vector5 result;
    result.SetX(first.X() + second.X());
    result.SetY(first.Y() + second.Y());
    result.SetZ(first.Z() + second.Z());
    result.SetW(first.W() + second.W());
    result.SetV(first.V() + second.V());
    return result;
}

//SUBSTRACTION
__Vector2 __SubstractVector_2D(__Vector2 first, __Vector2 second)
{
    __Vector2 result;
    result.SetX(first.X() - second.X());
    result.SetY(first.Y() - second.Y());
    return result;
}

__Vector3 __SubstractVector_3D(__Vector3 first, __Vector3 second)
{
    __Vector3 result;
    result.SetX(first.X() - second.X());
    result.SetY(first.Y() - second.Y());
    result.SetZ(first.Z() - second.Z());
    return result;
}

__Vector4 __SubstractVector_4D(__Vector4 first, __Vector4 second)
{
    __Vector4 result;
    result.SetX(first.X() - second.X());
    result.SetY(first.Y() - second.Y());
    result.SetZ(first.Z() - second.Z());
    result.SetW(first.W() - second.W());
    return result;
}

__Vector5 __SubstractVector_5D(__Vector5 first, __Vector5 second)
{
    __Vector5 result;
    result.SetX(first.X() - second.X());
    result.SetY(first.Y() - second.Y());
    result.SetZ(first.Z() - second.Z());
    result.SetW(first.W() - second.W());
    result.SetV(first.V() - second.V());
    return result;
}

//VECTOR MULTIPLICATION BY A SCALAR
__Vector2 __VecScalarMultiply_2D(__Vector2 vec, __scalar s)
{
    __Vector2 result;
    result.SetX(vec.X() * s);
    result.SetY(vec.Y() * s);
    return result;
}

__Vector3 __VecScalarMultiply_3D(__Vector3 vec, __scalar s)
{
    __Vector3 result;
    result.SetX(vec.X() * s);
    result.SetY(vec.Y() * s);
    result.SetZ(vec.Z() * s);
    return result;
}

__Vector4 __VecScalarMultiply_4D(__Vector4 vec, __scalar s)
{
    __Vector4 result;
    result.SetX(vec.X() * s);
    result.SetY(vec.Y() * s);
    result.SetZ(vec.Z() * s);
    result.SetW(vec.W() * s);
    return result;
}

__Vector5 __VecScalarMultiply_5D(__Vector5 vec, __scalar s)
{
    __Vector5 result;
    result.SetX(vec.X() * s);
    result.SetY(vec.Y() * s);
    result.SetZ(vec.Z() * s);
    result.SetW(vec.W() * s);
    result.SetV(vec.V() * s);
    return result;
}


//DOT PRODUCTS
__scalar __DotProduct_2D(__Vector2 first, __Vector2 second)
{
    __scalar result = first.X()*second.X() + first.Y()*second.Y();
    return result;
}

__scalar __DotProduct_3D(__Vector3 first, __Vector3 second)
{
    __scalar result = first.X()*second.X() + first.Y()*second.Y() + first.Z()*second.Z();
    return result;
}

__scalar __DotProduct_4D(__Vector4 first, __Vector4 second)
{
    __scalar result = first.X()*second.X() + first.Y()*second.Y() + first.Z()*second.Z() + first.W()*second.W();
    return result;
}

__scalar __DotProduct_5D(__Vector5 first, __Vector5 second)
{
    __scalar result = first.X()*second.X() + first.Y()*second.Y() + first.Z()*second.Z() + first.W()*second.W() + first.V()*second.V();
    return result;
}

//CROSS PRODUCT
__Vector3 __VectorX_3D(__Vector3 first, __Vector3 second)
{
    __Vector3 result;
    __Vector3 i = __Vector3(1, 0, 0);
    __Vector3 j = __Vector3(0, 1, 0);
    __Vector3 k = __Vector3(0, 0, 1);
    __scalar a, b, c;
    a = first.Y()*second.Z() - first.Z()*second.Y();
    b = first.Z()*second.X() - first.X()*second.Z();
    c = first.X()*second.Y() - first.Y()*second.X();
    result = __AddVector_3D(__VecScalarMultiply_3D(i,a), __VecScalarMultiply_3D(j,b));
    result = __AddVector_3D(result, __VecScalarMultiply_3D(k,c));
    return result;
}

//CROSS PRODUCT 4D ANALOGUE
//.
//.
//.

//PERSPECTIVE DIVISION
__Vector4 __Vector5PerspectiveDivide(__Vector5 vec)
{
    __Vector4 result;
    result.SetX(vec.X()/vec.V());
    result.SetY(vec.Y()/vec.V());
    result.SetZ(vec.Z()/vec.V());
    result.SetW(vec.W()/vec.V());
    return result;
}

__Vector3 __Vector4PerspectiveDivide(__Vector4 vec)
{
    __Vector3 result;
    result.SetX(vec.X()/vec.W());
    result.SetY(vec.Y()/vec.W());
    result.SetZ(vec.Z()/vec.W());
    return result;
}

//REDUSING VECTOR DIMENSION BY ONE
__Vector4 __Vector5DropV(__Vector5 vec)
{
    __Vector4 result = __Vector4(vec.X(), vec.Y(), vec.Z(), vec.W());
    return result;
}

__Vector3 __Vector4DropW(__Vector4 vec)
{
    __Vector3 result = __Vector3(vec.X(), vec.Y(), vec.Z());
    return result;
}

//MATRIX MULTIPLICATION
__Matrix4x4 __Matrix4x4_Multiply(__Matrix4x4 first, __Matrix4x4 second)
{
    __Matrix4x4 result;
    for (int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            __scalar value;
            __Vector4 vec1 = first.GetRow(i);
            __Vector4 vec2 = second.GetColumn(j);
            value = __DotProduct_4D(vec1, vec2);
            result.SetElement(i, j, value);
        }
    }
    return result;
}

__Matrix5x5 __Matrix5x5_Multiply(__Matrix5x5 first, __Matrix5x5 second)
{
    __Matrix5x5 result;
    for (int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            __scalar value;
            __Vector5 vec1 = first.GetRow(i);
            __Vector5 vec2 = second.GetColumn(j);
            value = __DotProduct_5D(vec1, vec2);
            result.SetElement(i, j, value);
        }   
    }   
    return result;
}

//MATRIX IDENTITIES
__Matrix4x4 __Matrix4x4Identity()
{
    __Matrix4x4 result;
    for (int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(i == j)
                result.SetElement(i,j,1);
        }
    }
    return result;
}

__Matrix5x5 __Matrix5x5Identity()
{
    __Matrix5x5 result;
    for (int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if(i == j)
                result.SetElement(i,j,1);
        }   
    }   
    return result;
}

//MATRIX TRANSPOSITION
__Matrix4x4 __Matrix4x4Transpose(__Matrix4x4 mat)
{
    __Matrix4x4 result;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            result.SetElement(j, i, mat.GetElement(i, j));
        }
    }
    return result;
}

__Matrix5x5 __Matrix5x5Transpose(__Matrix5x5 mat)
{
    __Matrix5x5 result;
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            result.SetElement(j, i, mat.GetElement(i, j));
        }   
    }   
    return result;
}


//SCALING MATRICES
__Matrix4x4 __ScaleMatrix4x4(__scalar x, __scalar y, __scalar z)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(0,0,x);
    result.SetElement(1,1,y);
    result.SetElement(2,2,z);
    return result;
}

//TRANSLATION MATRICES
__Matrix4x4 __TranslationMatrix4x4(__scalar x, __scalar y, __scalar z)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(0,3,x);
    result.SetElement(1,3,y);
    result.SetElement(2,3,z);
    return result;
}

__Matrix5x5 __TranslationMatrix5x5(__scalar x, __scalar y, __scalar z, __scalar w)
{
    __Matrix5x5 result = __Matrix5x5Identity();
    result.SetElement(0,4,x);
    result.SetElement(1,4,y);
    result.SetElement(2,4,z);
    result.SetElement(3,4,w);
    return result;
}

//ROTATION MATRICES
__Matrix4x4 __Matrix4x4RotationX(__scalar value)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(1,1,cos(value));
    result.SetElement(1,2, -sin(value));
    result.SetElement(2,1,sin(value));
    result.SetElement(2,2,cos(value));
    return result;
}

__Matrix4x4 __Matrix4x4RotationY(__scalar value)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(0,0,cos(value));
    result.SetElement(0,2,sin(value));
    result.SetElement(2,0, -sin(value));
    result.SetElement(2,2,cos(value));
    return result;
}

__Matrix4x4 __Matrix4x4RotationZ(__scalar value)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(0,0,cos(value));
    result.SetElement(0,1, -sin(value));
    result.SetElement(1,0,sin(value));
    result.SetElement(1,1,cos(value));
    return result;
}

__Matrix4x4 __Matrix4x4RotationYawPitchRoll(__scalar yaw_angle, __scalar pitch_angle, __scalar roll_angle)
{
    __Matrix4x4 rot_yaw = __Matrix4x4RotationY(yaw_angle);
    __Matrix4x4 rot_pitch = __Matrix4x4RotationX(pitch_angle);
    __Matrix4x4 rot_roll = __Matrix4x4RotationZ(roll_angle);
    __Matrix4x4 result = rot_yaw * rot_pitch * rot_roll;
    return result;
}

__Matrix4x4 __Matrix4x4RotationXW(__scalar value)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(0,0, cos(value));
    result.SetElement(0,3, -sin(value));
    result.SetElement(3,0, sin(value));
    result.SetElement(3,3, cos(value));
    return result;
}

__Matrix4x4 __Matrix4x4RotationYW(__scalar value)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(1,1, cos(value));
    result.SetElement(1,3, -sin(value));
    result.SetElement(3,1, sin(value));
    result.SetElement(3,3, cos(value));
    return result;
}

__Matrix4x4 __Matrix4x4RotationZW(__scalar value)
{
    __Matrix4x4 result = __Matrix4x4Identity();
    result.SetElement(2,2, cos(value));
    result.SetElement(2,3, -sin(value));
    result.SetElement(3,2, sin(value));
    result.SetElement(3,3, cos(value));
    return result;
}

//5x5 SINGLE PLANE 4D ROTATION MATRICES
__Matrix5x5 __Matrix5x5RotationXW(__scalar value)
{
    __Matrix5x5 result = __Matrix5x5Identity();
    result.SetElement(0,0, cos(value));
    result.SetElement(0,3, -sin(value));
    result.SetElement(3,0, sin(value));
    result.SetElement(3,3, cos(value));
    return result;
}

//PROJECTION MATRICES
//3D projection matrix
__Matrix4x4 __Matrix4x4PerspectiveFov(__scalar fov, __scalar ar, __scalar z_near, __scalar z_far)
{
    __scalar _cot = 1/tan(fov/2);
    __Matrix4x4 result;
    result.SetElement(0,0, _cot/ar);
    result.SetElement(1,1, _cot);
    result.SetElement(2,2, (__scalar)((z_near + z_far)/(z_near - z_far)));
    result.SetElement(2,3, (__scalar)(2*z_near*z_far/(z_near - z_far)));
    result.SetElement(3,2, -1);
    return result;
}

//4D Projection Matrix
__Matrix5x5 __Matrix5x5PerspectiveFov(__scalar fov)
{
    __Matrix5x5 result;
    __scalar _cot = 1/tan(fov/2);
    result.SetElement(0,0, _cot);
    result.SetElement(1,1, _cot);
    result.SetElement(2,2, _cot);
    result.SetElement(4,3, 1);    
    return result;
}

//VIEW MATRICES
__Matrix4x4 __Matrix4x4ViewLookAt(__Vector3 position, __Vector3 lookAt, __Vector3 up)
{
    __Matrix4x4 result;
    __Vector3 look = position - lookAt;
    look.normalize();
    __Vector3 right = __VectorX_3D(up, look);
    right.normalize();
    up = __VectorX_3D(look, right);
    up.normalize();
    result.SetElement(0,0, right.X());
    result.SetElement(1,0, right.Y());
    result.SetElement(2,0, right.Z());
    result.SetElement(0,1, up.X());
    result.SetElement(1,1, up.Y());
    result.SetElement(2,1, up.Z());
    result.SetElement(0,2, look.X());
    result.SetElement(1,2, look.Y());
    result.SetElement(2,2, look.Z());
    result.SetElement(0,3, -__DotProduct_3D(right, position));
    result.SetElement(1,3, -__DotProduct_3D(up, position));
    result.SetElement(2,3, -__DotProduct_3D(look, position));
    result.SetElement(3,3, 1);
    return result;
}

//VECTOR TRANSFORMATIONS
__Vector3 __Vec3Transform(__Vector3 vec, __Matrix4x4 mat)
{
    __Vector4 vector = __Vector4(vec, 0);
    vector.SetX(__DotProduct_4D(vector, mat.GetRow(0)));
    vector.SetY(__DotProduct_4D(vector, mat.GetRow(1)));
    vector.SetZ(__DotProduct_4D(vector, mat.GetRow(2)));
    vector.SetW(__DotProduct_4D(vector, mat.GetRow(3)));
    __Vector3 result = __Vector3(vector.X(), vector.Y(), vector.Z());
    return result;
}

__Vector4 __Vec4Transform(__Vector4 vec, __Matrix4x4 mat)
{
    __Vector4 result;
    result.SetX(__DotProduct_4D(vec, mat.GetRow(0)));
    result.SetY(__DotProduct_4D(vec, mat.GetRow(1)));
    result.SetZ(__DotProduct_4D(vec, mat.GetRow(2)));
    result.SetW(__DotProduct_4D(vec, mat.GetRow(3)));
    return result;
}

__Vector5 __Vec5Transform(__Vector5 vec, __Matrix5x5 mat)
{
    __Vector5 result; 
    result.SetX(__DotProduct_5D(vec, mat.GetRow(0)));
    result.SetY(__DotProduct_5D(vec, mat.GetRow(1)));
    result.SetZ(__DotProduct_5D(vec, mat.GetRow(2)));
    result.SetW(__DotProduct_5D(vec, mat.GetRow(3)));
    result.SetV(__DotProduct_5D(vec, mat.GetRow(4)));
    return result;
}
