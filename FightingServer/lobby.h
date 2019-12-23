#include <QString>
#include <QTimer>
#include <QObject>
#include "character.h"



class Lobby : public QObject {
	Q_OBJECT
public:
	Lobby();
	~Lobby();
	Character *owner;
	Character *guest;


public slots:
	void timerUpdate();
	

signals:
	void gameUpdated();//сигнал обновления сцены вызывается по таймеру
	
private:
	QTimer* _gameTimer;
};



