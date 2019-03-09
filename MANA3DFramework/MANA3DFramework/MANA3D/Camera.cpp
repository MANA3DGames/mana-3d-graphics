#include "Camera.h"
#include "Renderer.h"
#include "Win32App.h"
#include "Transform.h"
using namespace MANA3D;


Camera::Camera()
{
	fieldOfView = 45.0;
	nearClipping = 0.1;
	farClipping = 1000.0;

	GameObject *child = GameObject::Create( "camPointer" );
	childPointer = child->transform;
	childPointer->SetPosition( 0, 0, 100 );
	child->SetParent( gameObject );
}

Camera::~Camera()
{
}


void Camera::SetProperties( GLdouble fovy, GLdouble nearClip, GLdouble farClip )
{
	fieldOfView = fovy;
	nearClipping = nearClip;
	farClipping = farClip;
	
	if ( this == activeCam )
		UpdateGLView();
}

void Camera::SetFieldOfView( GLdouble fovy )
{
	fieldOfView = fovy;
	
	if ( this == activeCam )
		UpdateGLView();
}

void Camera::SetNearClipping( GLdouble nearClip )
{
	nearClipping = nearClip;
	
	if ( this == activeCam )
		UpdateGLView();
}

void Camera::SetFarClipping( GLdouble farClip )
{
	farClipping = farClip;
	
	if ( this == activeCam )
		UpdateGLView();
}


GLdouble Camera::GetFieldOfView()
{
	return fieldOfView;
}

GLdouble Camera::GetNearClipping()
{
	return nearClipping;
}

GLdouble Camera::GetFarClipping()
{
	return farClipping;
}



Camera* Camera::activeCam = NULL;


void Camera::UpdateGLView()
{
	GLint width, height;
	Screen::GetWidthAndHeight( Win32App::GetHWND(), width, height );
	UpdateGLView( width, height );
}

void Camera::UpdateGLView( GLint width, GLint height )
{
	// Make sure activeCam was set.
	if ( activeCam == NULL ) return;

	// Prevent a divide by zero error.
	if ( height == 0 )
		height = 1;

	// Set view port.
	glViewport( 0, 0, width, height );

	// Select the projection matrix.
	glMatrixMode( GL_PROJECTION );

	// Reset projection matrix.
	glLoadIdentity();

	// Calcualte aspect ratio for the OpenGl window.
	GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

	// Set camera perspective.
	gluPerspective( activeCam->fieldOfView, aspectRatio, activeCam->nearClipping, activeCam->farClipping );

	// Select the ModelView Matrix.
	glMatrixMode( GL_MODELVIEW );

	// Reset the ModelView matrix.
	glLoadIdentity();
}

void Camera::RenderScene()
{
	// Make sure activeCam was set.
	if ( activeCam == NULL ) return;

	// Clear the Screen and the Depth Buffer.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset the view.
	glLoadIdentity();

	// Get current active camera position.
	Vector3 camPos = activeCam->gameObject->transform->GetPosition();

	// *** Determine where the camera's position and view is going to be ***
	gluLookAt( camPos.x, camPos.y, camPos.z,			// Position: Where are we standing at right now?
			   0, 0, 0,									// View: We are looking at the center of the axis (0, 0, 0).
			   0, 1, 0 );								// Up Vector: We say that up is (0, 1, 0).

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );


	/////////////////////////////////////////////////////////////////////////
	//// Create light components.
	//GLfloat ambientLight[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat diffuseLight[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//GLfloat position[]		= { 5.0f, 5.0f, 5.0f, 0.0f };

	//// Assign created components to GL_LIGHT0.
	//glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	//glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	////glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight );
	//glLightfv( GL_LIGHT0, GL_POSITION, position );

	//glEnable( GL_LIGHT0 );
	//glEnable( GL_LIGHTING );

	////glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	////glEnable( GL_COLOR_MATERIAL );
	/////////////////////////////////////////////////////////////////////////


	Renderer::RenderAll();

	// Swap the backbuffers to the foreground
	SwapBuffers( wglGetCurrentDC() );
}


Camera* Camera::Create()
{
	GameObject *camGO = GameObject::Create( "Camera" );
	GameObject *camPointer = GameObject::Create( "CamPointer" );
	
	Camera *camera = camGO->AddComponent<Camera>();

	return camera;
}

void Camera::SetCurrentActive( Camera *cam )
{
	activeCam = cam;
	UpdateGLView();
}