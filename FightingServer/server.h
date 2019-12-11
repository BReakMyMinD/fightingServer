#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QHash>
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
	
	QTcpServer* _tcpServer;
	QVector<Player*> _players;
	QList<int> occupiedUdpPorts;
};
