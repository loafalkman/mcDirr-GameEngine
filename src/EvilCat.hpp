#ifndef EVILCAT_HPP
#define EVILCAT_HPP

#include "InteractionSprite.hpp"

namespace mcDirr {
	class EvilCat : public InteractionSprite {
		public:
			static EvilCat* getInstance(SDL_Surface* surface, int x, int y);

		protected:
			void customTick(int timeDiff);
			EvilCat(SDL_Surface* s, int x, int y);

		private:
			void handleCollision(MobileSprite* collidedWith, int side);

	};
}

#endif
