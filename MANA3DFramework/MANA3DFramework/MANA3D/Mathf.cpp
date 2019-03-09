#include "Mathf.h"
using namespace MANA3D;


float MANA3D::Lerpf( float from, float to, float f )
{
	return from + f * ( to - from );
}