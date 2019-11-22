#include <QObject>
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
	Character* getCharacter();
	Character* getOpponentCharacter();

	int descriptor;

	void lobbyListGot(QStringList& list);
	void lobbyJoined();

public slots:
	void sendGameState();

signals:
	void getLobbyList();
	void joinLobby(int descriptor);
	void leave();
	
private slots:
	void readData();
	void disconnect();
private:
	template<class T>
	void writeData(qint8 code, T data);

	QTcpSocket* _socket;
	QDataStream in;

};