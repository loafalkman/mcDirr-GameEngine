#include "EvilCat.hpp"
#include "Particle.hpp"

#include <math.h>
#include <stdlib.h>

#define TEAM 2

using namespace mcDirr;

std::shared_ptr<EvilCat> EvilCat::getInstance(SDL_Surface* surface, SDL_Texture* healthSym) {
	return std::shared_ptr<EvilCat>(new EvilCat(surface, healthSym) );
}

EvilCat::EvilCat(SDL_Surface* surface, SDL_Texture* healthSym) :
	Sprite(surface, 0, 0), MobileSprite(surface, 0, 0, healthSym, TEAM) {
}

void EvilCat::customTick(int timeDiff) {
	damageCooldown -= timeDiff;
}

void EvilCat::handleMobileCollision(std::shared_ptr<MobileSprite> collidedWith, int side) {

	if (side && collidedWith->getTeam() == 1 && damageCooldown <= 0 ) {
		collidedWith->kill();
		damageCooldown = 500;
	}
}

void EvilCat::kill() {
	// splatter effect
	srand(SDL_GetTicks());
	int origX = dest.x + dest.w / 2;
	int origY = dest.y + dest.h / 2;
	float amount = 10;
	float velocity = 0.7f;

	if (health == 1) {
		amount = 50;
	}

	for (int i = 0; i < amount; i++) {
		int duration = rand() % 1500 + 1500;
		float rnd = (rand() % 1000) / 990.0f;
		float xVel = sin( (i/amount) * 2.0f * M_PI) * velocity + rnd;
		float yVel = cos( (i/amount) * 2.0f * M_PI) * velocity + rnd;

		std::shared_ptr<Particle> p = Particle::getInstance(getSurface(), origX, origY, xVel, yVel, duration);
		p->setBouncy(false);
		p->setCanCollide(false);
		getSpriteOutbox()->push(p);
	}

	changeHealth(-1);
	
	if (health <= 0) {
		alive = false;
	}
}
