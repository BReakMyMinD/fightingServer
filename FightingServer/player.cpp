#include "player.h"

Player::Player(QTcpSocket* socket) {
	_socket = socket;
	connect(_socket, &QTcpSocket::readyRead, this, &Player::readData);
	connect(_socket, &QTcpSocket::disconnected, this, &Player::disconnect);
	this->descriptor = _socket->socketDescriptor();
	in.setDevice(_socket);
	in.setVersion(QDataStream::Qt_5_9);
}

void Player::readData() {
	//in.startTransaction();
	qint8 code;
	in >> code;
	//if (!in.commitTransaction()) {
		//return;
	//}
	switch (code) {
	case CREATE_LOBBY: {
		QString name;
		in >> name;
		this->name = name;
		this->status = OWNER_WAITING;
		this->writeData<int>(LOBBY_CREATED, 0);
		break;
	}
	case GET_LOBBY_LIST: {
		QString name;
		in >> name;
		this->name = name;
		this->status = GUEST_WAITING;
		emit getLobbyList();
		break;
	}
	case JOIN_LOBBY: {
		int lobbyDescriptor;
		in >> lobbyDescriptor;
		emit joinLobby(lobbyDescriptor);
		break;
	}
	case KEY_PRESS: {
		int key;
		in >> key;
		qDebug() << name << " pressed " << key;
		switch (key) {
		case(Qt::Key_A): {
			getCharacter()->moveLeft();
			break;
		}
		case(Qt::Key_D): {
			getCharacter()->moveRight();
			break;
		}
		case(Qt::Key_W): {
			getCharacter()->jump();
			break;
		}
		}

		break;
	}
	}
}

template<class T>
void Player::writeData(qint8 code, T data) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);
	out << code << data;
	_socket->write(block);
}

void Player::lobbyListGot(QStringList& list) {
	writeData<QStringList>(LOBBY_LIST_GOT, list);
}

void Player::lobbyJoined() {
	writeData<int>(LOBBY_JOINED, 0);
}

void Player::setLobby(Lobby *lobbyPtr) {
	lobby = lobbyPtr;
	if (status == OWNER_WAITING) {
		status = OWNER_PLAYING;
	}
	if (status == GUEST_WAITING) {
		status = GUEST_PLAYING;
	}
	writeData<int>(LOBBY_JOINED, 0);
}

Character* Player::getCharacter() {
	if (status == OWNER_PLAYING) {
		return lobby->owner;
	}
	if (status == GUEST_PLAYING) {
		return lobby->guest;
	}
}

Character* Player::getOpponentCharacter() {
	if (status == OWNER_PLAYING) {
		return lobby->guest;
	}
	if (status == GUEST_PLAYING) {
		return lobby->owner;
	}
}

void Player::sendGameState() {
	QPair<Character*, Character*> gameData;
	gameData.first = getCharacter();
	gameData.second = getOpponentCharacter();
	writeData<QPair<Character*, Character*>>(GAME_UPDATE, gameData);
}

void Player::disconnect() {
	emit leave();
}