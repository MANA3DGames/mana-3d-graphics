#include "PhysicsData.h"
#include "Vector3.h"
using namespace MANA3D;


float MANA3D::sleepEpsilon = 0.3f;

void MANA3D::SetSleepEpsilon( float value )
{
	MANA3D::sleepEpsilon = value;
}

float MANA3D::GetSleepEpsilon()
{
	return MANA3D::sleepEpsilon;
}


const Vector3 MANA3D::GRAVITY = Vector3( 0, -9.81f, 0 );
const Vector3 MANA3D::HIGH_GRAVITY = Vector3( 0, -19.62f, 0 );