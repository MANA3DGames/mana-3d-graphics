#include "Transform.h"
using namespace MANA3D;


Transform::Transform() : Component()
{
	position	= VECTOR3_ZERO;
	eulerAngles = VECTOR3_ZERO;
	scale		= VECTOR3_ONE;

	rotation = QUATERNION_IDENTITY;

	modelMatrix = MATRIX4X4_IDENTITY;
}

Transform::~Transform()
{
}


Vector3 Transform::GetPosition()
{
	return position;
}

Vector3 Transform::GetLocalPosition()
{
	// Check if there is a parent for this transform.
	if ( gameObject->parent != NULL )
	{
		// Calculate the local position in regard to the parent.
		return gameObject->transform->GetLocalPosition() - position;
	}
	// There is no parent for this transform.
	else
		// Return world position.
		return position;
}

Vector3 Transform::GetEulerAngles()
{
	return eulerAngles;
}

Vector3 Transform::GetLocalEulerAngles()
{
	return eulerAngles;
}

Vector3 Transform::GetScale()
{
	return scale;
}

Vector3 Transform::GetLocalScale()
{
	return scale;
}


Quaternion	Transform::GetRotation()
{
	return rotation;
}

Quaternion	Transform::GetLocalRotation()
{
	return rotation;
}


Vector3 Transform::GetForwardVector()
{
	// Keep track of it from the beginning!!!!

	///??????????????????
	return VECTOR3_ZERO;
}

Vector3 Transform::GetBackwardVector()
{
	///??????????????????
	return VECTOR3_ZERO;
}

Vector3 Transform::GetUpVector()
{
	///??????????????????
	return VECTOR3_ZERO;
}

Vector3 Transform::GetDownVector()
{
	///??????????????????
	return VECTOR3_ZERO;
}

Vector3 Transform::GetRightVector()
{
	///??????????????????
	return VECTOR3_ZERO;
}

Vector3 Transform::GetLeftVector()
{
	///??????????????????
	return VECTOR3_ZERO;
}


void Transform::SetPosition( float x, float y, float z )
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SetPosition( Vector3 pos )
{
	position = pos;
}

void Transform::SetLocalPosition( float x, float y, float z )
{

}

void Transform::SetLocalPosition( Vector3 translate )
{

}


void Transform::SetEulerAngles( float x, float y, float z )
{
	eulerAngles.x = x;
	eulerAngles.y = y;
	eulerAngles.z = z;
}

void Transform::SetEulerAngles( Vector3 euler )
{
	eulerAngles = euler;
}


void Transform::SetRotation( Quaternion rotation )
{

}

void Transform::SetRotation( Matrix4X4 rMatrix )
{

}


void Transform::SeLocaltRotation( float x, float y, float z )
{

}

void Transform::SeLocaltRotation( Vector3 rotate )
{

}

void Transform::SeLocaltRotation( Quaternion rotation )
{

}

void Transform::SeLocaltRotation( Matrix4X4 rMatrix )
{

}


void Transform::SetScale( float x, float y, float z )
{

}

void Transform::SetScale( Vector3 scale )
{

}

void Transform::SetLocalScale( float x, float y, float z )
{

}

void Transform::SetLocalScale( Vector3 scale )
{

}



void Transform::Translate( Vector3 translate )
{

}

void Transform::Translate( Matrix4X4 tMatrix )
{

}


void Transform::Rotate( float x, float y, float z )
{

}

void Transform::Rotate( Vector3 rotation )
{

}

void Transform::Rotate( Quaternion rotation )
{

}

void Transform::Rotate( Matrix4X4 rMatrix )
{

}


void Transform::Scale( Matrix4X4 sMatrix )
{

}


void Transform::ApplyTRS( Matrix4X4 TRS_Matrix )
{

}

