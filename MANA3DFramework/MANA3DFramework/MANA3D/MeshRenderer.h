#ifndef MANA3D_MESHRENDERER_H
#define MANA3D_MESHRENDERER_H

#include "Mesh.h"
#include "Renderer.h"

namespace MANA3D
{
	class MeshRenderer : public Renderer
	{
		friend class GameObject;

	public:
		void SetMesh( Mesh *mesh );

	protected:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Render();
		//virtual void RenderChildren();

	private:
		void DrawMesh( void );
		void DrawMesh( Mesh *mesh );

		Mesh *mesh;
	};
}

#endif
