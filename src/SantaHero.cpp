#include "SantaHero.hpp"
#include "System.hpp"
#include "Projectile.hpp"

#define TEAM 1

using namespace mcDirr;

std::shared_ptr<SantaHero> SantaHero::getInstance(SDL_Surface* surf, SDL_Surface* fireSheet, SDL_Texture* healthSym, GameEngine* GEpek, void (*resetFunk)()) {
	std::shared_ptr<SantaHero> newMe( new SantaHero(surf, fireSheet, healthSym, GEpek, resetFunk) );
	newMe->me = newMe;
	return newMe;
}

SantaHero::SantaHero(SDL_Surface* surf, SDL_Surface* fireSheet, SDL_Texture* healthSym, GameEngine* GEpek, void (*resetFunk)())
	: Sprite(surf, 0, 0), FramedSprite(surf, 0, 0, 11), AnimatedMobileSprite(surf, 0, 0, 11, 10, healthSym, TEAM),
	firesheet(fireSheet), gameEngine(GEpek), resetFunc(resetFunk) {
}

void SantaHero::checkBounds() {
	int winWidth = 0;
	int winHeight = 0;

	SDL_Window* window = sys.getWin();
	SDL_GetWindowSize(window, &winWidth, &winHeight);

	if (dest.y > winHeight) {
		gameEngine->setScreen(1);
		resetFunc();

		Level* lvl = gameEngine->getLevel();
		if (lvl != nullptr) {
			dest.x = gameEngine->getLevel()->getStartX();
			dest.y = gameEngine->getLevel()->getStartY();
			yVel = 0;
		}
		return;
	}

	if (dest.x <= 0) {
		if (gameEngine->getScreenIndex() > 1) {
			gameEngine->previousScreen();
			dest.x = gameEngine->getLevel()->getEndX();
			dest.y = gameEngine->getLevel()->getEndY();
			gameEngine->getLevel()->setSpriteOutBox(this);
		} else {
			dest.x = 1;
		}
		xVel = 0;
		yVel = 0;
		return;

	} else if (dest.x > winWidth) {
		dest.x = winWidth - 1;
		xVel = 0;
		gameEngine->nextScreen();

		Level* lvl = gameEngine->getLevel();

		if (lvl != nullptr) {
			if (!lvl->exists(me)) {
				std::shared_ptr<MobileSprite> mobileMe = me;
				lvl->add(mobileMe);
			} else {
				lvl->setSpriteOutBox(this);
			}

			yVel = 0;
			dest.x = lvl->getStartX();
			dest.y = lvl->getStartY();
		}
	}
}


void SantaHero::customTick(int timeDiff) {
	if (projCooldown > 0)
		projCooldown -= timeDiff;

	if (sys.isKeyDown(SDLK_w) && onGround)
		yVel -= 1;

	if (sys.isKeyDown(SDLK_a))
		xVel = -0.5;

	else if (sys.isKeyDown(SDLK_d))
		xVel = 0.5;

	if (sys.isKeyDown(SDLK_SPACE) && projCooldown <= 0) {
		projCooldown = 200;
		SDL_Rect* rect = getDestRect();
		int projX = rect->x + 50;
		std::shared_ptr<MobileSprite> sprite = Projectile::getInstance(firesheet, projX, rect->y, 3, 70, healthSymbol, 1, facingRight);
		sprite->setYAccel(0.1);
		sprite->setBouncy(false);

		getSpriteOutbox()->push(sprite);
	}

	if (xVel < 0) {
		setStartFrame(2);
		setEndFrame(60);
		facingRight = false;
	} else if (xVel > 0) {
		setStartFrame(61);
		setEndFrame(120);
		facingRight = true;
	} else {
		if (!facingRight) {
			setStartFrame(0);
			setEndFrame(0);
		} else {
			setStartFrame(1);
			setEndFrame(1);
		}
	}
	checkBounds();
}

void SantaHero::kill() {
	if (health > 0) {
		changeHealth(-1);
	} else {
		gameEngine->setScreen(1);
		resetFunc();
	}
}
