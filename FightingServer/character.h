#pragma once
#include "qnumeric.h"
#include <QObject>
#include <QDataStream>

class Character : public QObject{
	Q_OBJECT
public:
	Character() {};
	Character(bool isLeftPosition);

	

	friend QDataStream& operator<<(QDataStream& stream, const Character* character) { 
		//сериализовывать только данные, необходимые для отрисовки
		return stream << character->x << character->y << character->hp;
	}
	

public slots:
	void moveRight();
	void moveLeft();
	void jump();

private:
	qint8 x,
		vx,//гориз. ускорение
		y,
		vy,//верт. ускорение
		hp,
		width, heigth;//размеры хитбокса
};