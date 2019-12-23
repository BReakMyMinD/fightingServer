#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include "lobby.h"
#include "types.h"

enum Status {
	NO_INFO,
	OWNER_WAITING,
	GUEST_WAITING,
	OWNER_PLAYING,
	GUEST_PLAYING
};

class Player : public QObject {
	Q_OBJECT
public:
	Player(QTcpSocket* socket);

	Lobby* lobby = nullptr;
	QString name;
	Status status = NO_INFO;
	void setLobby(Lobby* lobby);
	void setupUdp(qint32 port);
	Character* getCharacter();
	Character* getOpponentCharacter();

	qint32 descriptor;
	qint32 udpPort;

	void lobbyListGot(QStringList& list);

public slots:
	void sendGameState();
	void finishGame(const QString& msg);
	void playerWin();

signals:
	void getLobbyList();
	void joinLobby(qint32 descriptor);
	void gameFinished(QString& msg);
	void destroy();
	
private slots:
	void readData();
	void readKey();
	void disconnect();
	
private:
	template<class T>
	void writeData(qint8 code, T data);

	QTcpSocket* cmdSocket;
	QDataStream in;
	QHostAddress ip;
	QUdpSocket* gameSocket;
};