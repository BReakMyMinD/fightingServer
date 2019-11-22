#include "lobby.h"

Lobby::Lobby() {
	owner = new Character(true);
	guest = new Character(false);
	
	_gameTimer = new QTimer(this);
	connect(_gameTimer, &QTimer::timeout, this, &Lobby::timerUpdate);
	_gameTimer->start(70);//частота обновления сцены в миллисекундах
}

void Lobby::timerUpdate() {
	//обработка механики
	//реализовать всю механику как методы Character
	
	emit gameUpdated();
}




