#include <QString>
#include <QTcpSocket>
#include <QTimer>
#include <QObject>
#include "character.h"

enum status {
	NO_INFO,
	LOBBY_OWNER,
	LOBBY_GUEST,
	IS_PLAYING
};

class Lobby : public QObject {
	Q_OBJECT
public:
	Lobby();
	Lobby(QTcpSocket* playerOne, QTcpSocket* playerTwo);
	Character *playerOneCharacter, *playerTwoCharacter;
public slots:
	void timerUpdate();

signals:
	void gameUpdated(QTcpSocket *playerOne, QTcpSocket *playerTwo);//сигнал обновления сцены вызывается по таймеру
private:
	QTimer* _gameTimer;
	QTcpSocket *_playerOneSocket, *_playerTwoSocket;
	
};

class Player : public QObject{
	Q_OBJECT
public:
	Player() {};
	
	Lobby* _lobby;
	QString name;
	status playerStatus = NO_INFO;
	void setLobby(Lobby* lobby);
	
signals:
	void moveLeft();
	void moveRight();
	void jump();
};

