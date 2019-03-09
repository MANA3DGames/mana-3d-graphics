#include "Mesh.h"
#include "FBXImporter.h"
using namespace MANA3D;

#include <algorithm>


Mesh::Mesh() :
	vertices( NULL ), normals( NULL ), UVs( NULL ), 
	verticesCount( 0 ), normalsCount( 0 ), UVsCount( 0 )
{
}

Mesh::~Mesh()
{
	delete[] vertices;
	delete[] normals;
	delete[] UVs;

	for ( int i = 0; i < (int)faces.size(); i++ )
		delete faces[i];
	faces.clear();

	for ( int i = 0; i < (int)frames.size(); i++ )
		delete frames[i];
	frames.clear();
}


void Mesh::AddFace( Face* face )
{
	faces.push_back( face );
}

void Mesh::AddFace( int v1,  int v2,  int v3,
					int n1,  int n2,  int n3,
					int uv1, int uv2, int uv3,
					Material *mat )
{
	Face *face = new Face( v1, v2, v3, n1, n2, n3, uv1, uv2, uv3, vertices );
	face->material = mat;
	faces.push_back( face );
}


vector<Face*>& Mesh::GetFaces( void )
{
	return faces;
}

Vector3* Mesh::GetVertices( void )
{
	return vertices;
}

Vector3* Mesh::GetNormals( void )
{
	return normals;
}

Vector3* Mesh::GetUVs( void )
{
	return UVs;
}


vector<Mesh*> Mesh::allMeshes;

Mesh* Mesh::Create()
{
	Mesh *mesh = new Mesh(); 
	allMeshes.push_back( mesh );
	return mesh;
}

void Mesh::Destroy( Mesh *mesh )
{
	allMeshes.erase( std::remove(allMeshes.begin(), allMeshes.end(), mesh ), allMeshes.end() );
	delete mesh;
}

void Mesh::DestroyAll()
{
	while ( !allMeshes.empty() )
	{
		Mesh *temp = *allMeshes.begin();
		allMeshes.erase( allMeshes.begin() );
		delete temp;
	}

	allMeshes.clear();
}




// ************ Material ************
vector<Material*> Material::allMaterials;

Material* Material::Create()
{
	Material *mat = new Material();
	allMaterials.push_back(mat);
	return mat;
}

void Material::Destroy( Material *material )
{
	allMaterials.erase( std::remove( allMaterials.begin(), allMaterials.end(), material), allMaterials.end() );
	delete material;
}

void Material::DestroyAll()
{
	while ( !allMaterials.empty() )
	{
		Material* temp = *allMaterials.begin();
		allMaterials.erase( allMaterials.begin() );
		delete temp;
	}

	allMaterials.clear();
}