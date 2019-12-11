#include "player.h"

Player::Player(QTcpSocket* socket) {
	cmdSocket = socket;
	ip.setAddress(socket->peerAddress().toIPv4Address());
	connect(cmdSocket, &QTcpSocket::readyRead, this, &Player::readData);
	connect(cmdSocket, &QTcpSocket::disconnected, this, &Player::disconnect);
	this->descriptor = cmdSocket->socketDescriptor();
	in.setDevice(cmdSocket);
	in.setVersion(QDataStream::Qt_5_9);
}

void Player::readData() {
	in.startTransaction();
	qint8 code;
	in >> code;
	if (!in.commitTransaction()) {
		return;
	}
	switch (code) {
	case CREATE_LOBBY: {
		QString name;
		in >> name;
		this->name = name;
		this->status = OWNER_WAITING;
		this->writeData<qint8>(LOBBY_CREATED, 0);
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
		qint32 lobbyDescriptor;
		in >> lobbyDescriptor;
		emit joinLobby(lobbyDescriptor);
		break;
	}
	/*case KEY_PRESS: {
		int key;
		in >> key;
		//qDebug() << name << " pressed " << key;
		switch (key) {
		
		}

		break;
	}*/
	}
}

void Player::readKey() {
	while (gameSocket->hasPendingDatagrams()) {
		QNetworkDatagram data = gameSocket->receiveDatagram();
		QDataStream buf(data.data());
		qint16 key;
		buf >> key;
		qDebug() << key;
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
	}
}

template<class T>
void Player::writeData(qint8 code, T data) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);
	if (code == GAME_UPDATE) {
		out << data;
		gameSocket->writeDatagram(block, ip, 3025);//3025 - стандартный входной порт клиента
	}
	else {
		out << code << data;
		cmdSocket->write(block);
	}
}

void Player::lobbyListGot(QStringList& list) {
	writeData<QStringList>(LOBBY_LIST_GOT, list);
}

/*void Player::lobbyJoined() {
	writeData<int>(LOBBY_JOINED, 0);
}*/

void Player::setupUdp(qint32 port) {
	gameSocket = new QUdpSocket(this);
	gameSocket->bind(QHostAddress::AnyIPv4, port);
	connect(gameSocket, &QUdpSocket::readyRead, this, &Player::readKey);
	udpPort = port;
}

void Player::setLobby(Lobby *lobbyPtr) {
	lobby = lobbyPtr;
	if (status == OWNER_WAITING) {
		status = OWNER_PLAYING;
	}
	if (status == GUEST_WAITING) {
		status = GUEST_PLAYING;
	}
	writeData<qint32>(LOBBY_JOINED, udpPort);
}

Character* Player::getCharacter() {
	if (status == OWNER_PLAYING) {
		return lobby->owner;
	}
	else if (status == GUEST_PLAYING) {
		return lobby->guest;
	}
	else {
		return nullptr;
	}
}

Character* Player::getOpponentCharacter() {
	if (status == OWNER_PLAYING) {
		return lobby->guest;
	}
	else if (status == GUEST_PLAYING) {
		return lobby->owner;
	}
	else {
		return nullptr;
	}
}

void Player::sendGameState() {
	QPair<Character::charData, Character::charData> gameData;
	gameData.first = getCharacter()->data;
	gameData.second = getOpponentCharacter()->data;
	writeData<QPair<Character::charData, Character::charData>>(GAME_UPDATE, gameData);
}

void Player::disconnect() {
	emit leave();
}