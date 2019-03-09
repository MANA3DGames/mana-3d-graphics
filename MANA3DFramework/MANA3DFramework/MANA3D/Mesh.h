#ifndef MANA3D_MESH_H
#define MANA3D_MESH_H

#include <fbxsdk.h>
using namespace FBXSDK_NAMESPACE;

#include <fstream>
#include <iostream>
using std::ifstream;

#include <sstream>
using std::istringstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Texture.h"
#include "Vector3.h"

namespace MANA3D
{
	const int TRIANGLE_VERTEX_COUNT = 3;

	struct Material
	{
		float Ka[4];
		float Kd[4];
		float Ks[4];
		float Ke[4];

		float shininess;
		float alpha;
		float illum;

		Texture *ambientMap;
		Texture *diffuseMap;
		Texture *specularMap;
		Texture *emissionMap;
		Texture *shininessMap;
		Texture *transparencyMap;
		Texture *bumpMap;

		string name;

	private:
		Material()
		{
			float defaultAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			float defaultDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			float defaultSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			float defaultEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

			memcpy( Ka, defaultAmbient, sizeof(float) * 4 );
			memcpy( Kd, defaultDiffuse, sizeof(float) * 4 );
			memcpy( Ks, defaultSpecular, sizeof(float) * 4 );
			memcpy( Ke, defaultEmission, sizeof(float) * 4 );

			ambientMap = NULL;
			diffuseMap = NULL;
			specularMap = NULL;
			emissionMap = NULL;
			shininessMap = NULL;
			transparencyMap = NULL;
			bumpMap = NULL;

			shininess = 2.0f;
			alpha = 1.0f;
			illum = 1.0f;
		}

		~Material()
		{
		}

	private:
		static vector<Material*> allMaterials;

	public:
		static Material* Create();
		static void Destroy( Material* );
		static void DestroyAll();
	};

	struct Face
	{
		int vertixIndices[TRIANGLE_VERTEX_COUNT];
		int normalsIndices[TRIANGLE_VERTEX_COUNT];
		int UVIndices[TRIANGLE_VERTEX_COUNT];

		Material *material;

		Vector3 faceCenter;
		Vector3 faceNormal;

		Face( int v1,  int v2,  int v3,
			  int n1,  int n2,  int n3,
			  int uv1, int uv2, int uv3,
			  Vector3 *vertices )
		{
			vertixIndices[0] = v1;
			vertixIndices[1] = v2;
			vertixIndices[2] = v3;

			normalsIndices[0] = n1;
			normalsIndices[1] = n2;
			normalsIndices[2] = n3;

			UVIndices[0] = uv1;
			UVIndices[1] = uv2;
			UVIndices[2] = uv3;

			for ( int i = 0; i < TRIANGLE_VERTEX_COUNT; i++ )
				faceCenter += vertices[ vertixIndices[i] ];

			faceCenter /= (float)TRIANGLE_VERTEX_COUNT;

			faceNormal = CalculateCrossProduct( ( vertices[v1] - vertices[v2] ).GetNormalized(), ( vertices[v1] - vertices[v3] ).GetNormalized() );
			
			material = NULL;
		}
	};

	struct Frame
	{
		FbxMatrix	*bones;
		double		*weights;
	};


	class Mesh
	{
	private:
		Mesh();
		virtual ~Mesh();

	public:
		void AddFace( Face *face );
		void AddFace( int v1,  int v2,  int v3,
					  int n1,  int n2,  int n3,
					  int uv1, int uv2, int uv3,
					  Material *mat );


		vector<Face*>&	GetFaces( void );
		Vector3*		GetVertices( void );
		Vector3*		GetNormals( void );
		Vector3*		GetUVs( void );


	public:
		Vector3 *vertices;
		Vector3 *normals;
		Vector3 *UVs;

		int verticesCount;
		int normalsCount;
		int UVsCount;

		vector<Face*>		faces;
		vector<Frame*>		frames;
		vector<Material*>	materials;
		vector<Mesh*>		submeshes;

		float frameStart;
		float frameEnd;
		float frame;

		string fileName;
		string	name;

		double	*matrix;


	private:
		static vector<Mesh*> allMeshes;
	
	public:
		static Mesh* Create();
		static void Destroy( Mesh* );
		static void DestroyAll();
	};
}

#endif
