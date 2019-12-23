#include "character.h"

Character::Character(bool isLeftPosition) {
	data.hp = 100;
	data.y = 320;
	data.width = 160;
	data.height = 220;
	data.vy = 0;
	data.vx = 0;
	data.hit = 0;
	if (isLeftPosition) {
		data.x = 150;
	}
	else {
		data.x = 550;
	}
};

void Character::jump() {
	if (data.vy == 0) {
		data.vy = 20;
	}
	else {
		return;
	}
}

void Character::hit() {
	if (data.hit == 0) {
		data.hit = 1;
	}
	else {
		return;
	}
}

bool Character::isCollision(const Character* opponent) {
	if (data.x + data.width/2 > opponent->data.x + opponent->data.width/2 - 85 && 
		data.x + data.width/2 < opponent->data.x + opponent->data.width/2 + 85 &&
		data.y >= opponent->data.y - 80) {
		return true;
	}
	else {
		return false;
	}
}

void Character::move() {
	if (data.vx > 0 && data.x >= 630) {
		data.vx = 0;
	}
	if (data.vx < 0 && data.x <= 0) {
		data.vx = 0;
	}
	
	data.x += data.vx;
	
	if (data.hit > 0 && data.hit <= 6) {
		++data.hit;
	}
	else {
		data.hit = 0;
	}
	if (data.vy != 0) {
		data.y -= data.vy;
		if (data.vy == 1) {
			data.vy = 0;
		}
		if (data.vy == -20) {
			data.vy = 1;
		}
		data.vy -= 1;
	}
}

void Character::isAlive() {
	if (data.hp <= 0) {
		emit characterKilled();
	}
	else {
		return;
	}
}
