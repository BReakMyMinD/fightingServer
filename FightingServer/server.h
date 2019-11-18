#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QHash>
#include <QVector>
#include <QStringList>
#include <QObject>
#include <QString>
#include <QPair>
#include "types.h"
#include "lobby.h"
#include "character.h"

class Server : public QObject {
	Q_OBJECT
public:
	void startServer(int port);


private slots:
	void readData();
	void incomingConnection();
	void sendGameState(QTcpSocket *playerOne, QTcpSocket *playerTwo);//отправляет клиентам обновленную информацию о игровой сессии
	void socketDisconnected();

private:
	QDataStream in;
	QTcpServer* _tcpServer;
	QHash<QTcpSocket*, Player*> _players;
	template<class T>
	void writeData(QTcpSocket* client, qint8 num, T data);
};
