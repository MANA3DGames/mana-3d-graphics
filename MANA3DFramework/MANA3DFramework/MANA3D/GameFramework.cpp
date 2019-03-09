#include "MANA3DEngine.h"
using namespace MANA3D;


GameObject *go = NULL;
GameObject *child = NULL;
Particle *particle = NULL;

float angle = 0;

void CreateTestCam();
GameObject* CreateGameObjWithMesh( MString fileName, MString goName );
void CreateParticleBody();



void GameFramework::OnCreate()
{
}

void GameFramework::OnStartup()
{
	Debug::Enable();

	CreateTestCam();
	//"Resources/ColoredCube.fbx"
	//"GameObject0"

	go = CreateGameObjWithMesh( MString( "Resources/Transformation_Test/Zero.obj" ), MString( "GO0" ) );
	//go = CreateGameObjWithMesh( MString( "Resources/Transformation_Test/200.obj" ), MString( "GO1" ) );
	//go = CreateGameObjWithMesh( MString( "Resources/Transformation_Test/-200Freeze.obj" ), MString( "GO2" ) );

	//ImportFBX2016( MString( "Resources/Transformation_Test/Zero.obj" ).GetCStr(), nullptr );
}

void GameFramework::OnUpdate()
{
	/*if ( particle && go )
	{
		particle->integrate( Time::GetDeltaTime() );
		go->transform->SetEulerAngles( 0, angle, 0 );
		go->transform->SetPosition( particle->getPosition() );
		angle += 5.0f;
	}*/


	//if ( Input::KeyDown( Key::DEL ) )
	//{
	//	//DestroyAllGameObject();
	//	GameObject::Destroy( go, false );
	//	go = NULL;
	//}

	/*if ( Input::KeyDown( Key::H ) )
		go->SetActive( false );
	if ( Input::KeyDown( Key::S ) )
		go->SetActive( true );*/


	if ( Input::KeyDown( Key::RIGHT_ARROW ) )
	{
		if ( !go ) return;

		Vector3 temp = go->transform->GetEulerAngles();
		temp.y += 5.0f;
		go->transform->SetEulerAngles( temp );
	}
	if ( Input::KeyDown( Key::LEFT_ARROW ) )
	{
		if ( !go ) return;

		Vector3 temp = go->transform->GetEulerAngles();
		temp.y -= 5.0f;
		go->transform->SetEulerAngles( temp );
	}
	if ( Input::KeyDown( Key::UP_ARROW ) )
	{
		if ( !go ) return;
		
		Vector3 temp = go->transform->GetEulerAngles();
		temp.x -= 5.0f;
		go->transform->SetEulerAngles( temp );
	}
	if ( Input::KeyDown( Key::DOWN_ARROW ) )
	{
		if ( !go ) return;

		Vector3 temp = go->transform->GetEulerAngles();
		temp.x += 5.0f;
		go->transform->SetEulerAngles( temp );
	}


	//if ( Input::KeyUp( Key::SPACE ) )
	//{
	//	/*MString finalStr;
	//	MString temp2( "Vector: " );
	//	finalStr = temp2 + go->transform->GetPosition();
	//	Debug::Log( finalStr );*/

	//	MString temp( "temp " );
	//	MString finalStr;// = temp + go->transform->GetPosition();
	//	finalStr.Append( temp );

	//	Debug::Log( VECTOR3_DOWN );
	//}
 }

void GameFramework::OnDestroy()
{
	delete particle;
}





void CreateTestCam()
{
	Camera *mainCam = Camera::Create();
	mainCam->GetGameObject()->transform->SetPosition( -3.0f, 3.0f, 10.0f );
	Camera::SetCurrentActive( mainCam );
}

void CreateChildAndParent()
{
	Mesh *mesh = Mesh::Create();
	ImportFBX( "Resources/ColoredCube.fbx", mesh );

	go = GameObject::Create( "GameObject0" );
	MeshRenderer *renderer = go->AddComponent<MeshRenderer>();
	renderer->SetMesh( mesh );
	

	/*Mesh *childMesh = Mesh::Create();
	ImportFBX( "Resources/ColoredCube.fbx", childMesh );*/

	child = GameObject::Create( "GameObject0" );
	MeshRenderer *rendererChild = child->AddComponent<MeshRenderer>();
	rendererChild->SetMesh( mesh );
	child->transform->SetRotation( go->transform->GetEulerAngles() );
	child->transform->SetPosition( go->transform->GetPosition() );
	child->SetParent( go );
}

void CreateParticleBody()
{
	particle = new Particle();
	particle->setMass( 200.0f ); // 2.0kg
	particle->setVelocity( 0.0f, 22.0f, 10.0f ); // 35m/s
	particle->setAcceleration( 0.0f, -20.0f, 0.0f );
	particle->setDamping( 0.99f );
	particle->setPosition( 0.0f, 0.0f, 0.0f );
	// Clear the force accumulators.
	particle->clearAccumulator();
}

GameObject* CreateGameObjWithMesh( MString fileName, MString goName )
{
	Mesh *mesh = Mesh::Create();
	ImportFBX( fileName.GetCStr(), mesh );

	GameObject *goMesh = GameObject::Create( goName.GetCStr() );
	MeshRenderer *renderer = goMesh->AddComponent<MeshRenderer>();
	renderer->SetMesh( mesh );

	return goMesh;
}
