#ifndef MANA3D_MATRIX3_H
#define MANA3D_MATRIX3_H

#include "Quaternion.h"

namespace MANA3D
{
	/**
	Holds an inertia tensor, consisting of a 3x3 row-major matrix. This matrix is not padding to produce an aligned structure, 
	since it is most commonly used with a mass (single real) and two damping coefficients to make the 12-element characteristics array of a rigid body.
	*/
	class Matrix3
	{
	public:
		float data[9];															// Holds the tensor matrix data in array form.


	public:
		Matrix3();																		// Default constructor.
		Matrix3( const Vector3 &compOne, 
				 const Vector3 &compTwo, 
				 const Vector3 &compThree );											// Creates a new matrix with the given three vectors making up its columns.
		Matrix3( float c0, float c1, float c2, 
				 float c3, float c4, float c5, 
				 float c6, float c7, float c8 );										// Creates a new matrix with explicit coefficients.
		~Matrix3();																		// Default destructor.


		// Sets the matrix to be a diagonal matrix with the given values along the leading diagonal.
		void SetDiagonal( float a, float b, float c );									

		// Sets the value of the matrix from inertia tensor values.
		void SetInertiaTensorCoeffs( float ix, float iy, float iz,
									 float ixy = 0, float ixz = 0, float iyz = 0 );		

		// Sets the value of the matrix as an inertia tensor of a rectangular block aligned with 
		// the body's coordinate system with the given axis half-sizes and mass.
		void SetBlockInertiaTensor( const Vector3 &halfSizes, float mass );


		/**
		Sets the matrix to be a skew symmetric matrix based on the given vector. 
		The skew symmetric matrix is the equivalent of the vector product. 
		So if a,b are vectors. a x b = A_s b where A_s is the skew symmetric form of a.
		*/
		void SetSkewSymmetric( const Vector3 vector );

		/**
		* Sets the matrix values from the given three vector components.
		* These are arranged as the three columns of the vector.
		*/
		void SetComponents( const Vector3 &compOne, const Vector3 &compTwo, const Vector3 &compThree );


		/**
		* Transform the given vector by this matrix.
		*
		* @param vector The vector to transform.
		*/
		Vector3 operator*( const Vector3 &vector ) const;

		/**
		* Transform the given vector by this matrix.
		*
		* @param vector The vector to transform.
		*/
		Vector3 transform( const Vector3 &vector ) const;


		/**
		* Transform the given vector by the transpose of this matrix.
		*
		* @param vector The vector to transform.
		*/
		Vector3 transformTranspose( const Vector3 &vector ) const;


		/**
		* Gets a vector representing one row in the matrix.
		*
		* @param i The row to return.
		*/
		Vector3 GetRowVector( int i ) const;


		/**
		* Gets a vector representing one axis (i.e. one column) in the matrix.
		*
		* @param i The row to return.
		*
		* @return The vector.
		*/
		Vector3 GetAxisVector( int i ) const;


		/**
		* Sets the matrix to be the inverse of the given matrix.
		*
		* @param m The matrix to invert and use to set this.
		*/
		void SetInverse( const Matrix3 &m );

		/** Returns a new matrix containing the inverse of this matrix. */
		Matrix3 Inverse() const;

		/**
		* Inverts the matrix.
		*/
		void Invert();

		/**
		* Sets the matrix to be the transpose of the given matrix.
		*
		* @param m The matrix to transpose and use to set this.
		*/
		void SetTranspose( const Matrix3 &m );


		/** Returns a new matrix containing the transpose of this matrix. */
		Matrix3 Transpose() const;

		/**
		* Returns a matrix which is this matrix multiplied by the given
		* other matrix.
		*/
		Matrix3 operator*( const Matrix3 &o ) const;


		/**
		* Multiplies this matrix in place by the given other matrix.
		*/
		void operator*=( const Matrix3 &o );

		/**
		* Multiplies this matrix in place by the given scalar.
		*/
		void operator*=( const float scalar );


		/**
		* Does a component-wise addition of this matrix and the given
		* matrix.
		*/
		void operator+=( const Matrix3 &o );


		/**
		* Sets this matrix to be the rotation matrix corresponding to
		* the given quaternion.
		*/
		void SetOrientation( const Quaternion &q );

		/**
		* Interpolates a couple of matrices.
		*/
		static Matrix3 LinearInterpolate( const Matrix3& a, const Matrix3& b, float prop );
	};

}

#endif
