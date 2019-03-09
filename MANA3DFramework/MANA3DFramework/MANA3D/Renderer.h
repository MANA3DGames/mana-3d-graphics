#ifndef MANA3D_RENDERER_H
#define MANA3D_RENDERER_H

#include "Component.h"

#include <list>
using std::list;

namespace MANA3D
{
	class Camera;
	class Renderer : public Component
	{
	protected:
		Renderer();
		virtual ~Renderer();

		virtual void Render();


		friend Camera;
	private:
		static list<Renderer*> renderers;

		static void RenderAll();
	};
}

#endif
