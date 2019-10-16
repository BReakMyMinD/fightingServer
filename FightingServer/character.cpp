#include "character.h"

Character::Character(bool isLeftPosition) {
	hp = 100;
	y = 0;
	vx = 0;
	vy = -10;
	if (isLeftPosition) {
		x = 0;
	}
	else {
		x = 500;
	}
};

void Character::moveLeft() {
	vx = -10;
}

void Character::moveRight() {
	vx = 10;
}

void Character::jump() {
	vy = 10;
}