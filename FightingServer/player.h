#include <QObject>
#include <QHostAddress>
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

	int descriptor;

	void lobbyListGot(QStringList& list);
	//void lobbyJoined();

public slots:
	void sendGameState();

signals:
	void getLobbyList();
	void joinLobby(qint32 descriptor);
	void leave();
	
private slots:
	void readData();
	void readKey();
	void disconnect();
private:
	template<class T>
	void writeData(qint8 code, T data);

	QTcpSocket* cmdSocket; //команды пользователя в меню передаются по tcp
	QDataStream in;
	QHostAddress ip;
	qint32 udpPort;
	QUdpSocket* gameSocket; //данные игрового процесса передаются по udp
};