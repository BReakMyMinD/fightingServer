#pragma once
#include "qnumeric.h"
#include <QObject>
#include <QDataStream>



class Character : public QObject{
	Q_OBJECT
public:
	Character() {};
	Character(bool isLeftPosition);

	bool isCollision(const Character* opponent);
	
	struct charData {
		qint16 x, y, vy, vx;
		qint8 hit, hp, width, height;
	} data;
	
	
	friend QDataStream& operator << (QDataStream& stream, const charData& item) {
		stream << item.x << item.y << item.vx << item.vy << item.hp << item.hit;
		return stream;
	}
	

	void move();
	void jump();
	void hit();
	void isAlive();

signals:
	void characterKilled();
};