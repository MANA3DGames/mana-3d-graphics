#include "Matrix4X4.h"
using namespace MANA3D;

#include <assert.h>


Matrix4X4::Matrix4X4()
{

}

Matrix4X4::Matrix4X4( float M11, float M12, float M13, float M14,
					  float M21, float M22, float M23, float M24,
					  float M31, float M32, float M33, float M34,
					  float M41, float M42, float M43, float M44 )
{
	m11 = M11; m12 = M12; m13 = M13; m14 = M14;
	m21 = M21; m22 = M22; m23 = M23; m24 = M24;
	m31 = M31; m32 = M32; m33 = M33; m34 = M34;
	m41 = M41; m42 = M42; m43 = M43; m44 = M44;
}

Matrix4X4::Matrix4X4( const Vector3& orientation )
{
	float sinX = sinf( orientation.x );
	float cosX = cosf( orientation.x );
	float sinY = sinf( orientation.y );
	float cosY = cosf( orientation.y );
	float sinZ = sinf( orientation.z );
	float cosZ = cosf( orientation.z );

	m11 = cosY * cosZ + sinX * sinY * sinZ;
	m12 = -cosY * sinZ + sinX * sinY * cosZ;
	m13 = cosX * sinY;
	m14 = 0.0f;

	m21 = cosX * sinZ;
	m22 = cosX * cosZ;
	m23 = -sinX;
	m24 = 0.0f;

	m31 = -sinY * cosZ + sinX * cosY * sinZ;
	m32 = sinY * sinZ + sinX * cosY * cosZ;
	m33 = cosX * cosY;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}

//Matrix4X4::Matrix4X4( const Vector3& translatation,
//					  const Vector3& orientation,
//					  const Vector3& scale)
//{
//
//}

Matrix4X4::Matrix4X4( const Quaternion& q, Space3D space )
{
	if ( space == Space3D::InertialToObject )
		FromInertialToObjectQuaternion( q );
	else if (space == Space3D::ObjectToInertial)
		FromObjectToInertialQuaternion(q);
	else
		*this = MATRIX4X4_IDENTITY;
}

Matrix4X4::~Matrix4X4()
{

}


void Matrix4X4::FromInertialToObjectQuaternion( const Quaternion& q )
{
	m11 = 1.0f - 2.0f * ( q.y * q.y + q.z * q.z );
	m12 = 2.0f * ( q.x * q.y + q.w * q.z );
	m13 = 2.0f * ( q.x * q.z - q.w * q.y );
	m14 = 0.0f;

	m21 = 2.0f * ( q.x * q.y - q.w * q.z );
	m22 = 1.0f - 2.0f * ( q.x * q.x + q.z * q.z );
	m23 = 2.0f * ( q.y * q.z + q.w * q.x );
	m24 = 0.0f;

	m31 = 2.0f * ( q.x * q.z + q.w * q.y );
	m32 = 2.0f * ( q.y * q.z - q.w * q.x );
	m33 = 1.0f - 2.0f * ( q.x * q.x + q.y * q.y );
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}

void Matrix4X4::FromObjectToInertialQuaternion( const Quaternion& q )
{
	m11 = 1.0f - 2.0f * ( q.y * q.y + q.z * q.z );
	m12 = 2.0f * ( q.x * q.y - q.w * q.z );
	m13 = 2.0f * ( q.x * q.z + q.w * q.y );
	m14 = 0.0f;

	m21 = 2.0f * ( q.x * q.y + q.w * q.z );
	m22 = 1.0f - 2.0f * ( q.x * q.x + q.z * q.z );
	m23 = 2.0f * ( q.y * q.z - q.w * q.x );
	m24 = 0.0f;

	m31 = 2.0f * ( q.x * q.z - q.w * q.y );
	m32 = 2.0f * ( q.y * q.z + q.w * q.x );
	m33 = 1.0f - 2.0f * ( q.x * q.x + q.y * q.y );
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}


Vector3 Matrix4X4::RotateVector3FromInertialToObject( const Vector3& v ) const
{
	// Perform the matrix multiplication in the standard way.
	return Vector3( m11 * v.x + m21 * v.y + m31 * v.z,
					m12 * v.x + m22 * v.y + m32 * v.z,
					m13 * v.x + m23 * v.y + m33 * v.z );
}

Vector3 Matrix4X4::RotateVector3FromObjectToInertial( const Vector3& v ) const
{
	// Multiply by the transpose.
	return Vector3( m11 * v.x + m12 * v.y + m13 * v.z,
					m21 * v.x + m22 * v.y + m23 * v.z,
					m31 * v.x + m32 * v.y + m33 * v.z );
}


void Matrix4X4::ZeroTranslation()
{
	m41 = m42 = m43 = 0.0f;
}

void Matrix4X4::SetTranslation( const Vector3& d )
{
	m41 = d.x;
	m42 = d.y;
	m43 = d.z;
}

void Matrix4X4::MakeTranslationOnly( const Vector3& d )
{
	m11 = 1.0f; m12 = 0.0f; m31 = 0.0f; m34 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;

	m41 = d.x; m42 = d.y; m43 = d.z; m44 = 1.0f;
}


void Matrix4X4::SetupLocalToParent( const Vector3& pos, const Vector3& orient )
{
	Matrix4X4 orientMatrix( orient );
	SetupLocalToParent( pos, orientMatrix );
}

void Matrix4X4::SetupLocalToParent( const Vector3& pos, const Matrix4X4& orient )
{
	m11 = orient.m11; m12 = orient.m12; m13 = orient.m13; m14 = orient.m14;
	m21 = orient.m21; m22 = orient.m22; m23 = orient.m23; m24 = orient.m24;
	m31 = orient.m31; m32 = orient.m32; m33 = orient.m33; m34 = orient.m34;

	m41 = pos.x; m42 = pos.y; m43 = pos.z; m44 = orient.m44;
}

void Matrix4X4::SetupParentToLocal( const Vector3& pos, const Vector3& orient )
{
	Matrix4X4 orientMatrix( orient );
	SetupParentToLocal( pos, orientMatrix );
}

void Matrix4X4::SetupParentToLocal( const Vector3& pos, const Matrix4X4& orient )
{
	m11 = orient.m11; m12 = orient.m12; m13 = orient.m13; m14 = orient.m14;
	m21 = orient.m21; m22 = orient.m22; m23 = orient.m23; m24 = orient.m24;
	m31 = orient.m31; m32 = orient.m32; m33 = orient.m33; m34 = orient.m34;

	// The rotation is occurs first so we must rotate the translation portion.
	m41 = -( pos.x * m11 + pos.y * m21 + pos.z * m31 ); 
	m42 = -( pos.x * m12 + pos.y * m22 + pos.z * m32 ); 
	m43 = -( pos.x * m13 + pos.y * m23 + pos.z * m33 );
	m44 = orient.m44;
}


void Matrix4X4::Rotate( const Quaternion& q )
{
	float ww = 2.0f * q.w;
	float xx = 2.0f * q.x;
	float yy = 2.0f * q.y;
	float zz = 2.0f * q.z;

	m11 = 1.0f - yy * q.y - zz * q.z;
	m12 = xx * q.y + ww * q.z;
	m13 = xx * q.z - ww * q.x;
	m14 = 0.0f;

	m21 = xx * q.y - ww * q.z;
	m22 = 1.0f - xx * q.x - zz * q.z;
	m23 = yy * q.z + ww * q.x;
	m24 = 0.0f;

	m31 = xx * q.z + ww * q.y;
	m32 = yy * q.z - ww * q.x;
	m33 = 1.0f - ww * q.x - yy * q.y;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}

void Matrix4X4::RotateX( float theta )
{
	float sinTheta = sinf( theta );
	float cosTheta = cosf( theta );

	m11 = 1.0f;		m12 = 0.0f;			m13 = 0.0f;			m14 = 0.0f;
	m21 = 0.0f;		m22 = cosTheta;		m23 = sinTheta;		m24 = 0.0f;
	m31 = 0.0f;		m32 = -sinTheta;	m33 = cosTheta;		m34 = 0.0f;
	m41 = 0.0f;		m42 = 0.0f;			m43 = 0.0f;			m44 = 1.0f;
}

void Matrix4X4::RotateY( float theta )
{
	float sinTheta = sinf( theta );
	float cosTheta = cosf( theta );

	m11 = cosTheta;		m12 = 0.0f;		m13 = -sinTheta;	m14 = 0.0f;
	m21 = 0.0f;			m22 = 1.0f;		m23 = 0.0f;			m24 = 0.0f;
	m31 = sinTheta;		m32 = 0.0f;		m33 = cosTheta;		m34 = 0.0f;
	m41 = 0.0f;			m42 = 0.0f;		m43 = 0.0f;			m44 = 1.0f;
}

void Matrix4X4::RotateZ( float theta )
{
	float sinTheta = sinf( theta );
	float cosTheta = cosf( theta );

	m11 = cosTheta;		m12 = sinTheta;		m13 = 0.0f;		m14 = 0.0f;
	m21 = -sinTheta;	m22 = cosTheta;		m23 = 0.0f;		m24 = 0.0f;
	m31 = 0.0f;			m32 = 0.0f;			m33 = 1.0f;		m34 = 0.0f;
	m41 = 0.0f;			m42 = 0.0f;			m43 = 0.0f;		m44 = 1.0f;
}

void Matrix4X4::RotateAxis( const Vector3& axis, float theta )
{
	// Make sure they passed in a unit vector to specify the axis. 
	assert( fabsf( CalculateDotProduct( axis, axis ) - 1.0f ) < 0.01f );

	float sinTheta = sinf( theta );
	float cosTheta = cosf( theta );

	float a = 1.0f - cosTheta;
	float ax = a * axis.x;
	float ay = a * axis.y;
	float az = a * axis.z;

	m11 = ax * axis.x + cosTheta;
	m12 = ax * axis.y + axis.z * sinTheta;
	m13 = ax * axis.z - axis.y * sinTheta;
	m14 = 0.0f;

	m21 = ay * axis.x - axis.z * sinTheta;
	m22 = ay * axis.y + cosTheta;
	m23 = ay * axis.z + axis.x * sinTheta;
	m24 = 0.0;

	m31 = az * axis.x + axis.y * sinTheta;
	m32 = az * axis.y - axis.x * sinTheta;
	m33 = az * axis.z + cosTheta;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}


void Matrix4X4::Scale( const Vector3& s )
{
	m11 = s.x;	m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	m21 = 0.0f; m22 = s.y;	m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = s.z;	m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}

void Matrix4X4::ScaleAlongAxis( const Vector3& axis, float k )
{
	assert( fabsf( CalculateDotProduct( axis, axis ) - 1 ) < 0.01f );

	float a = k - 1.0f;
	float ax = a * axis.x;
	float ay = a * axis.y;
	float az = a * axis.z;

	m11 = ax * axis.x + 1.0f;
	m22 = ay * axis.y + 1.0f;
	m33 = az * axis.z + 1.0f;	// m32 in the reference!!

	m12 = m21 = ax * axis.y;
	m13 = m31 = ax * axis.z;
	m23 = m32 = ay * axis.z;

	m41 = m42 = m43 = 0.0f;

	m14 = m24 = m34 = 0.0f;
	m44 = 1.0f;
}


void Matrix4X4::ShearX( float s, float t )
{
	m11 = 1.0f; m12 = s;	m13 = t;	m14 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;	m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}

void Matrix4X4::ShearY( float s, float t )
{
	m11 = 1.0f; m12 = 0.0f;	m13 = 0.0f;	m14 = 0.0f;
	m21 = s;	m22 = 1.0f; m23 = t;	m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;	m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}

void Matrix4X4::ShearZ( float s, float t )
{
	m11 = 1.0f; m12 = 0.0f;	m13 = 0.0f;	m14 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	m31 = s;	m32 = t;	m33 = 1.0f;	m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}


void Matrix4X4::Project( const Vector3& n )
{
	assert( fabsf( CalculateDotProduct( n, n ) - 1.0f ) < 0.01f );

	m11 = 1.0f - n.x * n.x;
	m22 = 1.0f - n.y * n.y;
	m33 = 1.0f - n.z * n.z;
	m44 = 1.0f;

	m12 = m21 = -n.x * n.y;
	m13 = m31 = -n.x * n.z;
	m23 = m32 = -n.y * n.z;

	m14 = m24 = m34 = 0.0f;
	m41 = m42 = m43 = 0.0f;
}


void Matrix4X4::Reflect( const Vector3& n )
{
	assert( fabsf( CalculateDotProduct( n, n ) - 1.0f ) < 0.01f );

	float ax = -2.0f * n.x;
	float ay = -2.0f * n.y;
	float az = -2.0f * n.z;

	m11 = 1.0f + ax * n.x;
	m22 = 1.0f + ay * n.y;
	m33 = 1.0f + az * n.z;
	m44 = 1.0f;

	m12 = m21 = ax * n.y;
	m13 = m31 = ax * n.z;
	m23 = m32 = ay * n.z;

	m14 = m24 = m34 = 0.0f;
	m41 = m42 = m43 = 0.0f;
}

void Matrix4X4::ReflectX( float k = 0.0f )
{
	m11 = -1.0f;	 m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	m21 =  0.0f;	 m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	m31 =  0.0f;	 m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;

	m41 =  2.0f * k; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}

void Matrix4X4::ReflectY( float k = 0.0f )
{
	m11 = 1.0f;	m12 =  0.0f;	m13 = 0.0f; m14 = 0.0f;
	m21 = 0.0f;	m22 = -1.0f;	m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f;	m32 =  0.0f;	m33 = 1.0f; m34 = 0.0f;

	m41 = 0.0f; m42 = 2.0f * k; m43 = 0.0f; m44 = 1.0f;
}

void Matrix4X4::ReflectZ( float k = 0.0f )
{
	m11 = 1.0f;	m12 = 0.0f;	m13 =  0.0f;	m14 = 0.0f;
	m21 = 0.0f;	m22 = 1.0f;	m23 =  0.0f;	m24 = 0.0f;
	m31 = 0.0f;	m32 = 0.0f;	m33 = -1.0f;	m34 = 0.0f;

	m41 = 0.0f; m42 = 0.0f; m43 = 2.0f * k; m44 = 1.0f;
}


Vector3 Matrix4X4::GetEulerAngles()
{
	Vector3 result;

	float sinX = -m23;

	// check for Gimbel Lock.
	if ( fabsf( sinX ) > 0.9999f )
	{
		// Looking straight up or down.
		result.x = PIOver2 * sinX;
		result.y = atan2f( -m31, m11 );
		result.z = 0.0f;
	}
	else
	{
		result.x = asinf( sinX );
		result.y = atan2f( m13, m33 );
		result.z = atan2f( m21, m22 );
	}

	return result;
}



const Matrix4X4 MANA3D::MATRIX4X4_IDENTITY( 1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 1.0f );


Vector3 MANA3D::operator*( const Vector3& p, const Matrix4X4& m )
{
	return Vector3( p.x * m.m11 + p.y * m.m21 + p.z * m.m31 + m.m41,
					p.x * m.m12 + p.y * m.m22 + p.z * m.m32 + m.m42,
					p.x * m.m13 + p.y * m.m22 + p.z * m.m33 + m.m43 );
}

Vector3 MANA3D::operator*=( Vector3& p, const Matrix4X4& m )
{
	p = p * m;
	return p;
}


Matrix4X4 MANA3D::operator*( const Matrix4X4& a, const Matrix4X4& b )
{
	Matrix4X4 result;

	result.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
	result.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
	result.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;

	result.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
	result.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
	result.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;

	result.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
	result.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
	result.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;

	// Translation portion.
	result.m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + b.m41;
	result.m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + b.m42;
	result.m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + b.m43;

	return result;
}

Matrix4X4 MANA3D::operator*=( Matrix4X4& a, const Matrix4X4& b )
{
	a = a * b;
	return a;
}


float MANA3D::Matrix4X4Determinant( const Matrix4X4& m )
{
	return		m.m11 * ( m.m22 * m.m33 - m.m23 * m.m32 )
			+	m.m12 * ( m.m23 * m.m31 - m.m21 * m.m33 )
			+	m.m13 * ( m.m21 * m.m32 - m.m32 * m.m31 );
}

Matrix4X4 MANA3D::Matrix4X4Inverse( const Matrix4X4& m )
{
	float det = Matrix4X4Determinant( m );

	// If singular, then the determinent is zero and there is no inverse.
	assert( fabsf( det ) > 0.000001f );

	float oneOverDet = 1.0f / det;

	// Dividing the adjoint by the determinant.
	Matrix4X4 result;

	result.m11 = ( m.m22 * m.m33 - m.m23 * m.m32 ) * oneOverDet;
	result.m12 = ( m.m13 * m.m32 - m.m12 * m.m33 ) * oneOverDet;
	result.m13 = ( m.m12 * m.m23 - m.m13 * m.m22 ) * oneOverDet;

	result.m21 = ( m.m23 * m.m31 - m.m21 * m.m33 ) * oneOverDet;
	result.m22 = ( m.m11 * m.m33 - m.m13 * m.m31 ) * oneOverDet;
	result.m23 = ( m.m13 * m.m21 - m.m11 * m.m23 ) * oneOverDet;

	result.m31 = ( m.m21 * m.m32 - m.m22 * m.m31 ) * oneOverDet;
	result.m32 = ( m.m12 * m.m31 - m.m11 * m.m32 ) * oneOverDet;
	result.m33 = ( m.m11 * m.m22 - m.m12 * m.m21 ) * oneOverDet;

	// Translation portion.
	result.m41 = -( m.m41 * result.m11 + m.m42 * result.m21 + m.m43 * result.m31 );
	result.m41 = -( m.m41 * result.m12 + m.m42 * result.m22 + m.m43 * result.m32 );
	result.m41 = -( m.m41 * result.m13 + m.m42 * result.m23 + m.m43 * result.m33 );

	return result;
}


Vector3 MANA3D::Matrix4X4Translation( const Matrix4X4& m )
{
	return Vector3( m.m41, m.m42, m.m43 );
}


Vector3 MANA3D::Matrix4X4ObjectToWorldEulerAngles( const Matrix4X4& m )
{
	Vector3 euler;

	float sinX = -m.m32;

	// Check for Gimbel lock.
	if ( fabs(sinX) > 0.99999f )
	{
		// Looking straight down or up.
		euler.x = PIOver2 * sinX;
		euler.y = atan2f( -m.m23, m.m11 );
		euler.z = 0.0f;
	}
	else
	{
		euler.x = asinf( sinX );
		euler.y = atan2f( m.m31, m.m33 );
		euler.z = atan2f( m.m12, m.m22 );
	}

	return euler;
}

// same as GetEulerAngles()
Vector3 MANA3D::Matrix4X4WorldToObjectEulerAngles( const Matrix4X4& m )
{
	Vector3 euler;

	float sinX = -m.m23;

	// Check for Gimbel lock.
	if ( fabs(sinX) > 0.99999f )
	{
		// Looking straight down or up.
		euler.x = PIOver2 * sinX;
		euler.y = atan2f( -m.m23, m.m11 );
		euler.z = 0.0f;
	}
	else
	{
		euler.x = asinf( sinX );
		euler.y = atan2f( m.m31, m.m33 );
		euler.z = atan2f( m.m12, m.m22 );
	}

	return euler;
}




Vector3 MANA3D::Matrix4X4PositionFromParentToLocal( const Matrix4X4& m )
{
	return Vector3( -( m.m41 * m.m11 + m.m42 * m.m12 + m.m43 * m.m13 ),
					-( m.m41 * m.m21 + m.m42 * m.m22 + m.m43 * m.m23 ), 
					-( m.m41 * m.m31 + m.m42 * m.m32 + m.m43 * m.m33 ) );
}

Vector3 MANA3D::Matrix4X4PositionFromLocalToParent( const Matrix4X4& m )
{
	return Vector3( m.m41, m.m42, m.m43 );
}