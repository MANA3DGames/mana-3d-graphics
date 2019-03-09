#ifndef MANA3D_MATRIX4_H
#define MANA3D_MATRIX4_H

#include "Quaternion.h"

namespace MANA3D
{
	/**
	Holds a transform matrix, consisting of a rotation matrix and a position. 
	The matrix has 12 elements, it is assumed that the remaining four are (0,0,0,1); producing a homogenous matrix.
	*/
	class Matrix4
	{
	public:
		float data[12];															// Holds the transform matrix data in array form.

	public:
		Matrix4();																// Default construtor; creates identity matrix.
		~Matrix4();																// Default destructor.


		void SetDiagonal( float a, float b, float c );							// Sets the matrix to be a diagonal matrix with the given coefficients.

		Matrix4 operator*( const Matrix4 &o ) const;							// Returns a matrix which is this matrix multiplied by the given other matrix.
		Vector3 operator*( const Vector3 &vector ) const;						// Transform the given vector by this matrix.

		Vector3 transform( const Vector3 &vector ) const;						// Transform the given vector by this matrix.

		float GetDeterminant() const;											// Returns the determinant of the matrix.

		void SetInverse( const Matrix4 &m );									// Sets the matrix to be the inverse of the given matrix.

		Matrix4 Inverse() const;												// Returns a new matrix containing the inverse of this matrix.

		void Invert();															// Inverts the matrix.

		Vector3 transformDirection( const Vector3 &vector ) const;				// Transform the given direction vector by this matrix.

		/**
		* Transform the given direction vector by the
		* transformational inverse of this matrix.
		///<TransformDirection
		*
		* @note This function relies on the fact that the inverse of
		* a pure rotation matrix is its transpose. It separates the
		* translational and rotation components, transposes the
		* rotation, and multiplies out. If the matrix is not a
		* scale and shear free transform matrix, then this function
		* will not give correct results.
		*
		* @note When a direction is converted between frames of
		* reference, there is no translation required.
		*
		* @param vector The vector to transform.
		///>TransformDirection
		*/
		Vector3 transformInverseDirection( const Vector3 &vector ) const;

		/**
		* Transform the given vector by the transformational inverse
		* of this matrix.
		///<TransformInverse
		*
		* @note This function relies on the fact that the inverse of
		* a pure rotation matrix is its transpose. It separates the
		* translational and rotation components, transposes the
		* rotation, and multiplies out. If the matrix is not a
		* scale and shear free transform matrix, then this function
		* will not give correct results.
		*
		* @param vector The vector to transform.
		///>TransformInverse
		*/
		Vector3 transformInverse( const Vector3 &vector ) const;    

		/**
		* Gets a vector representing one axis (i.e. one column) in the matrix.
		*
		* @param i The row to return. Row 3 corresponds to the position
		* of the transform matrix.
		*
		* @return The vector.
		*/
		Vector3 GetAxisVector( int i ) const;

		/**
		* Sets this matrix to be the rotation matrix corresponding to
		* the given quaternion.
		*/
		void SetOrientationAndPos( const Quaternion &q, const Vector3 &pos );

		/**
		* Fills the given array with this transform matrix, so it is
		* usable as an open-gl transform matrix. OpenGL uses a column
		* major format, so that the values are transposed as they are
		* written.
		*/
		void FillGLArray( float array[16] ) const;
	};
}

#endif
