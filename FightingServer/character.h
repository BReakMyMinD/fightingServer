#pragma once
#include "qnumeric.h"
#include <QObject>
#include <QDataStream>



class Character : public QObject{
	Q_OBJECT
public:
	Character() {};
	Character(bool isLeftPosition);

	//void interact(Character& opponentCharacter);
	struct charData {
		qint16 x, y;
		qint8 hp, width, height;
	} data;
	qint16 vy;
	
	friend QDataStream& operator << (QDataStream& stream, const charData& item) {
		stream << item.x << item.y << item.hp;
		return stream;
	}
	

public slots:
	void moveRight();
	void moveLeft();
	void jump();

private:
	 //вертикальное ускорение
	/*qint8 x,
		vx,//гориз. ускорение
		y,
		vy,//верт. ускорение
		hp,
		width, heigth;//размеры хитбокса*/
};