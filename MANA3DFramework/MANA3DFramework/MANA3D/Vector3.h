#ifndef MANA3D_VECTOR3_H
#define MANA3D_VECTOR3_H

namespace MANA3D
{
	class Vector3
	{
	public:
		float x, y, z;															// X, Y and Z components of the vector.

		Vector3();																// Default Constructor.
		Vector3( float, float, float );											// Creates a new vector with given x, y, z components.
		~Vector3();																// Default Destructor.

		Vector3& operator=( const Vector3& );
		Vector3 operator+( const Vector3 rhsV3 ) const;							// Adds two vectors.
		Vector3& operator+=( const Vector3 rhsV3 );								// Adds two vectors.
		Vector3 operator-( const Vector3 rhsV3 ) const;							// Subtracts one vector from another.
		Vector3& operator-=( const Vector3 rhsV3 );								// Subtracts one vector from another.
		Vector3 operator*( float scaler ) const;								// Multiplies a vector by a number.
		Vector3& operator*=( float scaler );									// Multiplies a vector by a number.
		Vector3 operator/( float scaler ) const;								// Divides a vector by a number.
		Vector3& operator/=( float scaler );									// Divides a vector by a number.
		bool operator==( const Vector3 rhsV3 ) const;							// Returns true if the vectors are equal.
		bool operator!=( const Vector3 rhsV3 ) const;							// Returns true if vectors different.
		float operator[]( unsigned index ) const;
		float& operator[]( unsigned index );


		float GetMagnitude() const;												// Returns the length of this vector.
		float GetSquareMagnitude() const;										// Gets the squared magnitude of this vector.

		Vector3 GetNormalized() const;											// Returns this vector with a magnitude of 1.






		/**
		* Calculates and returns a component-wise product of this
		* vector with the given vector.
		*/
		Vector3 componentProduct(const Vector3 &vector) const
		{
			return Vector3(x * vector.x, y * vector.y, z * vector.z);
		}

		/**
		* Performs a component-wise product with the given vector and
		* sets this vector to its result.
		*/
		void componentProductUpdate(const Vector3 &vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
		}

		/**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 vectorProduct(const Vector3 &vector) const
        {
            return Vector3(y*vector.z-z*vector.y, 
                           z*vector.x-x*vector.z, 
                           x*vector.y-y*vector.x);
        }

        /**
         * Updates this vector to be the vector product of its current
         * value and the given vector.
         */
        void operator %=(const Vector3 &vector)
        {
            *this = vectorProduct(vector);
        }

        /**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 operator%(const Vector3 &vector) const
        {
            return Vector3(y*vector.z-z*vector.y, 
                           z*vector.x-x*vector.z, 
                           x*vector.y-y*vector.x);
        }

		/**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        float scalarProduct(const Vector3 &vector) const
        {
            return x*vector.x + y*vector.y + z*vector.z;
        }

        /**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        float operator *(const Vector3 &vector) const
        {
            return x*vector.x + y*vector.y + z*vector.z;
        }        

        /**
         * Adds the given vector to this, scaled by the given amount.
         */
        void addScaledVector(const Vector3& vector, float scale)
        {
            x += vector.x * scale;
            y += vector.y * scale;
            z += vector.z * scale;
        }

		/**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator<(const Vector3& other) const
        {
            return x < other.x && y < other.y && z < other.z;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator>(const Vector3& other) const
        {
            return x > other.x && y > other.y && z > other.z;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator<=(const Vector3& other) const
        {
            return x <= other.x && y <= other.y && z <= other.z;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator>=(const Vector3& other) const
        {
            return x >= other.x && y >= other.y && z >= other.z;
        }

        /** Zero all the components of the vector. */
        void clear() 
        { 
            x = y = z = 0;
        }

        /** Flips all the components of the vector. */
        void invert()
        {
            x = -x;
            y = -y;
            x = -z;
        }

		/** Turns a non-zero vector into a vector of unit length. */
        void normalize()
        {
            float l = GetMagnitude();
            if ( l > 0 ) 
            {
                (*this) *= ((float)1)/l;
            }
        }
	};

	extern const Vector3 VECTOR3_ZERO;											// Shorthand for writing Vector3( 0,  0,  0 ).
	extern const Vector3 VECTOR3_ONE;											// Shorthand for writing Vector3( 1,  1,  1 ).
	extern const Vector3 VECTOR3_UP;											// Shorthand for writing Vector3( 0,  1,  0 ).
	extern const Vector3 VECTOR3_DOWN;											// Shorthand for writing Vector3( 0, -1,  0 ).
	extern const Vector3 VECTOR3_RIGHT;											// Shorthand for writing Vector3( 1,  0,  0 ).
	extern const Vector3 VECTOR3_LEFT;											// Shorthand for writing Vector3(-1,  0,  0 ).
	extern const Vector3 VECTOR3_FORWARD;										// Shorthand for writing Vector3( 0,  0,  1 ).
	extern const Vector3 VECTOR3_BACKWARD;										// Shorthand for writing Vector3( 0,  0, -1 ).

	extern float GetAngle( const Vector3, const Vector3 );						// Returns the angle in degrees between from and to.
	extern float GetDistance( const Vector3, const Vector3 );					// Returns the distance between two vectors. (a-b).magnitude.
	extern int CompareDistance( const Vector3, const Vector3, float );			// Compares distance between two vectors to the given distance (faster).
	extern float CalculateDotProduct( const Vector3, const Vector3 );			// Return the Dot Product of two vectors.
	extern Vector3 CalculateCrossProduct( const Vector3, const Vector3 );		// Returns the Cross Product of two vectors.
	extern Vector3 LerpPoint3D( const Vector3, const Vector3, float );			// Linearly interpolates between two points.
	extern Vector3 LerpVector3( const Vector3, const Vector3, float );			// Linearly interpolates between two vectors.
	extern Vector3 MoveTowards( const Vector3, const Vector3, float );			// Moves a current point in a straight line towards a target point.
}

#endif