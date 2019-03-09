#ifndef MANA3D_QUATERNION_H
#define MANA3D_QUATERNION_H

#include "Vector3.h"

namespace MANA3D
{
	class Quaternion
	{
	public:
		union 
		{
			struct 
			{
				float w;					// Holds the real component of the quaternion.
				float x, y, z;				// Holds the complex components of the quaternion.
			};

			float data[4];					// Holds the quaternion data in array form.
		};

		Quaternion();														// Default constructor.
		Quaternion( float, float, float, float );							// Constructs a new quaternion with a given w x y z values.
		~Quaternion();														// default destructor.

		void operator *=( const Quaternion &multiplier );					// Multiplies the quaternion by the given quaternion.

		void Normalize();													// Normalises the quaternion to unit length, making it a valid orientation quaternion.


		/**
		* Adds the given vector to this, scaled by the given amount.
		* This is used to update the orientation quaternion by a rotation
		* and time.
		*
		* @param vector The vector to add.
		*
		* @param scale The amount of the vector to add.
		*/
		void AddScaledVector( const Vector3& vector, float scale );


		///>QuatRotByVector
		void RotateByVector( const Vector3& vector )
		{
			Quaternion q( 0, vector.x, vector.y, vector.z );
			(*this) *= q;
		}


		Quaternion GetNormalized() const;									// Normalize the quaternion.

		float GetRotationAngle() const;										// Returns rotation angle.
		Vector3 GetRotationAxis() const;									// Returns rotation axis.

		Vector3 GetObjectToInertialEulerAngles();							// Returns the euler angles form of this quaternion.
		Vector3 GetInertialToObjectEulerAngles();							// Returns the euler angles form of this quaternion.
	};

	// Global Constant.
	extern const Quaternion QUATERNION_IDENTITY;														// Global identity constant.

	extern float QuaternionDot( const Quaternion& q1, const Quaternion& q2 );							// Returns the dot product of two quaternions.
	extern Quaternion QuaternionCross( const Quaternion& q1, const Quaternion& q2 );					// Returns the quaternion Cross Product of two quaternions.
	extern Quaternion QuaternionSlerp( const Quaternion& from, const Quaternion& to, float t );			// Spherical linear interpolation.
	extern Quaternion QuaternionConjugate( const Quaternion& q );										// Returns quaternion conjugation.
	extern Quaternion QuaternionPow( const Quaternion& q, float exponent );								// Quaternion exponentiation.
}

#endif
