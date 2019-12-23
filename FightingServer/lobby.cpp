#include "lobby.h"

Lobby::Lobby() {
	owner = new Character(true);
	guest = new Character(false);
	
	_gameTimer = new QTimer(this);
	connect(_gameTimer, &QTimer::timeout, this, &Lobby::timerUpdate);
	_gameTimer->start(25);//частота обновления сцены в миллисекундах
}

void Lobby::timerUpdate() {
	//обработка механики
	//реализовать всю механику как методы Character
	
	
	if (owner->isCollision(guest) && owner->data.hit == 1) {
		guest->data.hp -= 5;
		guest->isAlive();
	};
	if (guest->isCollision(owner) && guest->data.hit == 1) {
		owner->data.hp -= 5;
		owner->isAlive();
	};
	
	owner->move();
	guest->move();
	

	emit gameUpdated();
}

Lobby::~Lobby() {
	_gameTimer->stop();
	_gameTimer->deleteLater();
	owner->deleteLater();
	guest->deleteLater();
}



