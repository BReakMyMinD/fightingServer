#include "server.h"





void Server::startServer(int port) {
	_socket = new QUdpSocket(this);
	_socket->bind(QHostAddress::LocalHost, 1025);
	qDebug() << "Listening to port " << port;
	connect(_socket, &QUdpSocket::readyRead, this, &Server::incomingConnection);
}

void Server::incomingConnection() {
	while (_socket->hasPendingDatagrams()) {
		QNetworkDatagram datagram = _socket->receiveDatagram();
		int i = 1026;
		while(i < 49150){
			if (_players.contains(i)) {
				i++;
			}
			else {
				Player* playerObj = new Player(datagram.senderAddress(), i);
				_players[i] = playerObj;
				playerObj->setPort(i);
				connect(playerObj, &Player::getLobbyList, this, &Server::getLobbyList);
				connect(playerObj, &Player::joinLobby, this, &Server::joinLobby);
				connect(playerObj, &Player::leave, this, &Server::socketDisconnected);
				break;
			}
		}
	}
	
		
	
}

void Server::getLobbyList() {
	Player* player = (Player*)sender();
	QStringList list;
	for (auto item : _players) {
		if (item->status == OWNER_WAITING) {
			list.append(item->name + "#" + QString::number(item->descriptor));
		}
	}
	player->lobbyListGot(list);
}

void Server::joinLobby(int descriptor) {
	Player* player = (Player*)sender();
	for (auto item : _players) {
		if (item->status == OWNER_WAITING && item->descriptor == descriptor) {
			Lobby* lobbyObj = new Lobby();
			player->setLobby(lobbyObj);
			item->setLobby(lobbyObj);
			connect(lobbyObj, &Lobby::gameUpdated, player, &Player::sendGameState);
			connect(lobbyObj, &Lobby::gameUpdated, item, &Player::sendGameState);
		}
	}
}
/*void Server::sendGameState(QTcpSocket *playerOne, QTcpSocket *playerTwo) {
	QPair<Character*, Character*> gameData;
	gameData.first = _players[playerOne]->lobby->playerOneCharacter;
	gameData.second = _players[playerOne]->lobby->playerTwoCharacter;
	writeData<QPair<Character*, Character*>>(playerOne, GAME_UPDATE, gameData);
	writeData<QPair<Character*, Character*>>(playerTwo, GAME_UPDATE, gameData);
}*/

void Server::socketDisconnected() {
	/*QTcpSocket* disconnectedSocket = (QTcpSocket*)sender();
	if (_players[disconnectedSocket]->lobby != nullptr) {
		QTcpSocket* remainingSocket = _players[disconnectedSocket]->lobby->remainingSocket(disconnectedSocket);
		writeData<QString>(remainingSocket, PLAYER_LEFT, _players[disconnectedSocket]->name);
		_players[remainingSocket]->playerStatus = NO_INFO;
		_players[disconnectedSocket]->lobby->deleteLater();
		_players[disconnectedSocket]->lobby = nullptr;
	}
	_players.remove(disconnectedSocket);
	disconnectedSocket->deleteLater();*/
}

/*void Server::readData() {//обработка любых входящих данных
	QTcpSocket* clientSocket = (QTcpSocket*)sender();
	//int descriptor = clientSocket->socketDescriptor();

	in.setDevice(clientSocket);
	in.setVersion(QDataStream::Qt_5_9);

	in.startTransaction();
	qint8 code;
	in >> code;
	if (!in.commitTransaction()) {
		return;
	}

	try {
		switch (code) {
		case CREATE_LOBBY: {
			QString name;
			in >> name;
			qDebug() << "player " << name << " created lobby";
			_players[clientSocket]->name = name;
			_players[clientSocket]->playerStatus = LOBBY_OWNER;
			writeData<int>(clientSocket, LOBBY_CREATED, 0);
			break;
		}
		case GET_LOBBY_LIST: {
			QString name;
			in >> name;
			qDebug() << "player " << name << " is searching lobby";
			_players[clientSocket]->name = name;
			_players[clientSocket]->playerStatus = LOBBY_GUEST;
			QStringList list;
			QHash<QTcpSocket*, Player*>::iterator i;
			for (i = _players.begin(); i != _players.end(); ++i) {
				if (i.value()->playerStatus == LOBBY_OWNER) {
					list.append(i.value()->name + "#" + QString::number(i.key()->socketDescriptor()));
				}
			}
			writeData<QStringList>(clientSocket, LOBBY_LIST_GOT, list);
			break;
		}
		case JOIN_LOBBY: {
			int lobbyDescriptor;
			in >> lobbyDescriptor;
			QHash<QTcpSocket*, Player*>::iterator i;
			for (i = _players.begin(); i != _players.end(); ++i) {
				if (i.value()->playerStatus == LOBBY_OWNER && i.key()->socketDescriptor() == lobbyDescriptor) {
					Lobby* newLobby = new Lobby(clientSocket, i.key());
					_players[clientSocket]->setLobby(newLobby);
					_players[i.key()]->setLobby(newLobby);
					writeData<int>(clientSocket, LOBBY_JOINED, 0);
					writeData<int>(i.key(), LOBBY_JOINED, 0);
					connect(newLobby, &Lobby::gameUpdated, this, &Server::sendGameState);
					//коннекты пользовательских действий к слоту персонажа
					/*connect(_players[clientSocket], &Player::moveLeft, newLobby->playerOneCharacter, &Character::moveLeft);
					connect(_players[clientSocket], &Player::moveRight, newLobby->playerOneCharacter, &Character::moveRight);
					connect(_players[clientSocket], &Player::jump, newLobby->playerOneCharacter, &Character::jump);
					connect(_players[i.key()], &Player::moveLeft, newLobby->playerTwoCharacter, &Character::moveLeft);
					connect(_players[i.key()], &Player::moveRight, newLobby->playerTwoCharacter, &Character::moveRight);
					connect(_players[i.key()], &Player::jump, newLobby->playerTwoCharacter, &Character::jump);
					break;
				}
			}
			break;
		}
		case KEY_PRESS: {
			int keyCode;
			in >> keyCode;
			qDebug() << _players[clientSocket]->name << " pressed " << keyCode;
			switch (keyCode) {
			case(Qt::Key_A): {
				//emit _players[clientSocket]->moveLeft();
				break;
			}
			case(Qt::Key_D): {
				//emit _players[clientSocket]->moveRight();
				break;
			}
			case(Qt::Key_W): {
				//emit _players[clientSocket]->jump();
				break;
			}
			}
			
			break;
		}
		default: {
			break;
		}
		}
	}
	catch (...) {
		writeData<QString>(clientSocket, SERVER_ERROR, "Internal server error");
	}
}

template<class T>//универсальный метод для отправки данных клиенту
void Server::writeData(QTcpSocket* client, qint8 code, T data) { //T - только сериализуемые qt типы данных! https://doc.qt.io/qt-5/datastreamformat.html
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);
	out << code << data;
	client->write(block);
}*/