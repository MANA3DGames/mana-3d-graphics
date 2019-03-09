#ifndef MANA3D_MATHF_H
#define MANA3D_MATHF_H

#include <math.h>
#include <float.h>

namespace MANA3D
{
	enum Space3D
	{
		ObjectToInertial = 100,
		InertialToObject = 101,
		ObjectToWorld = 102,
		WorldToObject = 103
	};

	const float PI = 3.14159265358979f;				// The infamous 3.14159265358979... value.
	const float PIOver2 = PI / 2.0f;
	const float oneOverPI = 1.0f / PI;
	const float Deg2Rad = (PI * 2) / 360;			// Degrees-to-radians conversion constant.
	const float Rad2Deg = 360 / (PI * 2);			// Radians-to-degrees conversion constant.


	extern float Lerpf(float from, float to, float f);
}

#endif