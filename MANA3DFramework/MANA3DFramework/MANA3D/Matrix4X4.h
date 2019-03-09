#ifndef MANA3D_MATRIX4X4_H
#define MANA3D_MATRIX4X4_H

#include "Math3D.h"

namespace MANA3D
{
	class Matrix4X4
	{
	public:
		float m11, m12, m13, m14,
			  m21, m22, m23, m24,
			  m31, m32, m33, m34,
			  m41, m42, m43, m44;

	public:
		Matrix4X4();																// Default Constructor.
		Matrix4X4( float, float, float, float,
				   float, float, float, float, 
				   float, float, float, float,
				   float, float, float, float );
		Matrix4X4( const Vector3& );												// Constructs a matrix wiht a given euler angles.
		//Matrix4X4( const Vector3&, const Vector3&, const Vector3& );				// Constructs a matrix wiht given translatation, orientation and scale.
		Matrix4X4( const Quaternion&, Space3D );									// Constructs a matrix with a given quaternion.

		~Matrix4X4();


		void FromInertialToObjectQuaternion( const Quaternion& );					// Setup a matrix from a quaternion that performs the rotation from Inertial To Object space.
		void FromObjectToInertialQuaternion( const Quaternion& );					// Setup a matrix from a quaternion that performs the rotation from Object To Inertial space.

		Vector3 RotateVector3FromInertialToObject( const Vector3& ) const;			// Rotate a vector3 from inertial to object space.
		Vector3 RotateVector3FromObjectToInertial( const Vector3& ) const;			// Rotate a vector3 from object to inertial space.

		void ZeroTranslation();														// Zero the first 3 elements of the fourth row of the matrix.
		void SetTranslation( const Vector3& );										// Sets the translation portion of the matrix. 
		void MakeTranslationOnly( const Vector3& );									// Sets the translation protion of the matrix and sets the linear transformation portion to identity.

		void SetupLocalToParent( const Vector3& pos, const Vector3& );
		void SetupLocalToParent( const Vector3& pos, const Matrix4X4& );
		void SetupParentToLocal( const Vector3& pos, const Vector3& );
		void SetupParentToLocal( const Vector3& pos, const Matrix4X4& );

		void Rotate( const Quaternion& );
		void RotateX( float );
		void RotateY( float );
		void RotateZ( float );
		void RotateAxis( const Vector3&, float );
		

		void Scale( const Vector3& );
		void ScaleAlongAxis( const Vector3&, float );

		void ShearX( float, float );											// Shears Y and Z using X.
		void ShearY( float, float );											// Shears X and Z using Y.
		void ShearZ( float, float );											// Shears X and Y using Z.

		void Project( const Vector3& );

		void Reflect( const Vector3& );
		void ReflectX( float );
		void ReflectY( float );
		void ReflectZ( float );


		Vector3 GetEulerAngles();
	};

	extern const Matrix4X4 MATRIX4X4_IDENTITY;

	extern Vector3 operator*( const Vector3&, const Matrix4X4& );
	extern Vector3 operator*=( Vector3&, const Matrix4X4& );

	extern Matrix4X4 operator*( const Matrix4X4&, const Matrix4X4& );
	extern Matrix4X4 operator*=( Matrix4X4&, const Matrix4X4& );

	extern float Matrix4X4Determinant( const Matrix4X4& );
	extern Matrix4X4 Matrix4X4Inverse( const Matrix4X4& );
	extern Vector3 Matrix4X4Translation( const Matrix4X4& );

	extern Vector3 Matrix4X4ObjectToWorldEulerAngles( const Matrix4X4& );
	extern Vector3 Matrix4X4WorldToObjectEulerAngles( const Matrix4X4& );



	extern Vector3 Matrix4X4PositionFromParentToLocal( const Matrix4X4& );		// This function doesn't make sense for non-rigid transformation (Assuming the given matrix is orthogonal)
	extern Vector3 Matrix4X4PositionFromLocalToParent( const Matrix4X4& );
}

#endif