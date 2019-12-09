#include "character.h"

Character::Character(bool isLeftPosition) {
	data.hp = 100;
	data.y = 0;
	if (isLeftPosition) {
		data.x = 0;
	}
	else {
		data.x = 300;
	}
};

void Character::moveLeft() {
	data.x -= 10;
}

void Character::moveRight() {
	data.x += 10;
}

void Character::jump() {
	data.y -= 10;
}