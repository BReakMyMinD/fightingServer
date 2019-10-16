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
		//��������������� ������ ������, ����������� ��� ���������
		return stream << character->x << character->y << character->hp;
	}
	

public slots:
	void moveRight();
	void moveLeft();
	void jump();

private:
	qint8 x,
		vx,//�����. ���������
		y,
		vy,//����. ���������
		hp,
		width, heigth;//������� ��������
};