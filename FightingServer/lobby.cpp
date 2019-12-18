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
	if (owner->vy != 0) {
		owner->data.y -= owner->vy;
		if (owner->vy - 1 == 0) {
			owner->vy = -21;
		}
		if (owner->vy > 0) {
			owner->vy -= 1;
		}
		else if (owner->vy < 0) {
			owner->vy += 1;
		}
	}
	if (guest->vy != 0) {
		guest->data.y -= guest->vy;
		if (guest->vy - 1 == 0) {
			guest->vy = -21;
		}
		if (guest->vy > 0) {
			guest->vy -= 1;
		}
		else if (guest->vy < 0) {
			guest->vy += 1;
		}
	}

	emit gameUpdated();
}

Lobby::~Lobby() {
	_gameTimer->stop();
	_gameTimer->deleteLater();
	owner->deleteLater();
	guest->deleteLater();
}



