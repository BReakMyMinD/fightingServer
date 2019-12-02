#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDataStream>
#include <QMap>
#include <QVector>
#include <QStringList>
#include <QObject>
#include <QString>
#include <QPair>
#include <QTimer>
#include "player.h"
#include "character.h"

class Server : public QObject {
	Q_OBJECT
public:
	void startServer(int port);

public slots:
	void joinLobby(int descriptor);
	void getLobbyList();

private slots:
	void incomingConnection();
	void socketDisconnected();
	

private:
	QUdpSocket *_socket;
	//QTcpServer* _tcpServer;
	QMap<int, Player*> _players;
	//QVector<Player*> _players;
};
