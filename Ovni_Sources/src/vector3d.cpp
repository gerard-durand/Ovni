#include "vector3d.h"
#include <cmath>

Vector3D::Vector3D()
{
    X = 0;
    Y = 0;
    Z = 0;
}

Vector3D::Vector3D(double x,double y,double z)
{
    X = x;
    Y = y;
    Z = z;
}

Vector3D::Vector3D(const Vector3D & v)
{
    X = v.X;
    Y = v.Y;
    Z = v.Z;
}

Vector3D::Vector3D(const Vector3D & from,const Vector3D & to)
{
    X = to.X - from.X;
    Y = to.Y - from.Y;
    Z = to.Z - from.Z;
}

Vector3D & Vector3D::operator= (const Vector3D & v)
{
    X = v.X;
    Y = v.Y;
    Z = v.Z;
    return *this;
}

Vector3D & Vector3D::operator+= (const Vector3D & v)
{
    X += v.X;
    Y += v.Y;
    Z += v.Z;
    return *this;
}

Vector3D Vector3D::operator+ (const Vector3D & v) const
{
    Vector3D t = *this;
    t += v;
    return t;
}

Vector3D & Vector3D::operator-= (const Vector3D & v)
{
    X -= v.X;
    Y -= v.Y;
    Z -= v.Z;
    return *this;
}

Vector3D Vector3D::operator- (const Vector3D & v) const
{
    Vector3D t = *this;
    t -= v;
    return t;
}

Vector3D & Vector3D::operator*= (const double a)
{
    X *= a;
    Y *= a;
    Z *= a;
    return *this;
}

Vector3D Vector3D::operator* (const double a)const
{
    Vector3D t = *this;
    t *= a;
    return t;
}

Vector3D operator* (const double a,const Vector3D & v)
{
    return Vector3D(v.X*a,v.Y*a,v.Z*a);
}

Vector3D & Vector3D::operator/= (const double a)
{
    X /= a;
    Y /= a;
    Z /= a;
    return *this;
}

Vector3D Vector3D::operator/ (const double a)const
{
    Vector3D t = *this;
    t /= a;
    return t;
}

Vector3D Vector3D::crossProduct(const Vector3D & v)const
{
    Vector3D t;
    t.X = Y*v.Z - Z*v.Y;
    t.Y = Z*v.X - X*v.Z;
    t.Z = X*v.Y - Y*v.X;
    return t;
}

double Vector3D::length()const
{
    return sqrt( X*X + Y*Y + Z*Z);
}

Vector3D & Vector3D::normalize()
{
    (*this) /= length();
    return (*this);
}

Vector3D Vector3D::MultPoint4dMatrice4x4(const double mat[16])
{
    // Adaptation de MultPoint4dMatrice4x4 de la version Tcl d'Ovni travaillant sur les vecteurs 4D (X,Y,Z,W). Ici, W, la 4ème coordonnée vaut toujours 1
	Vector3D res;
//	Vector3D final;

	res.X = mat[0]*X + mat[4]*Y + mat[8]*Z  + mat[12]; //*W;
	res.Y = mat[1]*X + mat[5]*Y + mat[9]*Z  + mat[13]; //*W;
	res.Z = mat[2]*X + mat[6]*Y + mat[10]*Z + mat[14]; //*W;
//	res.W = mat[3]*X + mat[7]*Y + mat[11]*Z + mat[15]; //*W; // inutile si mat[15] et W = 1 et mat 3, 7 et 11 = 0


//	final.x = (double)(res.x);
//	final.y = (double)(res.y);
//	final.z = (double)(res.z);

//	Normalise(&final);

//	return final;
    return res ;
}
