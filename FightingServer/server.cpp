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
		int descriptor = clientSocket->socketDescriptor();

		qDebug() << descriptor << " connected";
		connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readData); 
		connect(clientSocket, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
	}
}

void Server::socketDisconnected() {
	QTcpSocket* disconnectedSocket = (QTcpSocket*)sender();
	int descriptor = disconnectedSocket->socketDescriptor();

	qDebug() << descriptor << " disconnected";
	if (waitingPlayers.contains(descriptor)) {
		waitingPlayers.remove(descriptor);

		if (!waitingPlayers.empty()) {//обновление списка ожидающих игроков если один из них вышел
			QMap<int, QString>::iterator i;
			for (i = waitingPlayers.begin(); i != waitingPlayers.end(); ++i) {
				QTcpSocket* clientSocket = new QTcpSocket();
				clientSocket->setSocketDescriptor(i.key);
				writeData<QMap<int, QString>>(clientSocket, PLAYER_ADDED, waitingPlayers);
			}
		}
	}
	disconnectedSocket->deleteLater();
}

void Server::readData() {//обработка любых входящих данных
	QTcpSocket* clientSocket = (QTcpSocket*)sender();
	int descriptor = clientSocket->socketDescriptor();

	in.setDevice(clientSocket);
	in.setVersion(QDataStream::Qt_5_9);

	in.startTransaction();
	quint8 code;
	in >> code;
	if (!in.commitTransaction()) {
		return;
	}

	switch (code) {
	case ADD_PLAYER: {
		QString name;
		in >> name;
		qDebug() << "player " << name << " added";
		waitingPlayers[descriptor] = name;
		writeData<QMap<int, QString>>(clientSocket, PLAYER_ADDED, waitingPlayers);
		break;
	}
	}
}

template<class T>//универсальный метод для отправки данных клиенту
void Server::writeData(QTcpSocket* client, qint8 code, T data) { //T - только сериализуемые qt типы данных! https://doc.qt.io/qt-5/datastreamformat.html
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_9);
	out << code << data;
	client->write(block);
}