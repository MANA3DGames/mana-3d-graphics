#ifndef MANA3D_CAMERA_H
#define MANA3D_CAMERA_H

#include "Component.h"
#include "Screen.h"

namespace MANA3D
{
	class Win32App;
	class Camera : public Component
	{
		friend class Win32App;
		friend class GameObject;

	protected:
		Camera();
		virtual ~Camera();

	private:
		GLdouble fieldOfView;
		GLdouble nearClipping;
		GLdouble farClipping;

		Transform *childPointer;

	public:
		void SetProperties( GLdouble fovy, GLdouble nearClip, GLdouble farClip );

		void SetFieldOfView( GLdouble fovy );
		void SetNearClipping( GLdouble nearClip );
		void SetFarClipping( GLdouble farClip );

		GLdouble GetFieldOfView();
		GLdouble GetNearClipping();
		GLdouble GetFarClipping();


	private:
		static Camera *activeCam;

		static void RenderScene();
		static void UpdateGLView();
		static void UpdateGLView( GLint width, GLint height );

	public:
		static Camera* Create();
		static void SetCurrentActive( Camera *cam );

	};
}


#endif
