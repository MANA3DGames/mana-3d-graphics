#include "MeshRenderer.h"
#include "Transform.h"

#include "Debug.h"
using namespace MANA3D;


MeshRenderer::MeshRenderer() : Renderer()
{
}

MeshRenderer::~MeshRenderer()
{
}


void MeshRenderer::SetMesh( Mesh *m )
{
	mesh = m;
}

void MeshRenderer::Render()
{
	//// ------------- REMOVE THIS!! -------------
	//if ( gameObject->parent != NULL )
	//	return;
	//// ------------- REMOVE THIS!! -------------



	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	
	glScalef( gameObject->transform->GetScale().x, 
			  gameObject->transform->GetScale().y,
			  gameObject->transform->GetScale().z );

	glRotatef( gameObject->transform->GetEulerAngles().x, 1, 0, 0 );
	glRotatef( gameObject->transform->GetEulerAngles().y, 0, 1, 0 );
	glRotatef( gameObject->transform->GetEulerAngles().z, 0, 0, 1 );

	glTranslatef( gameObject->transform->GetPosition().x, 
				  gameObject->transform->GetPosition().y,
				  gameObject->transform->GetPosition().z );

	DrawMesh();


	//// ------------- REMOVE THIS!! -------------
	//// If the gameObject of this renderer has children then it is its responsibility to render them. They are not going to render themself.
	//if ( gameObject->children.size() > 0 )
	//	RenderChildren();
	//// ------------- REMOVE THIS!! -------------


	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

//void MeshRenderer::RenderChildren()
//{
//	for ( std::list<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); it++ )
//	{
//		glMatrixMode( GL_MODELVIEW );
//		glPushMatrix();
//
//
//		glScalef( (*it)->transform->GetScale().x,
//				  (*it)->transform->GetScale().y,
//				  (*it)->transform->GetScale().z );
//
//		glRotatef( (*it)->transform->GetEulerAngles().x, 1, 0, 0 );
//		glRotatef( (*it)->transform->GetEulerAngles().y, 0, 1, 0 );
//		glRotatef( (*it)->transform->GetEulerAngles().z, 0, 0, 1 );
//
//		glTranslatef( (*it)->transform->GetPosition().x,
//					  (*it)->transform->GetPosition().y,
//					  (*it)->transform->GetPosition().z );
//
//
//		if ( (*it)->GetActive() )
//		{
//			MeshRenderer *renderer = (*it)->GetComponent<MeshRenderer>();
//			if ( renderer->GetEnable() )
//				renderer->DrawMesh();
//		}
//
//		glMatrixMode( GL_MODELVIEW );
//		glPopMatrix();
//	}
//}


void MeshRenderer::DrawMesh( void )
{
	DrawMesh( mesh );
}

void MeshRenderer::DrawMesh( Mesh* pMesh )
{
	Material *lastMaterial = NULL;
	
	int faceCount = (int)pMesh->faces.size();

	for ( int i = 0; i < faceCount; i++ )
	{
		Face *face = pMesh->faces[i];

#pragma region
		glLineWidth( 1.5 );
		glDisable( GL_TEXTURE_2D );
		glColor3f( 0.0, 1.0, 0.0 );
		glBegin( GL_LINES );

			glVertex3f(
				pMesh->vertices[face->vertixIndices[0]].x,
				pMesh->vertices[face->vertixIndices[0]].y,
				pMesh->vertices[face->vertixIndices[0]].z );
			glVertex3f(
				pMesh->vertices[face->vertixIndices[1]].x,
				pMesh->vertices[face->vertixIndices[1]].y,
				pMesh->vertices[face->vertixIndices[1]].z );

			glVertex3f(
				pMesh->vertices[face->vertixIndices[1]].x,
				pMesh->vertices[face->vertixIndices[1]].y,
				pMesh->vertices[face->vertixIndices[1]].z );
			glVertex3f(
				pMesh->vertices[face->vertixIndices[2]].x,
				pMesh->vertices[face->vertixIndices[2]].y,
				pMesh->vertices[face->vertixIndices[2]].z );

			glVertex3f(
				pMesh->vertices[face->vertixIndices[2]].x,
				pMesh->vertices[face->vertixIndices[2]].y,
				pMesh->vertices[face->vertixIndices[2]].z );
			glVertex3f(
				pMesh->vertices[face->vertixIndices[0]].x,
				pMesh->vertices[face->vertixIndices[0]].y,
				pMesh->vertices[face->vertixIndices[0]].z );



			glColor3f( 0.0f, 0.0f, 1.0f );
			glVertex3f( face->faceCenter.x, face->faceCenter.y, face->faceCenter.z );
			Vector3 point = face->faceNormal.GetNormalized();
			//point *= 2;
			point.x += face->faceCenter.x;
			point.y += face->faceCenter.y;
			point.z += face->faceCenter.z;
			glVertex3f( point.x, point.y, point.z );


		glColor3f( 1.0, 1.0, 1.0 );
		glEnd();


		glColor3f( 1.0f, 0.0f, 0.0f );
		glPointSize( 5.0f );
		glBegin( GL_POINTS );

			glVertex3f( face->faceCenter.x, face->faceCenter.y, face->faceCenter.z );

			glVertex3f( point.x, point.y, point.z );

		glEnd();
		glColor3f( 1.0f, 1.0f, 1.0f );

#pragma endregion [Edges & Centers]


		Material *material = face->material;

		if ( material != lastMaterial )
		{
			if ( material->alpha < 1.0f )
				continue;

			material->Kd[3] = material->alpha;
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,	(GLfloat*)material->Ka	);
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,	(GLfloat*)material->Kd	);
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,	(GLfloat*)material->Ks	);
			glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,	(GLfloat*)material->Ke	);
			glMaterialf	( GL_FRONT_AND_BACK, GL_SHININESS,	material->shininess		);

			if ( material->diffuseMap != NULL )
			{
				glEnable( GL_TEXTURE_2D );
				glBindTexture( GL_TEXTURE_2D, material->diffuseMap->ID );
			}
			else
				glDisable( GL_TEXTURE_2D );

			lastMaterial = material;
		}

		glBegin( GL_TRIANGLES );
		for ( int v = 0; v < 3; v++ )
		{
			if ( face->UVIndices[v] != -1 )
				glTexCoord2f( pMesh->UVs[face->UVIndices[v]].x,
							  pMesh->UVs[face->UVIndices[v]].y );

			glNormal3d( pMesh->normals[face->normalsIndices[v]].x,
						pMesh->normals[face->normalsIndices[v]].y,
						pMesh->normals[face->normalsIndices[v]].z );

			glVertex3d( pMesh->vertices[face->vertixIndices[v]].x,
						pMesh->vertices[face->vertixIndices[v]].y,
						pMesh->vertices[face->vertixIndices[v]].z );
		}
		glEnd();
	}

	for ( vector<Mesh*>::iterator it = pMesh->submeshes.begin(); it != pMesh->submeshes.end(); it++ )
	{
		if ( (*it) != NULL )
			DrawMesh( *it );
	}
}
