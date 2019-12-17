#include "server.h"





void Server::startServer(int port) {
	_tcpServer = new QTcpServer(this);
	if (!_tcpServer->listen(QHostAddress::Any, port)) {
		qDebug() << "Could not start server";
	}
	else {
		qDebug() << "Listening to port " << port;
		connect(_tcpServer, &QTcpServer::newConnection, this, &Server::incomingConnection);
		
	}
}

void Server::incomingConnection() {
	while (_tcpServer->hasPendingConnections()) {
		QTcpSocket* clientSocket = _tcpServer->nextPendingConnection();
		qint32 descriptor = clientSocket->socketDescriptor();
		qDebug() << descriptor << " connected";
		Player* playerObj = new Player(clientSocket);
		_players.append(playerObj);
		connect(playerObj, &Player::getLobbyList, this, &Server::getLobbyList);
		connect(playerObj, &Player::joinLobby, this, &Server::joinLobby);
		connect(playerObj, &Player::destroy, this, &Server::deletePlayer);
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
			qint32 port = 3026;
			while (port < 40000) {
				if (occupiedUdpPorts.contains(port)) {
					port++;
				}
				else {
					occupiedUdpPorts.append(port);
					occupiedUdpPorts.append(port + 1);
					break;
				}
			}
			player->setupUdp(port);
			item->setupUdp(port + 1);
			Lobby* lobbyObj = new Lobby();
			player->setLobby(lobbyObj);
			item->setLobby(lobbyObj);
			connect(lobbyObj, &Lobby::gameUpdated, player, &Player::sendGameState);
			connect(lobbyObj, &Lobby::gameUpdated, item, &Player::sendGameState);
			
			connect(player, &Player::gameFinished, item, &Player::finishGame);
			connect(item, &Player::gameFinished, player, &Player::finishGame);
		}
	}
}

void Server::deletePlayer() {
	Player* disconnected = (Player*)sender();
	qDebug() << disconnected->name << " removed";
	_players.removeOne(disconnected);
	occupiedUdpPorts.removeOne(disconnected->udpPort);
	disconnected->deleteLater();
}
