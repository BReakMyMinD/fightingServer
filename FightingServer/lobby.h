#include <QString>

class Player {
	QString name;
	int socketDescriptor;
};

class Lobby {
public:
	Lobby(Player playerOne, Player playerTwo);

private:
	Player _playerOne, _playerTwo;
};