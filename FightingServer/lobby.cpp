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

QTcpSocket* Lobby::remainingSocket(QTcpSocket* disconnectedSocket) {
	_gameTimer->stop();
	_gameTimer->deleteLater();
	if (disconnectedSocket == _playerOneSocket) {
		return _playerTwoSocket;
	}
	if (disconnectedSocket == _playerTwoSocket) {
		return _playerOneSocket;
	}
}

void Player::setLobby(Lobby * lobbyPtr) {
	lobby = lobbyPtr;
	playerStatus = IS_PLAYING;
}

void Player::moveLeft() {
	if (this->lobby->_playerOneSocket->socketDescriptor() == id) {
		this->lobby->playerOneCharacter
	}
}

void Player::moveRight() {

}

void Player::jump() {

}
