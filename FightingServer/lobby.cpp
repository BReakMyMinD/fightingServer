#include "lobby.h"
Lobby::Lobby() {}

Lobby::Lobby(QTcpSocket *playerOne, QTcpSocket *playerTwo) {
	_playerOneSocket = playerOne;
	_playerTwoSocket = playerTwo;
	playerOneCharacter = new Character(true);
	playerTwoCharacter = new Character(false);
	
	_gameTimer = new QTimer(this);
	connect(_gameTimer, &QTimer::timeout, this, &Lobby::timerUpdate);
	_gameTimer->start(500);//частота обновления сцены в миллисекундах
}

void Lobby::timerUpdate() {
	//обработка механики
	//реализовать всю механику как методы Character
	emit gameUpdated(_playerOneSocket, _playerTwoSocket);
}

void Player::setLobby(Lobby * lobby) {
	_lobby = lobby;
	playerStatus = IS_PLAYING;
}
