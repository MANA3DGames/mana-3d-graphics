#include "Renderer.h"
using namespace MANA3D;

#include <Windows.h>

Renderer::Renderer() : Component()
{
	renderers.push_back( this );
}

Renderer::~Renderer()
{
	renderers.remove( this );
}


void Renderer::Render()
{

}


list<Renderer*> Renderer::renderers;
void Renderer::RenderAll()
{
	for ( list<Renderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it )
	{
		if ( (*it)->gameObject->GetActive() && (*it)->GetEnable() )
		{
			bool draw = true;
			GameObject *root = (*it)->gameObject->parent;
			while ( root != NULL )
			{
				if ( !root->GetActive() )
				{
					draw = false;
					break;
				}
				root = root->parent;
			}

			if ( draw )
				(*it)->Render();
		}
	}
}
