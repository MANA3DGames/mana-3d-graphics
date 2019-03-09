#ifndef MANA3D_TRANSFORM_H
#define MANA3D_TRANSFORM_H

#include "Component.h"
#include "Matrix4X4.h"

namespace MANA3D
{
	class Transform : public Component
	{
		friend class GameObject;

	protected:
		Transform();
		virtual ~Transform();

		Vector3 position;
		Vector3 eulerAngles;
		Vector3 scale;

		Quaternion rotation;

		Matrix4X4 modelMatrix;

	public:
		Vector3 GetPosition();
		Vector3 GetLocalPosition();
		
		Vector3 GetEulerAngles();
		Vector3 GetLocalEulerAngles();

		Vector3 GetScale();
		Vector3 GetLocalScale();

		Quaternion	GetRotation();
		Quaternion	GetLocalRotation();


		Vector3 GetForwardVector();
		Vector3 GetBackwardVector();
		Vector3 GetUpVector();
		Vector3 GetDownVector();
		Vector3 GetRightVector();
		Vector3 GetLeftVector();



		void SetPosition( float x, float y, float z );
		void SetPosition( Vector3 pos );
		void SetLocalPosition( float x, float y, float z );
		void SetLocalPosition( Vector3 pos );

		void SetEulerAngles( float x, float y, float z );
		void SetEulerAngles( Vector3 eulerAngles );

		void SetRotation( Quaternion rotation );
		void SetRotation( Matrix4X4 rMatrix );
		void SeLocaltRotation( float x, float y, float z );
		void SeLocaltRotation( Vector3 rotate );
		void SeLocaltRotation( Quaternion rotation );
		void SeLocaltRotation( Matrix4X4 rMatrix );

		void SetScale( float x, float y, float z );
		void SetScale( Vector3 scale );
		void SetLocalScale( float x, float y, float z );
		void SetLocalScale( Vector3 scale );



		void Translate( Vector3 translate );
		void Translate( Matrix4X4 tMatrix );

		void Rotate( float x, float y, float z );
		void Rotate( Vector3 rotation );
		void Rotate( Quaternion rotation );
		void Rotate( Matrix4X4 rMatrix );

		void Scale( Matrix4X4 sMatrix );


		void ApplyTRS( Matrix4X4 TRS_Matrix );
	};
}

#endif