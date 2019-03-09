#include "Debug.h"

#include "FBXImporter.h"
#include "TextureImporter.h"
using namespace MANA3D;

#include <map>
using std::map;


// initialize cache start and stop time
FbxTime mCache_Start = FBXSDK_TIME_INFINITE;
FbxTime mCache_Stop = FBXSDK_TIME_MINUS_INFINITE;

// Four floats for every position.
const int VERTEX_STRIDE = 4;
// Three floats for every normal.
const int NORMAL_STRIDE = 3;
// Two floats for every UV.
const int UV_STRIDE = 2;

const float frameRate = 30.0f;



// Init Fbx Importer; Use a custom memory allocator.
void MANA3D::InitFBXImporter()
{
	// Use a custom memory allocator
	FbxSetMallocHandler(	MyMemoryAllocator::MyMalloc		);
	FbxSetReallocHandler(	MyMemoryAllocator::MyRealloc	);
	FbxSetFreeHandler(		MyMemoryAllocator::MyFree		);
	FbxSetCallocHandler(	MyMemoryAllocator::MyCalloc		);
}

// Import fbx file.
bool MANA3D::ImportFBX( string fileName, Mesh *meshBuffer )
{
	// Check if the file name is an empty string. 
	if ( fileName.empty() ) return false;

	// Create the FBX SDK manager.
	FbxManager *fbxManager = FbxManager::Create();

	// Create an IOSettings object.
	FbxIOSettings *ios = FbxIOSettings::Create( fbxManager, "IOSRoot" );
	fbxManager->SetIOSettings( ios );

	// Configure the FbxIOSettings object.
	( *( fbxManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_MATERIAL,			true  );
	( *( fbxManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_TEXTURE,			true  );
	( *( fbxManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_ANIMATION,		true  );
	( *( fbxManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_GLOBAL_SETTINGS,	true  );
	( *( fbxManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_LINK,				false );
	( *( fbxManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_SHAPE,			false );
	( *( fbxManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_GOBO,				false );
	
	// Create an importer.
	FbxImporter *fbxImporter = FbxImporter::Create( fbxManager, "" );

	// Initialize the importer.
	bool importStatus = fbxImporter->Initialize( fileName.data(), -1, fbxManager->GetIOSettings() );

	// Error Handling.
	if ( !importStatus )
	{
		MessageBoxA( NULL, fbxImporter->GetStatus().GetErrorString(), "Error returned", NULL );
		return false;
	}

	// Create a new scene so it can be populated by the imported file.
	FbxScene *scene = FbxScene::Create( fbxManager, "" );

	// Import the contents of the file into the scene.
	fbxImporter->Import( scene );

	// Convert Axis System to what is used in this example, if needed
    FbxAxisSystem SceneAxisSystem = scene->GetGlobalSettings().GetAxisSystem();
    FbxAxisSystem OurAxisSystem( FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded );
    if( SceneAxisSystem != OurAxisSystem )
		OurAxisSystem.ConvertScene( scene );

    // Convert Unit System to what is used in this example, if needed
    FbxSystemUnit SceneSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
    if( SceneSystemUnit.GetScaleFactor() != 1.0 )
		//The unit in this example is centimeter.
        FbxSystemUnit::cm.ConvertScene( scene );

	FbxArray<FbxString*> mAnimStackNameArray;
    // Get the list of all the animation stack.
    scene->FillAnimStackNameArray( mAnimStackNameArray );

    // Convert mesh, NURBS and patch into triangle mesh
	FbxGeometryConverter lGeomConverter( fbxManager );
	lGeomConverter.Triangulate( scene, /*replace*/true );


	//FbxAnimLayer *mCurrentAnimLayer = NULL;
	//bool mSupportVBO = true;
 //   // Bake the scene for one frame
 //   LoadCacheRecursive( scene, mCurrentAnimLayer, fileName.data(), mSupportVBO );

    //// Convert any .PC2 point cache data into the .MC format for vertex cache deformer playback.
    //PreparePointCacheData( mScene, mCache_Start, mCache_Stop );

    //// Get the list of pose in the scene
    //FillPoseArray( mScene, mPoseArray );

    //// Initialize the frame period.
    //mFrameTime.SetTime( 0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode() );


	// Load textures data.
	LoadTextures( scene, fileName.data() );

	meshBuffer->fileName = fileName;

	// Get the root node of the scene.
	FbxNode *node = scene->GetRootNode();

	// Create a new mesh buffer to hold all the data that we are going to extract from the original fbxMesh.
	if ( node )
	{
		// load its contents recursively.
		LoadMeshContent( scene, node, meshBuffer );
	}

	// Unload textures data.
	UnloadTextures( scene );

	// The file has been imported; we can get rid of the importer.
	fbxImporter->Destroy();
	scene->Destroy();
	fbxManager->Destroy();


	return true;
}

// Load the textures.
void MANA3D::LoadTextures( FbxScene *pScene, const char *pFbxFileName )
{
    // Load the textures into GPU, only for file texture now
    const int lTextureCount = pScene->GetTextureCount();
    for ( int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex )
    {
        FbxTexture *lTexture = pScene->GetTexture( lTextureIndex );
        FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>( lTexture );
        if ( lFileTexture && !lFileTexture->GetUserDataPtr() )
        {
            // Try to load the texture from absolute path
            const FbxString lFileName = lFileTexture->GetFileName();
			
            //// Only TGA textures are supported now.
            //if (lFileName.Right(3).Upper() != "TGA")
            //{
            //    FBXSDK_printf("Only TGA textures are supported now: %s\n", lFileName.Buffer());
            //    continue;
            //}

            //GLuint lTextureObject = 0;
			Texture *texture = Texture::Create();
			texture->name = lTexture->GetName();

			bool lStatus = texture->Import( lFileName );

            const FbxString lAbsFbxFileName = FbxPathUtils::Resolve( pFbxFileName );
            const FbxString lAbsFolderName = FbxPathUtils::GetFolderName( lAbsFbxFileName );
            if ( !lStatus )
            {
                // Load texture from relative file name (relative to FBX file)
                const FbxString lResolvedFileName = FbxPathUtils::Bind( lAbsFolderName, lFileTexture->GetRelativeFileName() );
               
				lStatus = texture->Import( lResolvedFileName );
            }

            if ( !lStatus )
            {
                // Load texture from file name only (relative to FBX file)
                const FbxString lTextureFileName = FbxPathUtils::GetFileName( lFileName );
                const FbxString lResolvedFileName = FbxPathUtils::Bind( lAbsFolderName, lTextureFileName );

				lStatus = texture->Import( lResolvedFileName );
            }

            if ( !lStatus )
            {
                FBXSDK_printf( "Failed to load texture file: %s\n", lFileName.Buffer() );
                continue;
            }

            if ( lStatus )
            {
                GLuint *lTextureName = new GLuint( texture->ID );
                lFileTexture->SetUserDataPtr( lTextureName );
            }
        }
    }
}

// Load a node contents.
bool MANA3D::LoadMeshContent( FbxScene *scene, FbxNode *node, Mesh *meshBuffer )
{
	bool isMeshNode = false;

	// Check if this component is a mesh.
	if ( node->GetNodeAttribute() &&
		 node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh )
	{
		// This is an actual mesh node.
		isMeshNode = true;

		// Get the FbxMesh.
		FbxMesh *fbxMesh = (FbxMesh*)node->GetNodeAttribute();

		// Initialize mesh buffer data members.
		meshBuffer->name = node->GetName();
		meshBuffer->matrix = new double[16];
		memcpy( meshBuffer->matrix, (double*)node->EvaluateGlobalTransform(), sizeof(double) * 16 );
		

		// Fill vertices data in the meshBuffer from the original fbxMesh.
		FbxVector4 *controlPoints = fbxMesh->GetControlPoints();
		meshBuffer->verticesCount = fbxMesh->GetControlPointsCount();
		meshBuffer->vertices = new Vector3[meshBuffer->verticesCount];
		
		for ( int i = 0; i < meshBuffer->verticesCount; i++ )
		{
			meshBuffer->vertices[i] = Vector3( (float)controlPoints[i][0], (float)controlPoints[i][1], (float)controlPoints[i][2] );
		}


		// Fill normals data in the meshBuffer from the original fbxMesh.
		fbxsdk::FbxLayerElementArrayTemplate<FbxVector4> *normalsArray = NULL;
		fbxMesh->GetNormals( &normalsArray );

		if ( normalsArray != NULL )
		{
			meshBuffer->normalsCount = normalsArray->GetCount();
			meshBuffer->normals = new Vector3[meshBuffer->normalsCount];
		
			for ( int i = 0; i < meshBuffer->normalsCount; i++ )
			{
				FbxVector4 normal = normalsArray->GetAt( i );
				meshBuffer->normals[i] = Vector3( (float)normal[0], (float)normal[1], (float)normal[2] );
			}
		}
		

		// Fill UVs data in the meshBuffer from the original fbxMesh.
		fbxsdk::FbxLayerElementArrayTemplate<FbxVector2> *uvsArray = NULL;
		fbxMesh->GetTextureUV( &uvsArray );

		if ( uvsArray != NULL )
		{
			meshBuffer->UVsCount = uvsArray->GetCount();
			meshBuffer->UVs = new Vector3[meshBuffer->UVsCount];

			for ( int i = 0; i < meshBuffer->UVsCount; i++ )
			{
				FbxVector2 uv = uvsArray->GetAt(i);
				meshBuffer->UVs[i] = Vector3( (float)uv[0], (float)uv[1], (float)uv[2] );
			}
		}


		// Create a map that holds all the materials that will be created associated with the original fbxMaterial index.
		map<int, Material*> materialsMap;
		for ( int i = 0; i < node->GetMaterialCount(); i++ )
		{
			Material *material = Material::Create();
			FbxSurfaceMaterial *fbxMaterial = node->GetMaterial( i );
			BakeMaterial( fbxMaterial, material );
			materialsMap.insert( std::pair<int, Material*>( i, material ) );
		}

		// Fill faces data in the meshBuffer from the original fbxMesh.
		FbxInt polygonCount = fbxMesh->GetPolygonCount();

		for ( int lPolygonIndex = 0; lPolygonIndex < polygonCount; lPolygonIndex++ )
		{
			Material *material = NULL;
			FbxLayerElementArrayTemplate<int> *lMaterialIndices = &fbxMesh->GetElementMaterial()->GetIndexArray();
			if ( lMaterialIndices )
			{
				// Get the material index for the face which has the lPolygonIndex.
				const int lMaterialIndex = lMaterialIndices->GetAt( lPolygonIndex );
				material = materialsMap.at( lMaterialIndex );
			}

			// Get PolygonSize, which should be always 3 as already used lGeomConverter.Triangulate to convert the mesh into triangles.
			FbxInt polygonSize = fbxMesh->GetPolygonSize( lPolygonIndex );	

			/*Debug::Log( "polygonSize: " );
			Debug::Log( polygonSize );*/

			// Triangulation Code.
			FbxInt index1 = fbxMesh->GetPolygonVertex( lPolygonIndex, 0 );
			FbxInt index2 = fbxMesh->GetPolygonVertex( lPolygonIndex, 1 );
			FbxInt index3 = fbxMesh->GetPolygonVertex( lPolygonIndex, 2 );

			FbxInt uv1 = fbxMesh->GetTextureUVIndex( lPolygonIndex, 0 );
			FbxInt uv2 = fbxMesh->GetTextureUVIndex( lPolygonIndex, 1 );
			FbxInt uv3 = fbxMesh->GetTextureUVIndex( lPolygonIndex, 2 );

			meshBuffer->AddFace( 
				index1, index2, index3,
				index1, index2, index3,
				uv1, uv2, uv3,
				material );
		}

		Debug::Log( "Face Count: " );
		Debug::Log( (int)meshBuffer->faces.size() );

		// We don't need the materials map any more.
		materialsMap.clear();
	}

	// Load content for children as well.
	for ( int i = 0; i < node->GetChildCount(); i++ )
	{
		Mesh *nextBuffer = NULL;

		// Check if the parent node is an actual mesh node.
		if ( isMeshNode )
		{
			// Check if the current child is also an actual mesh node.
			if ( node->GetChild(i)->GetNodeAttribute() &&
				 node->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh )
			{
				// Create a mesh for the current child node.
				nextBuffer = Mesh::Create();
				nextBuffer->fileName = meshBuffer->fileName;

				// Add it to the current parent mesh
				meshBuffer->submeshes.push_back( nextBuffer );
			}
		}
		// The parent node is not a mesh node so that's mean the mesh buffer is still empty
		// So we need to make sure it will be fill in the next iteration.
		else
			nextBuffer = meshBuffer;

		LoadMeshContent( scene, node->GetChild(i), nextBuffer );
	}

	return true;
}

// Back FbxMaterial into our Material data type.
bool MANA3D::BakeMaterial( const FbxSurfaceMaterial *pMaterial, Material *material )
{
	Texture *emissionTex = NULL;
	const FbxDouble3 lEmissive = GetMaterialProperty( pMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, emissionTex );
	material->Ke[0] = static_cast<GLfloat>( lEmissive[0] );
	material->Ke[1] = static_cast<GLfloat>( lEmissive[1] );
	material->Ke[2] = static_cast<GLfloat>( lEmissive[2] );
	material->emissionMap = emissionTex;

	Texture *ambientTex = NULL;
	const FbxDouble3 lAmbient = GetMaterialProperty( pMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, ambientTex );
	material->Ka[0] = static_cast<GLfloat>( lAmbient[0] );
	material->Ka[1] = static_cast<GLfloat>( lAmbient[1] );
	material->Ka[2] = static_cast<GLfloat>( lAmbient[2] );
	material->ambientMap = ambientTex;

	Texture *diffuseTex = NULL;
	const FbxDouble3 lDiffuse = GetMaterialProperty( pMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, diffuseTex );
	material->Kd[0] = static_cast<GLfloat>( lDiffuse[0] );
	material->Kd[1] = static_cast<GLfloat>( lDiffuse[1] );
	material->Kd[2] = static_cast<GLfloat>( lDiffuse[2] );
	material->diffuseMap = diffuseTex;

	Texture *specularTex = NULL;
	const FbxDouble3 lSpecular = GetMaterialProperty( pMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, specularTex );
	material->Ks[0] = static_cast<GLfloat>( lSpecular[0] );
	material->Ks[1] = static_cast<GLfloat>( lSpecular[1] );
	material->Ks[2] = static_cast<GLfloat>( lSpecular[2] );
	material->specularMap = specularTex;

	FbxProperty lShininessProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sShininess );
	if ( lShininessProperty.IsValid() )
	{
		double lShininess = lShininessProperty.Get<FbxDouble>();
		material->shininess = static_cast<GLfloat>( lShininess );
	}

	return true;
}

// Get specific property value and connected texture if any Value = Property value * Factor property value (if no factor property, multiply by 1).
FbxDouble3 MANA3D::GetMaterialProperty( const FbxSurfaceMaterial *pMaterial, const char *pPropertyName, const char *pFactorPropertyName, Texture *&pTexture )
{
	FbxDouble3 lResult( 0, 0, 0 );
	const FbxProperty lProperty = pMaterial->FindProperty( pPropertyName );
	const FbxProperty lFactorProperty = pMaterial->FindProperty( pFactorPropertyName );
	
	if ( lProperty.IsValid() && lFactorProperty.IsValid() )
	{
		lResult = lProperty.Get<FbxDouble3>();
		double lFactor = lFactorProperty.Get<FbxDouble>();
		if ( lFactor != 1 )
		{
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;
		}
	}

	if ( lProperty.IsValid() )
	{
		// Check if it's layeredtextures
		int layered_texture_count = lProperty.GetSrcObjectCount<FbxLayeredTexture>();
		if ( layered_texture_count > 0 )
		{
			for ( int j=0; j < layered_texture_count; j++ )
			{
				FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>( lProperty.GetSrcObject<FbxLayeredTexture>(j) );
				int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();
				for ( int k = 0; k < lcount; k++ )
				{
					FbxTexture* texture = FbxCast<FbxTexture>( layered_texture->GetSrcObject<FbxTexture>( k ) );
					// Then, you can get all the properties of the texture, include its name
					pTexture = Texture::GetWithName( texture->GetName() );
				}
			}
		}
		else 
		{
			// Directly get textures
			int texture_count = lProperty.GetSrcObjectCount<FbxTexture>();
			for ( int j = 0; j < texture_count; j++ )
			{
				const FbxTexture *texture = FbxCast<FbxTexture>( lProperty.GetSrcObject<FbxTexture>( j ) );
				// Then, you can get all the properties of the texture, include its name
				pTexture = Texture::GetWithName( texture->GetName() );
			}
		}
	}

	return lResult;
}

// Unload the cache and release the memory fro this scene and release the textures in GPU
void MANA3D::UnloadTextures( FbxScene *pScene )
{
    const int lTextureCount = pScene->GetTextureCount();
    for ( int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex )
    {
        FbxTexture * lTexture = pScene->GetTexture( lTextureIndex );
        FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>( lTexture );
        if ( lFileTexture && lFileTexture->GetUserDataPtr() )
        {
            GLuint *lTextureName = static_cast<GLuint *>( lFileTexture->GetUserDataPtr() );
            lFileTexture->SetUserDataPtr( NULL );
            //glDeleteTextures( 1, lTextureName );
            delete lTextureName;
        }
    }
}
