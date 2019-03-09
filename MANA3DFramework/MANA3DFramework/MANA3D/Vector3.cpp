#include "Vector3.h"
using namespace MANA3D;

#include "Mathf.h"

Vector3::Vector3()
{
	x = y = z = 0;
}

Vector3::Vector3( float fX, float fY, float fZ )
{
	x = fX;
	y = fY;
	z = fZ;
}

Vector3::~Vector3()
{
}


Vector3& Vector3::operator=( const Vector3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3 Vector3::operator+( const Vector3 rhsV3 ) const
{
	return Vector3( x + rhsV3.x, y + rhsV3.y, z + rhsV3.z );
}

Vector3& Vector3::operator+=( const Vector3 rhsV3 )
{
	x += rhsV3.x;
	y += rhsV3.y;
	z += rhsV3.z;
	return *this;
}

Vector3 Vector3::operator-( const Vector3 rhsV3 ) const
{
	return Vector3( x - rhsV3.x, y - rhsV3.y, z - rhsV3.z );
}

Vector3& Vector3::operator-=( const Vector3 rhsV3 ) 
{
	x -= rhsV3.x;
	y -= rhsV3.y;
	z -= rhsV3.z;
	return *this;
}

Vector3 Vector3::operator*( float scaler ) const
{
	return Vector3( x * scaler, y * scaler, z * scaler );
}

Vector3& Vector3::operator*=( float scaler )
{
	x *= scaler;
	y *= scaler;
	z *= scaler;
	return *this;
}

Vector3 Vector3::operator/( float scaler ) const
{
	float oneOverS = 1 / scaler;
	return Vector3( x * oneOverS, y * oneOverS, z * oneOverS );
}

Vector3& Vector3::operator/=( float scaler )
{
	float oneOverS = 1 / scaler;
	x *= oneOverS;
	y *= oneOverS;
	z *= oneOverS;
	return *this;
}

bool Vector3::operator==( const Vector3 rhsV3 ) const
{
	return ( x == rhsV3.x && y == rhsV3.y && z == rhsV3.z );
}

bool Vector3::operator!=( const Vector3 rhsV3 ) const
{
	return ( x != rhsV3.x || y != rhsV3.y || z != rhsV3.z );
}

float Vector3::operator[]( unsigned i ) const
{
	if ( i == 0 ) return x;
	if ( i == 1 ) return y;
	return z;
}

float& Vector3::operator[]( unsigned i )
{
	if ( i == 0 ) return x;
	if ( i == 1 ) return y;
	return z;
}


float Vector3::GetMagnitude() const
{
	return (float)sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
}

float Vector3::GetSquareMagnitude() const
{
	return x*x + y*y + z*z;
}

Vector3 Vector3::GetNormalized() const
{
	Vector3 vector( x, y, z );

	// Get the magnitude of our normal.
	float magnitudeF = GetMagnitude();

	vector = vector / magnitudeF;

	// Return the normalized vector.
	return vector;
}




const Vector3 MANA3D::VECTOR3_ZERO( 0, 0, 0 );
const Vector3 MANA3D::VECTOR3_ONE( 1, 1, 1 );
const Vector3 MANA3D::VECTOR3_UP( 0, 1, 0 );
const Vector3 MANA3D::VECTOR3_DOWN( 0, -1, 0 );
const Vector3 MANA3D::VECTOR3_RIGHT( 1, 0, 0 );
const Vector3 MANA3D::VECTOR3_LEFT( -1, 0, 0 );
const Vector3 MANA3D::VECTOR3_FORWARD( 0, 0, 1 );
const Vector3 MANA3D::VECTOR3_BACKWARD( 0, 0, -1 );


float MANA3D::GetAngle( const Vector3 vFrom, const Vector3 vTo )
{
	// Dot( vFrom, vTo ) = ||vFrom||.||vTo||.Cos( a )	
	float dot = CalculateDotProduct( vFrom, vTo );
	float lenSq1 = vFrom.x * vFrom.x + vFrom.y * vFrom.y + vFrom.z * vFrom.z;
	float lenSq2 = vTo.x * vTo.x + vTo.y * vTo.y + vTo.z * vTo.z;
	return acosf( dot / sqrtf( lenSq1 * lenSq2 ) );
}

float MANA3D::GetDistance( const Vector3 p1, const Vector3 p2 )
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float dz = p1.z - p2.z;
	return sqrtf( dx * dx + dy * dy + dz * dz );
}

int MANA3D::CompareDistance( const Vector3 p1, const Vector3 p2, float distance )
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float dz = p1.z - p2.z;
	
	float val = dx * dx + dy * dy + dz * dz;
	float sqVal = distance * distance;

	if ( val > sqVal )
		return 1;	// Actual distance is larger than the given one.
	else if ( val < sqVal )
		return -1;	// Actual distance is smaller than the given one.
	else
		return 0;	// Actual distance is equal to the given one.
}


float MANA3D::CalculateDotProduct( const Vector3 v1, const Vector3 v2 )
{
	return ( v1.x * v2.x ) + ( v1.y * v2.y ) + ( v1.z * v2.z );
}

Vector3 MANA3D::CalculateCrossProduct( const Vector3 v1, const Vector3 v2 )
{
	return Vector3( v1.y * v2.z - v1.z * v2.y,
					v1.z * v2.x - v1.x * v2.z,
					v1.x * v2.y - v1.y * v2.x );
}


Vector3 MANA3D::LerpPoint3D( const Vector3 from, const Vector3 to, float t )
{
	float x = from.x + t * ( to.x - from.x );
	float y = from.y + t * ( to.y - from.y );
	float z = from.z + t * ( to.z - from.z );
	return Vector3( x, y, z );
}

Vector3 MANA3D::LerpVector3( const Vector3 from, const Vector3 to, float t )
{
	float angle = GetAngle( from, to );

	float x = ( sinf( ( 1 - t ) * angle ) / sinf( angle ) ) * from.x + ( sinf( t * angle ) / sinf( angle ) ) * to.x;
	float y = ( sinf( ( 1 - t ) * angle ) / sinf( angle ) ) * from.y + ( sinf( t * angle ) / sinf( angle ) ) * to.y;
	float z = ( sinf( ( 1 - t ) * angle ) / sinf( angle ) ) * from.z + ( sinf( t * angle ) / sinf( angle ) ) * to.z;

	return Vector3( x, y, z );
}

