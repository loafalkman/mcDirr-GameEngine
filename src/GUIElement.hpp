#ifndef GUIELEMENT_HPP
#define GUIELEMENT_HPP

#include <SDL.h>

#include "Sprite.hpp"

namespace mcDirr {

	class GUIElement: public Sprite {

		public:
			GUIElement(SDL_Surface* surf);
			virtual void mouseClick() = 0;
			virtual ~GUIElement() {}
	};
}

#endif // GUIELEMENT_HPP
