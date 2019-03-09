#include "Quaternion.h"
#include "Mathf.h"
using namespace MANA3D;

#include <assert.h>


Quaternion::Quaternion()
{
	w = 1;
	x = y = z = 0;
}

Quaternion::Quaternion( float W, float X, float Y, float Z )
{
	w = W;
	x = X;
	y = Y;
	z = Z;
}

Quaternion::~Quaternion()
{

}


void Quaternion::operator *=( const Quaternion &multiplier )
{
	Quaternion q = *this;
	w = q.w*multiplier.w - q.x*multiplier.x - q.y*multiplier.y - q.z*multiplier.z;
	x = q.w*multiplier.x + q.x*multiplier.w + q.y*multiplier.z - q.z*multiplier.y;
	y = q.w*multiplier.y + q.y*multiplier.w + q.z*multiplier.x - q.x*multiplier.z;
	z = q.w*multiplier.z + q.z*multiplier.w + q.x*multiplier.y - q.y*multiplier.x;
}


void Quaternion::Normalize()
{
	float d = w*w + x*x + y*y + z*z;

	// Check for zero length quaternion, and use the no-rotation quaternion in that case.
	if ( d == 0 ) 
	{
		w = 1;
		return;
	}

	d = ((float)1.0) / sqrtf( d );
	w *= d;
	x *= d;
	y *= d;
	z *= d;
}

Quaternion Quaternion::GetNormalized() const
{
	Quaternion result;

	// Compute magnitude of the quaternion.
	float mag = sqrtf( w * w + x * x + y * y + z * z );

	// Check for bogus length to avoid divide by zero.
	if ( mag > 0.0f )
	{
		float oneOverMag = 1.0f / mag;
		result.w *= oneOverMag;
		result.x *= oneOverMag;
		result.y *= oneOverMag;
		result.z *= oneOverMag;
	}
	else
	{
		assert( false );
		result = QUATERNION_IDENTITY;
	}

	return result;
}


void Quaternion::AddScaledVector( const Vector3& vector, float scale )
{
	Quaternion q( 0,
					vector.x * scale,
					vector.y * scale,
					vector.z * scale );
	q *= *this;
	w += q.w * 0.5f;
	x += q.x * 0.5f;
	y += q.y * 0.5f;
	z += q.z * 0.5f;
}



float   Quaternion::GetRotationAngle() const
{
	// Compute the half angle. w = cos(theta / 2)
	float thetaOver2 = acosf( w );

	return thetaOver2 * 2.0f;
}

Vector3  Quaternion::GetRotationAxis() const
{
	// Compute sin^2(theta/2). w = cos(theta/2)
	// sin^2(x) + cos^2(x) = 1
	float sinThetaOver2Sq = 1.0f - w * w;

	if ( sinThetaOver2Sq < 0.0f )
		return VECTOR3_ZERO;

	// Compute 1 / sin(theta/2)
	float oneOverSinThetaOver2 = 1.0f / sqrtf( sinThetaOver2Sq );

	// Return axis of rotation.
	return Vector3( x * oneOverSinThetaOver2,
					y * oneOverSinThetaOver2,
					z * oneOverSinThetaOver2 );
}


Vector3 Quaternion::GetObjectToInertialEulerAngles()
{
	Vector3 result;

	// Extract sin(x).
	float sinX = -2.0f * ( y * z - w * x );

	// Check for Gimble Lock, giving slight tolerance for numerical imprecision.
	if ( fabsf( sinX ) > 0.9999f )
	{
		// Looking straight up or down.
		result.x = PIOver2 * sinX;

		// Compute y
		result.y = atan2f( -x * z + w * y, 0.5f - y * y - z * z );

		// Slam z to zero.
		result.z = 0.0f;
	}
	else
	{
		// Compute angles, we don't have to use the safe asin function beccause we already checked for range errors we checking for Gimble lock.
		result.x = asinf( sinX );
		result.y = atan2f( x * z + w * y, 0.5f - x * x - y * y );
		result.z = atan2f( x * y + w * z, 0.5f - x * x - z * z );
	}

	return result;
}

Vector3 Quaternion::GetInertialToObjectEulerAngles()
{
	Vector3 result;

	// Extract sin(x)
	float sinX = -2.0f * ( y * z + w * x );

	// Check for Gimbel Lock, giving slight tolerance for numerical imprecision.
	if ( fabsf(sinX) > 0.9999f )
	{
		// Looking straight up or down.
		result.x = PIOver2 * sinX;

		// Compute Y.
		result.y = atan2f( -x * z - w * y, 0.5f - y * y - z * z );
		result.z = 0;
	}
	else
	{
		// Compute angles, we don't have to use the safe asin function beccause we already checked for range errors we checking for Gimble lock.
		result.x = asinf( sinX );
		result.y = atan2f( x * z - w * y, 0.5f - x * x - y * y );
		result.z = atan2f( x * y - w * z, 0.5f - x * x - z * z );
	}

	return result;
}




const Quaternion MANA3D::QUATERNION_IDENTITY( 1.0f, 0.0f, 0.0f, 0.0f );


float MANA3D::QuaternionDot( const Quaternion& q1, const Quaternion& q2 )
{
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

Quaternion MANA3D::QuaternionCross( const Quaternion& q1, const Quaternion& q2 )
{
	Quaternion result;

	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	result.x = q1.w * q2.x + q1.x * q2.w + q1.z * q2.y - q1.y * q2.z;
	result.y = q1.w * q2.y + q1.y * q2.w + q1.x * q2.z - q1.z * q2.x;
	result.z = q1.w * q2.z + q1.z * q2.w + q1.y * q2.x - q1.x * q2.y;

	return result;
}

Quaternion MANA3D::QuaternionSlerp( const Quaternion& from, const Quaternion& to, float t )
{
	// Check for out range parameters and return edge points if so.
	if ( t <= 0.0f ) return from;
	if ( t >= 1.0f ) return to;

	// Compute cos of angle btween quaternions using dot product.
	float cosOmega = QuaternionDot( from, to );

	// If negative dot, use (-from). two quaternions q and -q represent the same rotation, but may produce different slerp.
	// We chose q or -q to rotate using the acute angle.

	float toW = to.w;
	float toX = to.x;
	float toY = to.y;
	float toZ = to.z;
	
	if ( cosOmega < 0.0f )
	{
		toW *= -1;
		toX *= -1;
		toY *= -1;
		toZ *= -1;
		cosOmega *= -1;
	}

	// We should have two unit quaternions, so dot should be <= 1.0f
	assert( cosOmega < 1.1f );

	// Compute interpolation fraction, checking for quaternions almost exactly the same.
	float kFrom, kTo;
	if ( cosOmega < 0.9999f )
	{
		// Very close - just use linear interploation, to avoid a divide by zero.
		kFrom = 1.0f - t;
		kTo = t;
	}
	else
	{
		// Compute the sin of the angle using the trig identity sin^2(omega) + cos^2(omega) = 1
		float sinOmega = sqrtf( 1.0f - cosOmega * cosOmega );

		// Compute the angle from its sin and cos.
		float omega = atan2f( sinOmega, cosOmega );

		// Compute inverse of denominator, so we only have to divide once.
		float oneOverSinOmega = 1.0f / sinOmega;

		// Compute interpolation parameters.
		kFrom = sinf( ( 1.0f - t ) * omega ) * oneOverSinOmega;
		kTo = sinf( t * omega ) * oneOverSinOmega;
	}

	// Interpolate.
	Quaternion result;

	result.x = kFrom * from.x + kTo * to.x;
	result.x = kFrom * from.y + kTo * to.y;
	result.x = kFrom * from.z + kTo * to.z;
	result.x = kFrom * from.w + kTo * to.w;

	// Return the result.
	return result;
}

Quaternion MANA3D::QuaternionConjugate( const Quaternion& q )
{
	// Returns a quaternion with the opposite rotation as the original quaternion.
	Quaternion result;

	// Same rotation amount.
	result.w = q.w;

	// Opposite axis of rotation.
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;

	return result;
}

Quaternion MANA3D::QuaternionPow( const Quaternion& q, float exponent )
{
	// Check for identity quaternion to avoid divide by zero.
	if ( fabsf( q.w ) > 0.9999f )
		return q;

	// Extract the half angle alpha (alpha = theta/2)
	float alpha = acosf( q.w );

	// Compute new alpha value.
	float newAlpha = alpha * exponent;

	// Compute new w value.
	Quaternion result;
	result.w = cosf( newAlpha );

	// Compute new x y z values.
	float mult = sinf( newAlpha ) / sinf( alpha );

	result.x = q.x * mult;
	result.y = q.y * mult;
	result.z = q.z * mult;

	return result;
}