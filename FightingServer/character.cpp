#include "character.h"
#include <QDebug>

Character::Character(bool isLeftPosition) {
	data.hp = 100;
	data.y = 300;
	data.width = 56;
	data.height = 120;
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
	data.y -= 10;
}