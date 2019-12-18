#include "character.h"
#include <QDebug>

Character::Character(bool isLeftPosition) {
	data.hp = 100;
	data.y = 480;
	data.width = 56;
	data.height = 120;
	vy = 0;
	if (isLeftPosition) {
		data.x = 250;
	}
	else {
		data.x = 650;
	}
};

void Character::moveLeft() {
	data.x -= 10;
}

void Character::moveRight() {
	data.x += 10;
}

void Character::jump() {
	if (vy == 0) {
		vy = 20;
	}
	else {
		return;
	}
}